#include "stdafx.h"
#include "vrendererdx9.h"
#include "vtexture.h"
#include "vvertexbuffer.h"


bool VRendererDX9::InitializeRenderer( int _width,int _height,ch16* _title,bool fullscr,int bits )
{
	Create(_width,_height,_title,fullscr,bits);

	if (!InitializeDirect3D())
	{
		Destroy();
		return false;
	}

	return true;
}

void VRendererDX9::DestroyDirect3D()
{
	if(D3DDevice != NULL)
	{
		D3DDevice->Release();
		D3DDevice = NULL;
	}

	if(D3D != NULL)
	{
		D3D->Release();
		D3D = NULL;
	}

	Destroy();
}

bool VRendererDX9::InitializeDirect3D()
{
	// This object will allow us to set the display mode of the screen.
	D3DDISPLAYMODE DisplayMode;

	// This will allow us to set the parameters of the screen.
	D3DPRESENT_PARAMETERS Present_Parameters;

	// This is used to get the capabilities of the hardware.
	D3DCAPS9 D3DCaps;

	// It is always a good idea to clear out memory in object although not necessary.
	ZeroMemory(&Present_Parameters, sizeof(Present_Parameters));

	// Create the Direct3D object to get everything started.
	D3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (D3D == NULL)
	{
		// set cant create error
		return false;
	}

	// This function will get the display mode of the device and place it in DisplayMode.
	if(FAILED(D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode)))
	{
		//MessageBox(NULL, "Error setting the display mode.", "Error!", MB_OK);
		return false;
	}

	// Get the capabilities of the hardware.
	if(FAILED(D3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &D3DCaps)))
	{
		return false;
	}

	// Test which is supported, hardware or software vertex processing.
	DWORD VertexProcessing = 0;

	if(D3DCaps.VertexProcessingCaps != 0)
	{
		VertexProcessing |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		VertexProcessing |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if (vFullScreen)
	{
		Present_Parameters.Windowed = FALSE;               // Window mode (fullscreen).
		Present_Parameters.BackBufferWidth = vWidth;          // Width resolution.
		Present_Parameters.BackBufferHeight = vHeight;         // Height resolution.	
	}
	else
	{
		Present_Parameters.Windowed = TRUE;                      // Window mode (not fullscreen).
	}

	Present_Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;   // Dealing with animation (see doc).
	Present_Parameters.BackBufferFormat = DisplayMode.Format;// Render to the area of the screen.
	Present_Parameters.BackBufferCount = 1;                  // Number of back buffers.
	Present_Parameters.EnableAutoDepthStencil = TRUE;        // Check documentation.
	Present_Parameters.AutoDepthStencilFormat = D3DFMT_D16;  // Check documentation.

	//TODO: Implement Bit Depth Changing?

	// Now we must create the rendering device.
	if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,VertexProcessing,&Present_Parameters, &D3DDevice)))
	{
	//	MessageBox(NULL, "CreateDevice() failed!  Make sure you have DirectX 9.","Error!", MB_OK);
		return false;
	}

	// One last check to be sure.
	if(D3DDevice == NULL)
	{
		//MessageBox(NULL, "D3D_Device is equal to NULL!?!", "Error!", MB_OK);
		return false;
	}


	// SetRenderState() will set a parameter for the rendering.  Here we turn off lighting.
	D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Disable culling.  This will render the front and back of the triangles.
	D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	InitializeFormats();

	return true;
}

rtex VRendererDX9::LoadTextureFromBitmap( TBitmap* bmp, bool automipmap /*= true*/ )
{
	LPDIRECT3DTEXTURE9 rt;
	D3DFORMAT texfmt = (D3DFORMAT)((VTextureFormat*)bmp->format)->FormatDescriptor; //BitmapToD3DFormat(bmp->format);
	DWORD usg = 0;

	if (automipmap)
	{
		usg |= D3DUSAGE_AUTOGENMIPMAP;
	}

	D3DXCreateTexture(D3DDevice,bmp->width,bmp->height,0,usg,texfmt,D3DPOOL_MANAGED,&rt);

	//bmp->texID = (void*)rt; // small fix before calling func.
	UpdateTextureFromBitmap((rtex)rt,bmp); // we can use this function instead YAY!
	return (rtex)rt;
}

void VRendererDX9::LoadSurfaceFromBitmap( void* surf,TBitmap* bmp )
{
	IDirect3DSurface9 *texturesurface = (IDirect3DSurface9*)surf;
	D3DLOCKED_RECT lockedrect;
	texturesurface->LockRect(&lockedrect,NULL,0);

	byte* pdata = (byte*)lockedrect.pBits; // because it generally return 4 byte data
	byte* bmpdata = bmp->data;

	// TODO: Check format is capable with d3d device

	memcpy(pdata,bmpdata, bmp->pixels * bmp->format->BytesPerItem);

	texturesurface->UnlockRect();
	texturesurface->Release();
}

void VRendererDX9::UpdateTextureFromBitmap( rtex tx, TBitmap* bmp )
{
	LPDIRECT3DTEXTURE9 rt = (LPDIRECT3DTEXTURE9)tx;
	IDirect3DSurface9 *texturesurface;
	rt->GetSurfaceLevel(0,&texturesurface);

	LoadSurfaceFromBitmap(texturesurface,bmp);
}

void VRendererDX9::InitializeFormats()
{
	VTextureFormats k;
	k.Initialize();

	
	((VTextureFormat*)VTextureFormats::fARGB)->Supported(D3DFMT_A8R8G8B8);
	((VTextureFormat*)VTextureFormats::fABGR)->Supported(D3DFMT_A8B8G8R8);
	((VTextureFormat*)VTextureFormats::fRGBA)->UnSupported(VTextureFormats::fARGB);
	((VTextureFormat*)VTextureFormats::fBGRA)->UnSupported(VTextureFormats::fABGR);
	((VTextureFormat*)VTextureFormats::fRGB)->Supported(D3DFMT_R8G8B8);
	((VTextureFormat*)VTextureFormats::fBGR)->UnSupported(VTextureFormats::fRGB);

	VVertexBufferChannels l;
	l.Initialize();

	VVertexBufferFormats::Textured2DFormat = new VVertexBufferFormat("2DTEX1","POSRHWT0",D3DFVF_XYZRHW | D3DFVF_TEX0);

}

void VRendererDX9::CreateVertexBuffer( VVertexBuffer* buffer, int capacity )
{
	VVertexBufferFormat* bfmt = ((VVertexBufferFormat*)(buffer->BufferFormat));
	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
	D3DDevice->CreateVertexBuffer(capacity,0,bfmt->FormatDescriptor,D3DPOOL_DEFAULT,&g_pVertexBuffer, NULL);
	buffer->BufferObject = g_pVertexBuffer;
}

void VRendererDX9::DeleteVertexBuffer( VVertexBuffer* buffer )
{
	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = (LPDIRECT3DVERTEXBUFFER9)buffer->BufferObject;
	g_pVertexBuffer->Release();
}

void VRendererDX9::LockVertexBuffer( VVertexBuffer* buffer, int offset, int length )
{
	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = (LPDIRECT3DVERTEXBUFFER9)buffer->BufferObject;
	void* k;
	g_pVertexBuffer->Lock(offset,length,&k,NULL);
	buffer->Buffer = (byte*)k;
}

void VRendererDX9::UnlockVertexBuffer( VVertexBuffer* buffer )
{
	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = (LPDIRECT3DVERTEXBUFFER9)buffer->BufferObject;
	g_pVertexBuffer->Unlock();
}