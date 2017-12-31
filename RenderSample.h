/*----------------------------------------------------------------------
	class RenderSample
	作者: yanliang, 2007年12月
------------------------------------------------------------------------*/
#pragma once
#include "inc.h"

class RenderScene;

/**
 * 渲染策略的基类
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
