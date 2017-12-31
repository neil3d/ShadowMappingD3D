
#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <Mmsystem.h>
#pragma comment(lib,"Winmm.lib")

#include <stdio.h>
#include <assert.h>
#include <tchar.h>

#pragma warning(disable:4819)
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9effect.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//--
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//--
const int clientWidth = 800;
const int clientHeight = 600;

#include <vector>
using namespace std;