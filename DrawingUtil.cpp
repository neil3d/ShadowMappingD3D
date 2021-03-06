#include ".\drawingutil.h"
#include "CameraCtrl.h"

DrawingUtil::DrawingUtil(void)
{
	m_pLightMesh = NULL;
	m_pSimpleEffect = NULL;
	m_pD3DFont = NULL;
}

DrawingUtil::~DrawingUtil(void)
{
	destroy();
}

DrawingUtil* DrawingUtil::getInst()
{
	static DrawingUtil g_drawingUtil;
	return &g_drawingUtil;
}

void DrawingUtil::destroy()
{
	SAFE_RELEASE(m_pLightMesh);
	SAFE_RELEASE(m_pSimpleEffect);
	SAFE_RELEASE(m_pD3DFont);
}

void DrawingUtil::init(IDirect3DDevice9 *pD3DDevice)
{
	HRESULT hr;
	//-- 字体
	D3DXFONT_DESC font;
	memset(&font,0,sizeof(font));
	font.Height = 14;
	font.Width = 0;
	font.Weight = 500;
	font.MipLevels = D3DX_DEFAULT;
	font.Italic=FALSE;
	font.CharSet = DEFAULT_CHARSET;
	font.OutputPrecision = OUT_DEFAULT_PRECIS;
	font.Quality = DEFAULT_QUALITY;
	font.PitchAndFamily = DEFAULT_PITCH;
	_tcscpy( font.FaceName, _T("宋体") );

	hr=D3DXCreateFontIndirect(pD3DDevice,
		&font,
		&m_pD3DFont);
	assert(SUCCEEDED(hr));

	//-- 画出灯光位置需要的东西
	hr = D3DXCreateBox(pD3DDevice,10,10,10,&m_pLightMesh,NULL);
	assert(SUCCEEDED(hr));

	m_pSimpleEffect  = loadEffect(pD3DDevice,_T("SimpleDraw.fx"));
	assert(m_pSimpleEffect);
}

void DrawingUtil::drawLight(D3DXVECTOR3 pos)
{
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld,pos.x,pos.y,pos.z);

	D3DXMATRIX matWorldViewProj = matWorld * g_camera.getViewMat() * g_camera.getProjectMat();
	m_pSimpleEffect->SetMatrix("matWorldViewPrj",&matWorldViewProj);

	UINT numPass = 0;
	m_pSimpleEffect->Begin(&numPass,0);
	m_pSimpleEffect->BeginPass(0);
	m_pLightMesh->DrawSubset(0);
	m_pSimpleEffect->EndPass();
	m_pSimpleEffect->End();
}

void DrawingUtil::drawText(const TCHAR* szText,int x, int y, D3DCOLOR color)
{
	RECT rc;
	rc.left = x;
	rc.top = y;
	rc.right = 640;
	rc.bottom = 480;
	
	int len = (int)_tcslen(szText);

	m_pD3DFont->DrawText(NULL,szText,len,&rc,DT_TOP|DT_LEFT,color);
}

ID3DXEffect *DrawingUtil::loadEffect(IDirect3DDevice9 *pD3DDevice,const TCHAR* szFileName)
{
	HRESULT hr;

	ID3DXEffect *pEffect = NULL;

	DWORD dwFlag = 0;

	ID3DXBuffer *pError = NULL;
	hr = D3DXCreateEffectFromFile(pD3DDevice,szFileName,
		NULL,NULL,
		dwFlag,
		NULL,
		&pEffect,&pError);
	if(FAILED(hr))
	{
		if(pError)
		{
			const TCHAR* szError = (const TCHAR*)pError->GetBufferPointer();
			::MessageBox(NULL,szError,_T("Error"),MB_OK);
		}
	}

	return pEffect;
}