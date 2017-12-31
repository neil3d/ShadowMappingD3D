
//--
float3 g_lgtPt;
float3 g_lgtDir;
float g_cosTheta;

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
};

//--生成Shadow map----------------------------------------------------------------
float4x4 g_lgtWorldViewPrj;

struct lgtVS_OUTPUT
{
	float4 pos : POSITION;
	float2 depth : TEXCOORD0;
};

lgtVS_OUTPUT lgtVS(VS_INPUT vert)
{
	lgtVS_OUTPUT vsout;	
	vsout.pos = mul(float4(vert.pos,1),g_lgtWorldViewPrj);
	vsout.depth.xy = vsout.pos.zw;
	return vsout;
}

float4 lgtPS(float2 depth:TEXCOORD0):COLOR
{
	return depth.x/depth.y;
}

//--
technique genShadowMap
{
	pass p0
	{
		VertexShader = compile vs_1_1 lgtVS();
		PixelShader = compile ps_2_0 lgtPS();
	}
}

//--渲染场景----------------------------------------------------------------
float4x4 g_worldViewPrj;
float4x4 g_world;
float4x4 g_texScaleBiasMat;

texture g_texShadowMap;

sampler2D g_smpShadowMap =
sampler_state
{
    Texture = <g_texShadowMap>;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct shdVS_OUT
{
	float4 pos : POSITION;
	float4 lgtPos : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3 normal : TEXCOORD2;
};

shdVS_OUT shdVS(VS_INPUT vert)
{
	shdVS_OUT vsout;
	vsout.pos = mul(float4(vert.pos,1),g_worldViewPrj);
	vsout.lgtPos = mul(float4(vert.pos,1),g_lgtWorldViewPrj);
	vsout.worldPos = mul(float4(vert.pos,1),g_world);
	vsout.normal = normalize(mul(vert.normal, g_world));
	return vsout;
}

float4 shdPS(float4 lgtPos:TEXCOORD0,
			 float3 worldPos:TEXCOORD1,
			 float3 normal:TEXCOORD2):COLOR
{
	float3 vLight = normalize(worldPos - g_lgtPt);

	//-- 计算灯光颜色
	float4 ambientColor = {0.2, 0.2, 0.2, 1};
	float4 diffuseColor = {0.9, 0.9, 0.9, 1};

	normal = normalize(normal);
	diffuseColor *= saturate(dot(normal, -vLight));

	//--
	if( dot(vLight, g_lgtDir) > g_cosTheta )
	{
		lgtPos = mul(lgtPos, g_texScaleBiasMat);

		float shadow = lgtPos.z/lgtPos.w > tex2Dproj(g_smpShadowMap, lgtPos) ? 0:1;
		return diffuseColor*shadow + ambientColor;
	}
	else
		return ambientColor;
}

technique shdScene
{
	pass p0
	{
		VertexShader = compile vs_1_1 shdVS();
		PixelShader = compile ps_2_0 shdPS();
	}
}