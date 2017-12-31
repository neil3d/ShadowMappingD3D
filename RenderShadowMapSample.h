/*----------------------------------------------------------------------
	class RenderShadowMapSample
	����: yanliang, 2007��12��
------------------------------------------------------------------------*/
#pragma once
#include "rendersample.h"

class RenderScene;
/**
 * Shadow map��ʾ�Ļ��࣬������ƹ�����
 */
class RenderShadowMapSample : public RenderSample
{
protected:
	enum EConst
	{
		SHADOW_MAP_SIZE = 2048,
	};
public:
	RenderShadowMapSample(void);
	virtual ~RenderShadowMapSample(void);

protected:
	void updateLightView();
	void setupLightParam(ID3DXEffect *pEffect);

protected:
	float				m_lgtFov;
	D3DXVECTOR3			m_lightPos;
	D3DXMATRIX			m_lightProject;
	D3DXMATRIX			m_lightView;

	D3DXMATRIX			m_texScaleBiasMat;
};
