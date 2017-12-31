/*----------------------------------------------------------------------
	class DrawingUtil
	作者: yanliang, 2007年12月
------------------------------------------------------------------------*/

#pragma once

#include "inc.h"

/**
 * 一些简单功能函数的封装
 */
class DrawingUtil
{
public:
	DrawingUtil(void);
	~DrawingUtil(void);

	static DrawingUtil* getInst();
	void init(IDirect3DDevice9 *pD3DDevice);
	void destroy();

	ID3DXEffect *loadEffect(IDirect3DDevice9 *pD3DDevice,const TCHAR* szFileName);

	void drawLight(D3DXVECTOR3 pos);
	void drawText(const TCHAR* szText,int x, int y, D3DCOLOR color = 0xFFFFFFFF);
private:
	ID3DXFont	*m_pD3DFont;
	ID3DXMesh	*m_pLightMesh;
	ID3DXEffect	*m_pSimpleEffect;
};

/**
 * SetRenderTarget
 */
class SetRenderTarget
{
	IDirect3DSurface9	*m_pLastRT;
	IDirect3DSurface9	*m_pLastDepth;
	IDirect3DDevice9	*m_pD3DDevice;
public:
	SetRenderTarget(IDirect3DDevice9 *pD3DDevice, IDirect3DTexture9* pTex, IDirect3DSurface9* pDepth)
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
		hr = pTex->GetSurfaceLevel(0, &pSurface );
		if(FAILED(hr))
			return;

		hr = pD3DDevice->SetRenderTarget(0, pSurface);
		hr = pSurface->Release();

		DWORD clearFlag = D3DCLEAR_TARGET;
		if(pDepth)
		{
			hr = pD3DDevice->SetDepthStencilSurface(pDepth);
			clearFlag |= D3DCLEAR_ZBUFFER;
		}

		hr = pD3DDevice->Clear(0, NULL, clearFlag, 0, 1, 0);
		
	}

	~SetRenderTarget()
	{
		HRESULT hr;
		hr = m_pD3DDevice->SetRenderTarget(0, m_pLastRT);
		if(m_pLastDepth)
			hr = m_pD3DDevice->SetDepthStencilSurface(m_pLastDepth);
	}

};