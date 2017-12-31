/*----------------------------------------------------------------------
	class RenderHardwareShadow
	����: yanliang, 2007��12��
------------------------------------------------------------------------*/
#pragma once
#include "RenderShadowMapSample.h"

/**
 * ʹ��Ӳ��֧�ֵ�Shadow Map
 */
class RenderHardwareShadow : public RenderShadowMapSample
{
public:
	RenderHardwareShadow(void);
	virtual ~RenderHardwareShadow(void);

	virtual bool create(IDirect3DDevice9 *pD3DDevice);
	virtual void destroy();
	virtual void render(IDirect3DDevice9 *pD3DDevice, RenderScene* pScene);
	virtual void onScreenShoot();

protected:
	virtual void preDrawMesh(RenderScene* pScene,int meshIndex, DWORD flag);

private:
	ID3DXEffect			*m_pEffect;
	IDirect3DTexture9	*m_pShadowMap;
	IDirect3DSurface9	*m_pColorSurface;

};
