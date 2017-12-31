/*----------------------------------------------------------------------
	class RenderScene
	作者: yanliang, 2007年12月
------------------------------------------------------------------------*/

#pragma once
#include "inc.h"

/**
 * 管理演示用的场景模型几何数据
 */
class RenderScene
{
public:
	RenderScene(void);
	virtual ~RenderScene(void);

	virtual bool create(IDirect3DDevice9 *pD3DDevice) =0;
	virtual void destroy() =0;

	virtual int getNumMesh() =0;
	virtual void getMeshWorldMat(int i, D3DXMATRIX& mat) =0;
	virtual void drawMesh(int i) =0;
};
