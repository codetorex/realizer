
#ifndef VRENDERERDX9_H
#define VRENDERERDX9_H

#include "tstring.h"
#include "tbitmap.h"
#include "cwindows.h" // this renderer is windows only


// Clear Bit Definitions
#define RL_COLOR_BUFFER		D3DCLEAR_TARGET
#define RL_ZBUFFER			D3DCLEAR_ZBUFFER

#define RL_SRCALPHA			D3DBLEND_SRCALPHA
#define RL_INVSRCALPHA		D3DBLEND_INVSRCALPHA

#define RL_CUBE_PX			0	// RIGHT
#define RL_CUBE_NX			1	// LEFT
#define RL_CUBE_PY			2	// TOP
#define RL_CUBE_NY			3	// BOTTOM
#define RL_CUBE_PZ			4	// FRONT
#define RL_CUBE_NZ			5	// BACK

#define RL_CULL_CCW			D3DCULL_CCW
#define RL_CULL_CW			D3DCULL_CW
#define RL_CULL_NONE		D3DCULL_NONE

#define	RL_MAGFILTER		D3DSAMP_MAGFILTER
#define RL_MINFILTER		D3DSAMP_MINFILTER
#define RL_MIPFILTER		D3DSAMP_MIPFILTER

#define RL_DISABLEFILTER	D3DTEXF_NONE	// just for mipmap			
#define RL_POINTFILTER		D3DTEXF_POINT
#define RL_LINEARFILTER		D3DTEXF_LINEAR
#define RL_ANISOTROPIC		D3DTEXF_ANISOTROPIC

typedef void* RendererTexture; // renderer texture type
typedef RendererTexture rtex; // note: rtex must be defined in every renderer!

typedef void* RenderTargetObject;
typedef RenderTargetObject rrto;

typedef void* RendererFont; // renderer font typedef
typedef RendererFont rfnt;  // must defined in every renderer!

typedef void* RendererVertexBuffer;
typedef RendererVertexBuffer rvbf;  // must defined in every renderer!

class VVertexBuffer;

class RDLL VRendererDX9: public CWin32RenderWindow
{
public:
	LPDIRECT3DDEVICE9	D3DDevice; // D3D_Device
	LPDIRECT3D9			D3D; // D3D_Object

private:
	bool	InitializeDirect3D();
	void	DestroyDirect3D();
	D3DFORMAT BitmapToD3DFormat(dword bmpFormat);

public:

	static void InitializeFormats();
	bool InitializeRenderer(int _width,int _height,ch16* _title,bool fullscr,int bits);

	inline void Clear(dword flags, dword color)
	{
		D3DDevice->Clear(0, NULL,flags,color,1.0f,0);
	}

	inline void Clear(dword flags)
	{
		Clear(flags,0);
	}

	inline void BeginScene()
	{
		D3DDevice->BeginScene();
	}

	inline void EndScene()
	{
		D3DDevice->EndScene();
	}

	inline void SwapBackBuffer()
	{
		D3DDevice->Present(NULL, NULL, NULL, NULL);
	}

	inline rtex LoadTextureFromFile(ch8* filepath)
	{
		LPDIRECT3DTEXTURE9 rt;
		D3DXCreateTextureFromFileA(D3DDevice,filepath,&rt);
		return (void*)rt;
	}

	inline void DeleteTexture(rtex txtr)
	{
		((LPDIRECT3DTEXTURE9)txtr)->Release();
	}

	void CreateVertexBuffer(VVertexBuffer* buffer, int capacity);
	void DeleteVertexBuffer(VVertexBuffer* buffer);
	void LockVertexBuffer(VVertexBuffer* buffer, int offset, int length);
	void UnlockVertexBuffer(VVertexBuffer* buffer);

	rtex LoadTextureFromBitmap(TBitmap* bmp, bool automipmap = true);
	void UpdateTextureFromBitmap(rtex tx, TBitmap* bmp);
	
// Functions that shouldn't be needed to implemented by other drivers
private:
	void LoadSurfaceFromBitmap( void* surf, TBitmap* bmp);
};

#endif