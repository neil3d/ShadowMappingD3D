#include "RenderShadowMapSample.h"
#include "RenderScene.h"

RenderShadowMapSample::RenderShadowMapSample(void)
{
	m_lgtFov = D3DX_PI/3;
	D3DXMatrixPerspectiveFovLH( &m_lightProject, m_lgtFov, 1, 10.0f, 5000.0f);

	//-- init matrix
	float fBias = -0.0005f;
	float fOffsetX = 0.5f + (0.5f / SHADOW_MAP_SIZE);
	float fOffsetY = 0.5f + (0.5f / SHADOW_MAP_SIZE);
	D3DXMATRIX texScaleBiasMat(
		0.5f,     0.0f,     0.0f,      0.0f,
		0.0f,    -0.5f,     0.0f,      0.0f,
		0.0f,     0.0f,     1.0f,	   0.0f,
		fOffsetX, fOffsetY, fBias,     1.0f);
	m_texScaleBiasMat = texScaleBiasMat;
}

RenderShadowMapSample::~RenderShadowMapSample(void)
{
}

void RenderShadowMapSample::updateLightView()
{
	//-- 更新灯光的位置
	float t = timeGetTime()/4000.0f;
	float r = 200;
	m_lightPos.x = sinf(t) * r;
	m_lightPos.z = cosf(t) * r;
	m_lightPos.y = 200;

	//-- 计算view矩阵
	D3DXVECTOR3 zero(0,0,0);
	D3DXVECTOR3 up(0,1,0);

	D3DXMatrixLookAtLH(&m_lightView, &m_lightPos, &zero, &up);
}

void RenderShadowMapSample::setupLightParam(ID3DXEffect *pEffect)
{
	HRESULT hr = pEffect->SetFloatArray("g_lgtPt",(float*)&m_lightPos,3);

	D3DXVECTOR3 lgtDir;
	D3DXVec3Normalize(&lgtDir, &m_lightPos);	
	lgtDir *= -1;
	pEffect->SetFloatArray("g_lgtDir",(float*)&lgtDir,3);

	float cosTheta = cosf(m_lgtFov/2);
	pEffect->SetFloat("g_cosTheta", cosTheta);
}