#include "resource.h"
#include "MyD3DApp.h"
#include "CameraCtrl.h"
#include "RenderSample.h"
#include "RenderScene.h"
#include "DrawingUtil.h"

#include "D3DXShapeScene.h"
#include "D3DXMeshScene.h"
#include "RenderBasicShadow.h"
#include "RenderHardwareShadow.h"

//----------------------------------------------------------------------------------------
MyD3DApp g_app;

int WINAPI WinMain( HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR     lpCmdLine,
				   int       nCmdShow )
{
	//--创建
	if(!g_app.create(hInstance,clientWidth,clientHeight,_T("Shadow Map Samples")))
		return -1;

	//--默认camera参数
	g_camera.setPerspective(45.0f,clientWidth,clientHeight,10,5000);

	DrawingUtil::getInst()->init(g_app.getD3DDevice());

	//-- open default content
	g_app.openSample(ESample_Basic);
	g_app.openScene(EScene_D3DXShape);

	//--消息循环
	MSG        uMsg;
	memset(&uMsg,0,sizeof(uMsg));

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else
			g_app.render();
	}


	//--销毁
	DrawingUtil::getInst()->destroy();
	g_app.destroy();
	return 0;
}

//--消息处理函数
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							UINT   msg, 
							WPARAM wParam, 
							LPARAM lParam )
{
	switch( msg )
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_EXIT:
			PostQuitMessage(0);
			break;
		//-- 响应菜单，切换不同的测试场景
		case ID_SCENE_SPHERES:
			g_app.openScene(EScene_D3DXShape);
			break;
		case ID_SCENE_TANK:
			g_app.openScene(EScene_D3DXMesh);
			break;
		
		//-- 响应菜单，切换不通的渲染算法
		case ID_SHADOWSAMPLE_BASIC:
			g_app.openSample(ESample_Basic);
			break;
		case ID_SHADOWSAMPLE_HARDWARE:
			g_app.openSample(ESample_Hardware);
			break;		
		
		//--
		case ID_VIEW_RESETCAMERA:
			g_camera.setDefaultView();
			break;
		case ID_USAGE:
			MessageBox(hWnd,
				_T("旋转摄像机：\t鼠标左键拖动\r\n移动摄像机：\tW,A,S,D,Z,X\r\n屏幕截图：\tF1\n\n\t\tby Yanliang, Dec 2007"),
				_T("Usage"), MB_OK);
			break;
		}
		break;
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_F1:
				g_app.requestScreenShot();
				break;
			default:				
				g_camera.onKeyDown((DWORD)wParam);
				break;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		g_camera.onLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); 
		break;
	case WM_LBUTTONUP:
		g_camera.onLbuttonUp();
		break;
	case WM_MOUSEMOVE:
		g_camera.onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEWHEEL:
		g_camera.onMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_CLOSE:
		{
			PostQuitMessage(0);
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}



//-- class MyD3DApp ---------------------------------------------------------------------------
MyD3DApp::MyD3DApp(void)
{
	m_hWnd = NULL;

	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	m_FPS = 0;

	m_pSample = NULL;
	m_pScene = NULL;

	m_requestScreenShoot = false;
}

MyD3DApp::~MyD3DApp(void)
{
}

bool MyD3DApp::create(HINSTANCE hInstance, int width, int height, const TCHAR* szWindowTitle)
{
	//--注册窗口类
	WNDCLASSEX winClass;

	winClass.lpszClassName = _T("MyD3DAppWndClass");
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = NULL;
	winClass.hIconSm	   = NULL;
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return false;

	//-- 计算窗口位置
	int scrW=::GetSystemMetrics(SM_CXSCREEN);
	int scrH=::GetSystemMetrics(SM_CYSCREEN);

	RECT wndRc;
	wndRc.left = (scrW-width)/2;
	wndRc.top = (scrH-height)/2;
	wndRc.right = wndRc.left + width;
	wndRc.bottom = wndRc.top + height;


	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	AdjustWindowRect(&wndRc,style,TRUE);

	//-- 创建窗口
	m_hWnd = CreateWindowEx( NULL, _T("MyD3DAppWndClass"), 
		szWindowTitle,
		style,
		wndRc.left, wndRc.top, wndRc.right-wndRc.left, wndRc.bottom-wndRc.top,
		NULL, NULL, hInstance, NULL );

	if( m_hWnd == NULL )
		return false;

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	//-- 创建D3D对象
	if(!createD3D())
		return false;

	return true;
}

void MyD3DApp::destroy()
{
	destroyD3D();

	SAFE_DELETE(m_pSample);
	SAFE_DELETE(m_pScene);
}


void MyD3DApp::render()
{
	HRESULT hr;

	hr = m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_COLORVALUE(0.8f,0.8f,0.8f,1.0f), 1.0f, 0 );

	hr = m_pD3DDevice->BeginScene();

	if(SUCCEEDED(hr))
	{
		if(m_pSample && m_pScene)
			m_pSample->render(m_pD3DDevice, m_pScene);

		//-- show fps
		calFPS();
		TCHAR szText[512] = {0};
		_stprintf(szText,_T("FPS:%.2f"),m_FPS);
		DrawingUtil::getInst()->drawText(szText,2,2);

		//--
		m_pD3DDevice->EndScene();
		
		if(m_requestScreenShoot)
		{
			screenShoot();
			m_requestScreenShoot = false;
		}

		m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
	}
}

bool MyD3DApp::createD3D()
{
	HRESULT hr;

	//--创建d3d对象
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if(m_pD3D == NULL)
	{
		::MessageBox(m_hWnd,_T("D3D Create failed."),_T("Error"),MB_OK);
		return false;
	}

	//--测试shader版本
	D3DCAPS9 caps;
	hr = m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	if(caps.VertexShaderVersion < D3DVS_VERSION(2,0)
		|| caps.PixelShaderVersion < D3DPS_VERSION(2,0))
	{
		::MessageBox(m_hWnd,_T("显卡不支持Shader 2.0"),_T("Error"),MB_OK);
		return false;
	}

	//--创建device
	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));

	d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed               = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_DEFAULT;

	DWORD dwBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	D3DDEVTYPE devType = D3DDEVTYPE_HAL;

	hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, 
		devType, 
		m_hWnd,
		dwBehaviorFlags, &d3dpp, &m_pD3DDevice );


	return SUCCEEDED(hr);
}

void MyD3DApp::destroyD3D()
{
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pD3DDevice);
}

void MyD3DApp::calFPS()
{
	float deltaTime;
	static DWORD lastTime = timeGetTime();
	deltaTime = (timeGetTime()-lastTime)*0.001f;
	lastTime = timeGetTime();

	static float c = 0;
	static float sum = 0;
	if(c < 10)
	{
		c += 1.0f;
		sum += deltaTime;
	}
	else
	{
		m_FPS = c/sum;

		c=0;
		sum=0;
	}
}

void MyD3DApp::openSample(ESample s)
{
	//-- destroy old
	if(m_pSample)
	{
		m_pSample->destroy();
		SAFE_DELETE(m_pSample);
	}

	//-- create new one
	switch(s)
	{
	case ESample_Basic:
		m_pSample = new RenderBasicShadow;
		break;
	case ESample_Hardware:
		m_pSample = new RenderHardwareShadow;
		break;
	}

	if(m_pSample)
	{
		if(!m_pSample->create(m_pD3DDevice))
		{
			SAFE_DELETE(m_pSample);
		}
	}
}

void MyD3DApp::openScene(EScene s)
{
	//-- destroy old
	if(m_pScene)
	{
		m_pScene->destroy();
		SAFE_DELETE(m_pScene);
	}

	//-- create new one
	switch(s)
	{
	case EScene_D3DXShape:
		m_pScene = new D3DXShapeScene;
		break;
	case EScene_D3DXMesh:
		m_pScene = new D3DXMeshScene;
		break;
	}

	if(m_pScene)
	{
		if(!m_pScene->create(m_pD3DDevice))
		{
			SAFE_DELETE(m_pScene);
		}
	}
}

void MyD3DApp::screenShoot()
{
	if(m_pSample)
		m_pSample->onScreenShoot();

	if(m_pD3DDevice == NULL)
		return;

	IDirect3DSurface9* pRenderTarget=NULL;
	HRESULT hr = m_pD3DDevice->GetRenderTarget(0,&pRenderTarget);
	if(SUCCEEDED(hr))
	{
		hr = D3DXSaveSurfaceToFile(_T("screen.bmp"),D3DXIFF_BMP,pRenderTarget, NULL, NULL);
		pRenderTarget->Release();
	}
}