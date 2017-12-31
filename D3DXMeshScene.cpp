#include "D3DXMeshScene.h"
#include "inc.h"

D3DXMeshScene::D3DXMeshScene(void)
{
	m_pMeshBox = NULL;
	D3DXMatrixIdentity(&m_matBox);	

	m_numSubMeshTank = 0;
	m_pMeshTank = NULL;
	D3DXMatrixIdentity(&m_matTank);	
}

D3DXMeshScene::~D3DXMeshScene(void)
{
	destroy();
}

bool D3DXMeshScene::create(IDirect3DDevice9 *pD3DDevice)
{
	vector<D3DXMATERIAL> xmtlArray;
	m_pMeshTank = loadMeshFVF(pD3DDevice, _T("Media\\Tank.x"), D3DFVF_XYZ|D3DFVF_NORMAL,xmtlArray);
	m_numSubMeshTank = (DWORD)xmtlArray.size();

	if(m_pMeshTank == NULL)
		return false;

	HRESULT hr;
	hr = D3DXCreateBox(pD3DDevice,1000,10,1000,&m_pMeshBox,NULL);
	assert(SUCCEEDED(hr));

	D3DXMatrixTranslation(&m_matBox, 0, -5, 0);

	return true;
}

void D3DXMeshScene::destroy()
{
	SAFE_RELEASE(m_pMeshBox);
	SAFE_RELEASE(m_pMeshTank);
}

int D3DXMeshScene::getNumMesh()
{
	return 2;
}

void D3DXMeshScene::getMeshWorldMat(int i, D3DXMATRIX& mat)
{
	if(i == 0)
		mat = m_matBox;
	else
		mat = m_matTank;
}

void D3DXMeshScene::drawMesh(int i)
{
	if(i == 0)
	{
		if(m_pMeshBox)
			m_pMeshBox->DrawSubset(0);
	}
	else
	{
		for(DWORD j=0;j<m_numSubMeshTank;j++)
				m_pMeshTank->DrawSubset(j);
	}
}

ID3DXMesh* D3DXMeshScene::loadMeshFVF(IDirect3DDevice9 *pD3DDevice, const TCHAR* szFileName, DWORD fvf, vector<D3DXMATERIAL>& xmtlArray)
{
	xmtlArray.clear();
	
	//--
	ID3DXMesh* pRet = NULL;

	HRESULT hr;
	ID3DXBuffer *pAdjacency = NULL;
	ID3DXMesh *pMesh = NULL;
	ID3DXBuffer *pXMtlBuffer = NULL;
	DWORD numMtl = 0;

	hr = D3DXLoadMeshFromX(szFileName,
		D3DXMESH_SYSTEMMEM,
		pD3DDevice,
		&pAdjacency,&pXMtlBuffer,NULL,&numMtl,
		&pMesh);
	if(SUCCEEDED(hr))
	{
		//--copy d3dx mtl
		D3DXMATERIAL *pXMtl = (D3DXMATERIAL *)pXMtlBuffer->GetBufferPointer();
		for(DWORD i=0;i<numMtl;i++)
		{
			xmtlArray.push_back(pXMtl[i]);
		}

		//--
		hr = pMesh->CloneMeshFVF(D3DXMESH_MANAGED, fvf, pD3DDevice, &pRet);
		if(SUCCEEDED(hr))
		{
			if(fvf & D3DFVF_NORMAL)
				hr = D3DXComputeNormals(pRet,(DWORD*)pAdjacency->GetBufferPointer());
		}

		pMesh->Release();
	}

	return pRet;
}