#include "RenderHardwareShadow.h"
#include "DrawingUtil.h"
#include "CameraCtrl.h"
#include "RenderScene.h"

RenderHardwareShadow::RenderHardwareShadow(void)
{
	m_pShadowMap = NULL;
	m_pEffect = NULL;
	m_pColorSurface = NULL;
}

RenderHardwareShadow::~RenderHardwareShadow(void)
{
	destroy();
}

bool RenderHardwareShadow::create(IDirect3DDevice9 *pD3DDevice)
{
	HRESULT hr;

	//-- create shadow map
	hr = pD3DDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 
		1, D3DUSAGE_DEPTHSTENCIL, D3DFMT_D24X8, 
        D3DPOOL_DEFAULT, &m_pShadowMap, NULL);
	if(FAILED(hr))
		return false;

	hr = pD3DDevice->CreateRenderTarget(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE,
		D3DFMT_X8R8G8B8,D3DMULTISAMPLE_NONE,0, FALSE,
		&m_pColorSurface, NULL);

	if(FAILED(hr))
		return false;

	//-- load effect
	m_pEffect = DrawingUtil::getInst()->loadEffect(pD3DDevice,_T("Hardware.fx"));

	return m_pEffect!=NULL;
}

void RenderHardwareShadow::destroy()
{
	SAFE_RELEASE(m_pShadowMap);
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pColorSurface);
}

class SetShadowMapZ
{
	IDirect3DSurface9	*m_pLastRT;
	IDirect3DSurface9	*m_pLastDepth;
	IDirect3DDevice9	*m_pD3DDevice;

public:
	SetShadowMapZ(IDirect3DDevice9 *pD3DDevice, IDirect3DTexture9* pDepth, IDirect3DSurface9* pColor)
	{
		m_pD3DDevice = pD3DDevice;

		HRESULT hr;
		hr = pD3DDevice->GetRenderTarget(0, &m_pLastRT);
		if(FAILED(hr))
			return;
		hr = pD3DDevice->GetDepthStencilSurface(&m_pLastDepth);
		if(FAILED(hr))
			return;

		IDirect3DSurface9 *pSurface = NULL;
		hr = pDepth->GetSurfaceLevel(0, &pSurface );
		if(FAILED(hr))
			return;

		hr = pD3DDevice->SetDepthStencilSurface(pSurface);
		hr = pSurface->Release();

		hr = pD3DDevice->SetRenderTarget(0, pColor);

		hr = pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1, 0);
	}

	~SetShadowMapZ()
	{
		HRESULT hr;
		hr = m_pD3DDevice->SetRenderTarget(0, m_pLastRT);		
		hr = m_pD3DDevice->SetDepthStencilSurface(m_pLastDepth);
	}
};

void RenderHardwareShadow::render(IDirect3DDevice9 *pD3DDevice, RenderScene* pScene)
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
		SetShadowMapZ srt(pD3DDevice, m_pShadowMap, m_pColorSurface);
		drawSceneMesh(m_pEffect, pScene, 0);
	}

	//-- 渲染整个场景
	m_pEffect->SetTechnique("shdScene");
	m_pEffect->SetTexture("g_texShadowMap", m_pShadowMap);
	m_pEffect->SetMatrix("g_texScaleBiasMat",&m_texScaleBiasMat);

	drawSceneMesh(m_pEffect, pScene, 1);
	
	//-- 画出灯光的位置
	DrawingUtil::getInst()->drawLight(m_lightPos);

	DrawingUtil::getInst()->drawText(_T("Hardware Shadow Mapping"), 2, 16);
}

void RenderHardwareShadow::onScreenShoot()
{
	HRESULT hr ;
	if(m_pShadowMap)
	{
		hr = D3DXSaveTextureToFile(_T("shadowmap.dds"),D3DXIFF_DDS,m_pShadowMap, NULL);
	}
}

void RenderHardwareShadow::preDrawMesh(RenderScene* pScene,int meshIndex, DWORD flag)
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
		m_pEffect->SetMatrix("g_worldViewPrj", &matWorldViewProj);		

		matWorldViewProj = matWorld * m_lightView * m_lightProject;
		m_pEffect->SetMatrix("g_lgtWorldViewPrj", &matWorldViewProj);
	}
}