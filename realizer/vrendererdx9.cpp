#include "stdafx.h"
#include "vrendererdx9.h"
#include "vtexture.h"
#include "vvertexbuffer.h"


bool VRendererDX9::InitializeRenderer( int _width,int _height,const TString& _title,bool fullscr,int bits )
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
	//if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,VertexProcessing,&Present_Parameters, &D3DDevice)))
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
	D3DFORMAT texfmt = (D3DFORMAT)((VTextureFormat*)bmp->BufferFormat)->FormatDescriptor; //BitmapToD3DFormat(bmp->format);
	DWORD usg = 0;

	if (automipmap)
	{
		usg |= D3DUSAGE_AUTOGENMIPMAP;
	}

	D3DXCreateTexture(D3DDevice,bmp->Width,bmp->Height,0,usg,texfmt,D3DPOOL_MANAGED,&rt);

	//bmp->texID = (void*)rt; // small fix before calling func.
	UpdateTextureFromBitmap(rt,bmp); // we can use this function instead YAY!
	return rt;
}

void VRendererDX9::LoadSurfaceFromBitmap( void* surf,TBitmap* bmp )
{
	IDirect3DSurface9 *texturesurface = (IDirect3DSurface9*)surf;
	D3DLOCKED_RECT lockedrect;
	texturesurface->LockRect(&lockedrect,NULL,0);

	byte* pdata = (byte*)lockedrect.pBits; // because it generally return 4 byte data
	byte* bmpdata = bmp->Data;

	// TODO: Check format is capable with d3d device

	//int pix = bmp->PixelCount;

	/*while(pix--)
	{
		*(dword*)pdata = *(dword*)bmpdata;
		pdata+=4;
		bmpdata+=4;
	}*/

	//memset(pdata,128,pix * 4);

	
	MemoryDriver::Copy(pdata,bmpdata, bmp->PixelCount * bmp->BufferFormat->BytesPerItem);

	texturesurface->UnlockRect();
	texturesurface->Release();
}

void VRendererDX9::UpdateTextureFromBitmap( rtex tx, TBitmap* bmp )
{
	IDirect3DSurface9 *texturesurface;
	tx->GetSurfaceLevel(0,&texturesurface);

	LoadSurfaceFromBitmap(texturesurface,bmp);
}

void VRendererDX9::InitializeFormats()
{
	TextureFormats = new VTextureFormats(&TCompositeFormatFactoryTexture::Instance);

	// TODO: test for RGB is supported, if not set it to fallback

	// ((VTextureFormat*)TextureFormats->fRGBA)->Supported(D3DFMT_A8B8G8R8); //D3DFMT_A8R8G8B8

	((VTextureFormat*)TextureFormats->fRGBA)->UnSupported(TextureFormats->fBGRA); //D3DFMT_A8R8G8B8

	((VTextureFormat*)TextureFormats->fBGRA)->Supported(D3DFMT_A8R8G8B8); // TextureFormats->fABGR);
	((VTextureFormat*)TextureFormats->fBGRX)->Supported(D3DFMT_X8R8G8B8);
	((VTextureFormat*)TextureFormats->fBGR)->UnSupported(TextureFormats->fBGRX);
	

	/*((VTextureFormat*)TextureFormats->fARGB)->Supported(D3DFMT_A8R8G8B8);
	((VTextureFormat*)TextureFormats->fABGR)->Supported(D3DFMT_A8B8G8R8);
	
	((VTextureFormat*)TextureFormats->fXBGR)->Supported(D3DFMT_X8B8G8R8);
	
	((VTextureFormat*)TextureFormats->fBGRA)->UnSupported(TextureFormats->fABGR);
	((VTextureFormat*)TextureFormats->fRGB)->UnSupported(TextureFormats->fXRGB);*/
	

	VertexBufferFormats = new VVertexBufferFormats(&TCompositeFormatFactoryVertex::Instance);

	VertexBufferFormats->Textured1 = VertexBufferFormats->CreateVertexFormat("2DTEX1","POST0",D3DFVF_XYZ | D3DFVF_TEX1);
	VertexBufferFormats->ColoredTextured1 = VertexBufferFormats->CreateVertexFormat("2DCLRTEX1","POSCLRT0",D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	/*TBitmapBMP::Install();
	TBitmapTGA::Install();
	TBitmapPNG::Install();*/
}