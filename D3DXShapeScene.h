/*----------------------------------------------------------------------
	class D3DXShapeScene
	作者: yanliang, 2007年12月
------------------------------------------------------------------------*/

#pragma once
#include "renderscene.h"

/**
 * 使用D3DX函数创建一些几何形体的简单场景
 */
class D3DXShapeScene : public RenderScene
{
	enum EConst
	{
		SPHERE_NUM = 3
	};
public:
	D3DXShapeScene(void);
	virtual ~D3DXShapeScene(void);

	virtual bool create(IDirect3DDevice9 *pD3DDevice);
	virtual void destroy();

	virtual int getNumMesh();
	virtual void getMeshWorldMat(int i, D3DXMATRIX& mat);
	virtual void drawMesh(int i);

private:
	ID3DXMesh	*m_pMeshSphere;
	ID3DXMesh	*m_pMeshBox;
	
	D3DXMATRIX	m_matBox;
	float		m_sphereRadius;
};
