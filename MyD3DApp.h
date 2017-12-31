/*----------------------------------------------------------------------
	class MyD3DApp
	作者: yanliang, 2007年12月
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
 * 一个简单的封装类,实现Win32窗口创建,D3D Device创建等
*/
class MyD3DApp
{
public:
	MyD3DApp(void);
	~MyD3DApp(void);

	/**
	 * 创建窗口及D3D
	 * @param hInstance 当前程序的instance句柄
	 * @param width 窗口的宽
	 * @param height 窗口的高
	 * @param szWindowTitle 窗口标题
	 * @return 成功则返回true
	*/
	bool create(HINSTANCE hInstance, int width, int height, const TCHAR* szWindowTitle);
	/**
	 * 销毁
	*/
	void destroy();

	/**
	 * 渲染, 应在主循环中每帧调用一次
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
	/** 主窗口HANDLE */
	HWND				m_hWnd;
	/** D3D Object */
	IDirect3D9			*m_pD3D;
	/** D3D Device Object */
	IDirect3DDevice9	*m_pD3DDevice;
	/** 帧速率 */
	float				m_FPS;

	RenderSample		*m_pSample;
	RenderScene			*m_pScene;
	bool				m_requestScreenShoot;
};
