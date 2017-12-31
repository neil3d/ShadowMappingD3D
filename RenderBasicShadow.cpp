#include "RenderBasicShadow.h"
#include "CameraCtrl.h"
#include "DrawingUtil.h"
#include "RenderScene.h"

RenderBasicShadow::RenderBasicShadow(void)
{
	m_pEffect = NULL;
	m_pShadowMap = NULL;
	m_pDepthBuffer = NULL;
}

RenderBasicShadow::~RenderBasicShadow(void)
{
	destroy();
}

bool RenderBasicShadow::create(IDirect3DDevice9 *pD3DDevice)
{
	//-- create shadow map
	HRESULT hr = pD3DDevice->CreateTexture( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE,
		1, D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F,
		D3DPOOL_DEFAULT,
		&m_pShadowMap,
		NULL );
	if(FAILED(hr))
	{
		MessageBox(NULL, _T("创建Shadow map失败"),_T("Error"), MB_OK);
		return false;
	}

	hr = pD3DDevice->CreateDepthStencilSurface( SHADOW_MAP_SIZE,SHADOW_MAP_SIZE,
		D3DFMT_D24X8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pDepthBuffer,
		NULL );

	if(FAILED(hr))
	{
		MessageBox(NULL, _T("创建depth surface失败"),_T("Error"), MB_OK);
		return false;
	}

	//-- load effect
	m_pEffect = DrawingUtil::getInst()->loadEffect(pD3DDevice,_T("Basic.fx"));

	return m_pEffect != NULL;
}

void RenderBasicShadow::destroy()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pShadowMap);
	SAFE_RELEASE(m_pDepthBuffer);
}

void RenderBasicShadow::render(IDirect3DDevice9 *pD3DDevice, RenderScene* pScene)
{
	if(m_pEffect == NULL)
		return;

	//-- 更新灯光view
	updateLightView();

	//-- 设置Effect的灯光参数
	setupLightParam(m_pEffect);	

	//-- 生成Shadow map
	m_pEffect->SetTechnique("genShadowMap");
	{
		SetRenderTarget srt(pD3DDevice, m_pShadowMap, m_pDepthBuffer);
		drawSceneMesh(m_pEffect, pScene, 0);
	}

	//-- 渲染整个场景
	m_pEffect->SetTechnique("shdScene");
	m_pEffect->SetTexture("g_texShadowMap", m_pShadowMap);
	m_pEffect->SetMatrix("g_texScaleBiasMat",&m_texScaleBiasMat);

	drawSceneMesh(m_pEffect, pScene, 1);
	
	//-- 画出灯光的位置
	DrawingUtil::getInst()->drawLight(m_lightPos);

	DrawingUtil::getInst()->drawText(_T("Basic Shadow Mapping"), 2, 16);
}

void RenderBasicShadow::onScreenShoot()
{
	if(m_pShadowMap)
	{
		HRESULT hr = D3DXSaveTextureToFile(_T("shadowmap.dds"),D3DXIFF_DDS,m_pShadowMap, NULL);
	}
}

void RenderBasicShadow::preDrawMesh(RenderScene* pScene,int meshIndex, DWORD flag)
{
	if(flag == 0)// gen shadow map
	{
		//-- 设置effect参数
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		pScene->getMeshWorldMat(meshIndex, matWorld);

		D3DXMATRIX matWorldViewProj = matWorld * m_lightView * m_lightProject;

		m_pEffect->SetMatrix("g_lgtWorldViewPrj",&matWorldViewProj);
	}
	else
	{
		//-- 设置effect参数
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		pScene->getMeshWorldMat(meshIndex, matWorld);
		m_pEffect->SetMatrix("g_world",&matWorld);

		D3DXMATRIX matWorldViewProj = matWorld * g_camera.getViewMat() * g_camera.getProjectMat();
		m_pEffect->SetMatrix("g_worldViewPrj",&matWorldViewProj);		

		matWorldViewProj = matWorld * m_lightView * m_lightProject;
		m_pEffect->SetMatrix("g_lgtWorldViewPrj",&matWorldViewProj);
	}
}