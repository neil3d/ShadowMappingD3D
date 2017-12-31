/*----------------------------------------------------------------------
	class CameraCtrl
	����: yanliang, 2007��12��
------------------------------------------------------------------------*/
#pragma once
#include "inc.h"

/**
 * ʵ��ʹ�����,���̿��������
*/
class CameraCtrl
{
public:
	CameraCtrl(void);
	~CameraCtrl(void);

	/**
	 * ����͸�Ӿ���
	 */
	void setPerspective(float fov, int clientW, int clientH, float zNear, float zFar);

	/**
	 * ���Ӿ�����ΪĬ��ֵ
	 */
	void setDefaultView();
	
	//-- ȡ���ڲ�����
	const D3DXMATRIX& getProjectMat()	{	return m_matPrj;}
	const D3DXMATRIX& getViewMat()		{	return m_matView;}
	const D3DXVECTOR3& getEyePos()		{	return m_eyePos;}
	float getZNear()					{return m_zNear;}
	float getZFar()						{return m_zFar;}

	//-- �û������ӿ�
	void onKeyDown(DWORD vk);
	void onLButtonDown(int x,int y);
	void onLbuttonUp();
	void onMouseMove(int x,int y);
	void onMouseWheel(int delta);
private:
	void updateViewMat();
private:
	D3DXMATRIX m_matPrj;
	D3DXMATRIX m_matView;

	D3DXVECTOR3		m_eyePos;
	D3DXVECTOR3		m_lookAt;
	float			m_dist;
	D3DXQUATERNION	m_rotate;
	D3DXQUATERNION	m_lastRotate;

	float			m_zNear;
	float			m_zFar;

	//--
	bool	m_bDrag;
	POINT	m_lastDragPt;
};

extern CameraCtrl g_camera;