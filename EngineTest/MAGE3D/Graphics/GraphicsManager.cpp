#include "precompiled.h"
#include "GraphicsManager.h"
#include <Core/Log.h>

namespace MAGE3D
{
namespace Graphics
{

GraphicsManager::GraphicsManager()
	:mpD3D9(NULL)
	,mpDevice(NULL)
	,mInitalized(false)
{
	ZeroMemory(&mD3DCaps, sizeof(mD3DCaps));
	ZeroMemory(&mD3DPP, sizeof(mD3DPP));
}

//------------------------------------------------------------------------------------------

GraphicsManager::~GraphicsManager()
{
	Terminate();
}

//------------------------------------------------------------------------------------------

void GraphicsManager::Initalize(HWND hWindow, u32 iWidth, u32 iHeight, bool bFullscreen)
{
	// check if we have already intialized the graphics system
	ASSERT(!mInitalized, "[Graphics manager] system already initalized");

	// write to log
	CORE::Log::Get().Message("[GrpahicsManager] Initializing...",CORE::kSystem);

	// obtain an interface to Direct3D
	mpD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	// get device capabilities
	mpD3D9->GetDeviceCaps
	(
		D3DADAPTER_DEFAULT, // get primary display adapter
		D3DDEVTYPE_HAL,		// device type
		&mD3DCaps			// Direct3D capabilities struct
	);

	// Check for hardware vertex processing
	int iVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	if(mD3DCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		iVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	// Set presentation parameters
	mD3DPP.BackBufferWidth					= iWidth;
	mD3DPP.BackBufferHeight					= iHeight;
	mD3DPP.BackBufferFormat					= D3DFMT_A8R8G8B8;
	mD3DPP.BackBufferCount					= 1;
	mD3DPP.MultiSampleType					= D3DMULTISAMPLE_NONE;
	mD3DPP.MultiSampleQuality				= 0;
	mD3DPP.SwapEffect						= D3DSWAPEFFECT_DISCARD;
	mD3DPP.hDeviceWindow					= hWindow;
	mD3DPP.Windowed							= !bFullscreen;
	mD3DPP.EnableAutoDepthStencil			= true;
	mD3DPP.AutoDepthStencilFormat			= D3DFMT_D24S8;
	mD3DPP.Flags							= 0;
	mD3DPP.FullScreen_RefreshRateInHz		= D3DPRESENT_RATE_DEFAULT;
	mD3DPP.PresentationInterval				= D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = mpD3D9->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWindow,
		iVertexProcessing,
		&mD3DPP,
		&mpDevice
	);

	// check if we have a valid device
	ASSERT(SUCCEEDED(hr), "[GraphicsManager] Failed to create Direct3D devicce.");

	// setflag
	mInitalized = true;

	// write to log
	CORE::Log::Get().Message("[GraphicsManager] system initalized.",CORE::kSystem);
}

//------------------------------------------------------------------------------------------

void GraphicsManager::Terminate()
{
	if(mInitalized)
	{
		// write to log
		CORE::Log::Get().Message("[GraphicsManager] Terminating",CORE::kSystem);

		// release everything
		SafeRelease(mpDevice);
		SafeRelease(mpD3D9);

		// clear flag
		mInitalized = false;

		// write to log
		CORE::Log::Get().Message("[GraphicsManager] Terminated",CORE::kSystem);
	}
}

//------------------------------------------------------------------------------------------


IDirect3DDevice9* GraphicsManager::Device(void)
{
	ASSERT(mInitalized, "[GraphicsManager] Initialized() needs to be called first");

	return mpDevice;
}
//------------------------------------------------------------------------------------------


}//namespace MAGE3D
}//namespace Graphics

