#include "D3DXShapeScene.h"

D3DXShapeScene::D3DXShapeScene(void)
{
	m_sphereRadius = 10;
	m_pMeshSphere = NULL;
	m_pMeshBox = NULL;
	
	D3DXMatrixIdentity(&m_matBox);	
}

D3DXShapeScene::~D3DXShapeScene(void)
{
	destroy();
}

bool D3DXShapeScene::create(IDirect3DDevice9 *pD3DDevice)
{
	HRESULT hr;

	hr = D3DXCreateBox(pD3DDevice,1000,10,1000,&m_pMeshBox,NULL);
	assert(SUCCEEDED(hr));

	D3DXMatrixTranslation(&m_matBox, 0, -5, 0);

	hr = D3DXCreateSphere(pD3DDevice,m_sphereRadius,32,32,
		&m_pMeshSphere,NULL);
	assert(SUCCEEDED(hr));

	return true;
}

void D3DXShapeScene::destroy()
{
	SAFE_RELEASE(m_pMeshSphere);
	SAFE_RELEASE(m_pMeshBox);
}

int D3DXShapeScene::getNumMesh()
{
	return 1+SPHERE_NUM*SPHERE_NUM*SPHERE_NUM;
}

void D3DXShapeScene::getMeshWorldMat(int i, D3DXMATRIX& mat)
{
	if(i > 0)
	{
		int sphereIndex = i-1;
		int sphereZ = sphereIndex/SPHERE_NUM/SPHERE_NUM;
		
		sphereIndex = sphereIndex%(SPHERE_NUM*SPHERE_NUM);
		int sphereX = sphereIndex%SPHERE_NUM;
		int sphereY = sphereIndex/SPHERE_NUM;
		

		float step = m_sphereRadius*3;

		D3DXMATRIX matSphere;
		D3DXMatrixIdentity(&matSphere);	
		D3DXMatrixTranslation(&matSphere, sphereX*step, sphereY*step+step, sphereZ*step);
		
		mat =  matSphere;
	}
	else
	{
		mat =  m_matBox;
	}
}

void D3DXShapeScene::drawMesh(int i)
{
	if(i > 0)
	{
		if(m_pMeshSphere)
			m_pMeshSphere->DrawSubset(0);
	}
	else
	{
		if(m_pMeshBox)
			m_pMeshBox->DrawSubset(0);
	}
}