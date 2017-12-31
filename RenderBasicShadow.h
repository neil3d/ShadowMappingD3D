/*----------------------------------------------------------------------
	class RenderBasicShadow
	作者: yanliang, 2007年12月
------------------------------------------------------------------------*/

#pragma once
#include "RenderShadowMapSample.h"

/**
 * 实现基本的Shadow Mapping算法
 */
class RenderBasicShadow : public RenderShadowMapSample
{
public:
	RenderBasicShadow(void);
	virtual ~RenderBasicShadow(void);

	virtual bool create(IDirect3DDevice9 *pD3DDevice);
	virtual void destroy();
	virtual void render(IDirect3DDevice9 *pD3DDevice, RenderScene* pScene);
	virtual void onScreenShoot();

protected:
	virtual void preDrawMesh(RenderScene* pScene,int meshIndex, DWORD flag);
private:
	ID3DXEffect			*m_pEffect;
	IDirect3DTexture9	*m_pShadowMap;
	IDirect3DSurface9	*m_pDepthBuffer;
};
