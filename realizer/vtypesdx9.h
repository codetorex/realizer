#ifndef VTYPESDX9_H
#define VTYPESDX9_H


#define RL_POINTLIST		D3DPT_POINTLIST
#define RL_LINELIST			D3DPT_LINELIST
#define RL_LINESTRIP		D3DPT_LINESTRIP
#define RL_TRIANGLELIST		D3DPT_TRIANGLELIST
#define RL_TRIANGLESTRIP	D3DPT_TRIANGLESTRIP
#define RL_TRIANGLEFAN		D3DPT_TRIANGLEFAN


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

typedef LPDIRECT3DTEXTURE9 RendererTexture; // renderer texture type
typedef RendererTexture rtex; // note: rtex must be defined in every renderer!

typedef void* RenderTargetObject;
typedef RenderTargetObject rrto;

typedef void* RendererFont; // renderer font typedef
typedef RendererFont rfnt;  // must defined in every renderer!

typedef LPDIRECT3DVERTEXBUFFER9 RendererVertexBuffer;
typedef RendererVertexBuffer rvbf;  // must defined in every renderer!

#endif