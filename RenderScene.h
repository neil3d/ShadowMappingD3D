/*----------------------------------------------------------------------
	class RenderScene
	����: yanliang, 2007��12��
------------------------------------------------------------------------*/

#pragma once
#include "inc.h"

/**
 * ������ʾ�õĳ���ģ�ͼ�������
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
