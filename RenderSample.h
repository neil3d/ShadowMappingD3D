/*----------------------------------------------------------------------
	class RenderSample
	����: yanliang, 2007��12��
------------------------------------------------------------------------*/
#pragma once
#include "inc.h"

class RenderScene;

/**
 * ��Ⱦ���ԵĻ���
 */
class RenderSample
{
public:
	RenderSample(void);
	virtual ~RenderSample(void);

	virtual bool create(IDirect3DDevice9 *pD3DDevice) =0;
	virtual void destroy() =0;
	virtual void render(IDirect3DDevice9 *pD3DDevice, RenderScene* pScene) =0;
	virtual void onScreenShoot() = 0;

protected:
	void drawSceneMesh(ID3DXEffect *pEffect, RenderScene* pScene, DWORD flag);
	virtual void preDrawMesh(RenderScene* pScene,int meshIndex, DWORD flag) = 0;
};
