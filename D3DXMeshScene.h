/*----------------------------------------------------------------------
	class D3DXShapeScene
	����: yanliang, 2007��12��
------------------------------------------------------------------------*/

#pragma once
#include "renderscene.h"

/**
 *	����һ��x�ļ������һ���򵥲��Գ���
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
