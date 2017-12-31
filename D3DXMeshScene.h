/*----------------------------------------------------------------------
	class D3DXShapeScene
	作者: yanliang, 2007年12月
------------------------------------------------------------------------*/

#pragma once
#include "renderscene.h"

/**
 *	加载一个x文件，组成一个简单测试场景
 */
class D3DXMeshScene : public RenderScene
{
public:
	D3DXMeshScene(void);
	virtual ~D3DXMeshScene(void);

	virtual bool create(IDirect3DDevice9 *pD3DDevice);
	virtual void destroy();

	virtual int getNumMesh();
	virtual void getMeshWorldMat(int i, D3DXMATRIX& mat);
	virtual void drawMesh(int i);

private:
	ID3DXMesh* loadMeshFVF(IDirect3DDevice9 *pD3DDevice, const TCHAR* szFileName, DWORD fvf, vector<D3DXMATERIAL>& xmtlArray);
private:
	ID3DXMesh	*m_pMeshBox;
	D3DXMATRIX	m_matBox;

	DWORD		m_numSubMeshTank;
	ID3DXMesh	*m_pMeshTank;
	D3DXMATRIX	m_matTank;
};
