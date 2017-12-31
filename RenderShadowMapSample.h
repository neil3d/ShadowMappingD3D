/*----------------------------------------------------------------------
	class RenderShadowMapSample
	作者: yanliang, 2007年12月
------------------------------------------------------------------------*/
#pragma once
#include "rendersample.h"

class RenderScene;
/**
 * Shadow map演示的基类，负责处理灯光数据
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
