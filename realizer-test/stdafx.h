#pragma once

#ifdef WIN32

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#else
#define LINUX
#include <pthread.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

