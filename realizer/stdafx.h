// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef WIN32

#include <SDKDDKVer.h>

#define USE_DIRECTX9

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#include <winsock2.h>
#include <mswsock.h>
#include <Ws2tcpip.h>



#ifdef USE_DIRECTX9
//d3dx9.lib d3d9.lib winmm.lib
#if DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"winmm.lib")
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>

#endif

#endif


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

// TODO: reference additional headers your program requires here
