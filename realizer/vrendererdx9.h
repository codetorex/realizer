
#ifndef VRENDERERDX9_H
#define VRENDERERDX9_H

#include "tstring.h"
#include "tbitmap.h"
#include "cwindows.h" // this renderer is windows only
#include "vvertexstream.h"
#include "mmatrix.h"
#include "vmesh.h"
#include "vvertexbufferformat.h"

class RDLL VRendererDX9: public CRendererWindow
{
public:
	LPDIRECT3DDEVICE9	D3DDevice; // D3D_Device
	LPDIRECT3D9			D3D; // D3D_Object

private:
	bool	InitializeDirect3D();
	void	DestroyDirect3D();
	D3DFORMAT BitmapToD3DFormat(ui32 bmpFormat);

public:

	static void InitializeFormats();
	bool InitializeRenderer(int _width,int _height,const String& _title,bool fullscr,int bits);

	inline void Clear(ui32 flags, ui32 color)
	{
		D3DDevice->Clear(0, NULL,flags,color,1.0f,0);
	}

	inline void Clear(ui32 flags)
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
		return rt;
	}

	inline void DeleteTexture(rtex txtr)
	{
		txtr->Release();
	}

	inline void SetTexture(int stage,rtex txtr)
	{
		D3DDevice->SetTexture(stage,txtr);
	}

	inline void SetTextureFilter(int stage, int filterType, int value)
	{
		D3DDevice->SetSamplerState(stage,(D3DSAMPLERSTATETYPE)filterType,value);
	}

	/**
	 * DirectX has different texture coordinate than OpenGL so we can use this to conver them?
	 */
	inline void ProcessTextureCoordinate(Vector3& input)
	{
		input.y = 1.0f - input.y;
	}

	/*inline void SetTextureState(int stage, int stateType, int value)
	{

	}*/

	inline void EnableTexturing(int stage)
	{
		D3DDevice->SetTextureStageState(stage,D3DTSS_COLOROP, D3DTOP_MODULATE);
	}

	inline void EnableTextureAlphaVertexColorAlpha(int stage)
	{
		D3DDevice->SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		/*
		Basically
		Direct3D.TextureState(0).ColorOperation = Direct3D.TextureOperation.Modulate
		Direct3D.TextureState(0).ColorArgument1 = Direct3D.TextureArgument.TextureColor
		Direct3D.TextureState(0).ColorArgument2 = Direct3D.TextureArgument.Diffuse
		Direct3D.TextureState(0).AlphaOperation = Direct3D.TextureOperation.Modulate
		*/
	}

	inline void DisableTexturing(int stage)
	{
		D3DDevice->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_DISABLE);
	}

	/**
	 * We should get rid of FVF ASAP!!!
	 */
	inline void RenderMesh(VMesh* mesh)
	{
		D3DDevice->SetStreamSource(0,mesh->Vertices->BufferObject,0,mesh->Vertices->BufferFormat->BytesPerItem);
		D3DDevice->SetFVF(((VVertexBufferFormat*)(mesh->Vertices->BufferFormat))->FormatDescriptor);
		D3DDevice->DrawPrimitive((D3DPRIMITIVETYPE)mesh->MeshType,0,mesh->PrimitiveCount);
	}

	inline void RenderVertexBuffer(VVertexBuffer* buffer)
	{
		
	}

	inline rvbf CreateVertexBuffer(VVertexBuffer* buffer, int capacity)
	{
		VVertexBufferFormat* bfmt = ((VVertexBufferFormat*)(buffer->BufferFormat));
		LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
		D3DDevice->CreateVertexBuffer(capacity,0,bfmt->FormatDescriptor,D3DPOOL_DEFAULT,&g_pVertexBuffer, NULL);
		buffer->BufferObject = g_pVertexBuffer;
		return g_pVertexBuffer;
	}

	inline void DeleteVertexBuffer(VVertexBuffer* buffer)
	{
		LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = buffer->BufferObject;
		g_pVertexBuffer->Release();

	}

	inline void LockVertexBuffer(VVertexBuffer* buffer, int offset, int length)
	{
		LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = buffer->BufferObject;
		void* k;
		g_pVertexBuffer->Lock(offset,length,&k,NULL);
		buffer->Data = (byte*)k;
	}

	inline void UnlockVertexBuffer(VVertexBuffer* buffer)
	{
		LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = buffer->BufferObject;
		g_pVertexBuffer->Unlock();
	}

	inline void SetView(const mat4& matrix)
	{
		D3DDevice->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&matrix);
	}

	inline void SetProjection(const mat4& matrix)
	{
		D3DDevice->SetTransform(D3DTS_PROJECTION,(D3DXMATRIX*)&matrix);
	}

	inline void SetDepthTest(bool enabled)
	{
		D3DDevice->SetRenderState(D3DRS_ZENABLE,enabled);
	}

	inline void SetWorld(const mat4& matrix)
	{
		D3DDevice->SetTransform(D3DTS_WORLD,(D3DXMATRIX*)&matrix);
	}

	/**
	* Enables 2D rendering mode, thats good for rendering hud, gui and on screen stuff.
	*/
	inline void Enter2D()
	{
		MProjectionMatrix mOrtho;
		mOrtho.OrthoOffCenterR(0,(float)vWidth,(float)vHeight,0.0f,0.0f,1.0f);
		SetProjection(mOrtho);
		SetDepthTest(false);
		// save old projection ,view and world matrices
	}

	inline void Exit2D()
	{
		// restore old projection, view and world matrices
		// set depth test back?
	}

	inline void EnableBlending(ui32 src = RL_SRCALPHA,ui32 dst = RL_INVSRCALPHA)
	{
		D3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		D3DDevice->SetRenderState( D3DRS_SRCBLEND, src );
		D3DDevice->SetRenderState( D3DRS_DESTBLEND, dst );
	}

	inline void DisableBlending()
	{
		D3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	}

	inline void EnableScissor(const IRectangle& clipRect)
	{
		RECT cwinRect;
		cwinRect.left = clipRect.X;
		cwinRect.top = clipRect.Y;
		cwinRect.right = clipRect.Right();
		cwinRect.bottom = clipRect.Bottom();

		D3DDevice->SetScissorRect(&cwinRect);
		D3DDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
	}

	inline void DisableScissor()
	{
		D3DDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
	}

	rtex LoadTextureFromBitmap(TBitmap* bmp, bool automipmap = true);
	void UpdateTextureFromBitmap(rtex tx, TBitmap* bmp);
	
// Functions that shouldn't be needed to implemented by other drivers
private:
	void LoadSurfaceFromBitmap( void* surf, TBitmap* bmp);
};



#endif