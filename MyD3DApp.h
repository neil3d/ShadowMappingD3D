/*----------------------------------------------------------------------
	class MyD3DApp
	����: yanliang, 2007��12��
------------------------------------------------------------------------*/
#pragma once
#include "inc.h"

class RenderSample;
class RenderScene;

enum ESample
{
	ESample_Basic,
	ESample_Hardware,
};

enum EScene
{
	EScene_D3DXShape,
	EScene_D3DXMesh,
};

/**
 * һ���򵥵ķ�װ��,ʵ��Win32���ڴ���,D3D Device������
*/
class MyD3DApp
{
public:
	MyD3DApp(void);
	~MyD3DApp(void);

	/**
	 * �������ڼ�D3D
	 * @param hInstance ��ǰ�����instance���
	 * @param width ���ڵĿ�
	 * @param height ���ڵĸ�
	 * @param szWindowTitle ���ڱ���
	 * @return �ɹ��򷵻�true
	*/
	bool create(HINSTANCE hInstance, int width, int height, const TCHAR* szWindowTitle);
	/**
	 * ����
	*/
	void destroy();

	/**
	 * ��Ⱦ, Ӧ����ѭ����ÿ֡����һ��
	*/
	void render();
	void requestScreenShot()	{	m_requestScreenShoot=true;}
	

	void openSample(ESample s);
	void openScene(EScene s);

	IDirect3DDevice9* getD3DDevice()	{	return m_pD3DDevice; }
private:
	bool createD3D();
	void destroyD3D();
	void calFPS();
	void screenShoot();

private:
	/** ������HANDLE */
	HWND				m_hWnd;
	/** D3D Object */
	IDirect3D9			*m_pD3D;
	/** D3D Device Object */
	IDirect3DDevice9	*m_pD3DDevice;
	/** ֡���� */
	float				m_FPS;

	RenderSample		*m_pSample;
	RenderScene			*m_pScene;
	bool				m_requestScreenShoot;
};
