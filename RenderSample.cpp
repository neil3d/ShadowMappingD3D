#include "RenderSample.h"
#include "RenderScene.h"

RenderSample::RenderSample(void)
{
}

RenderSample::~RenderSample(void)
{
}

void RenderSample::drawSceneMesh(ID3DXEffect *pEffect, RenderScene* pScene, DWORD flag)
{
	for(int meshIndex=0; meshIndex<pScene->getNumMesh(); meshIndex++)
	{
		preDrawMesh(pScene, meshIndex, flag);

		HRESULT hr;
		UINT numPass = 0;

		hr = pEffect->Begin(&numPass,0);
		for(UINT i=0; i<numPass; i++)
		{
			pEffect->BeginPass(i);
			pScene->drawMesh(meshIndex);
			pEffect->EndPass();
		}
		hr = pEffect->End();
	}
}