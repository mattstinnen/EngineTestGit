//====================================================================================================
//	Filename:		Application.cpp
//	Created by:		Matt Stinnen
//====================================================================================================

//====================================================================================================
// includes
//====================================================================================================

#include "Precompiled.h"

#include "Application.h"

#include "Core/Log.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace MAGE3D
{

namespace Platform
{

//====================================================================================================
// Class definitions
//====================================================================================================

Application::Application()
	: mInstance(NULL)
	, mWindow(NULL)
	, mWinWidth(0)
	, mWinHeight(0)
{
	// empty
}

//----------------------------------------------------------------------------------------------------

Application::~Application()
{
	// empty
}

//----------------------------------------------------------------------------------------------------

void Application::Initialize(const char* pAppName, HINSTANCE hInstance, int iWinWidth, int iWinHeight)
{
	// initalize error log
	CORE::Log::Get().Initilize();

	// Write to log
	CORE::Log::Get().Message("Initializing application");

	// Cache parameters
	mAppName = pAppName;
	mInstance = hInstance;
	mWinWidth = iWinWidth;
	mWinHeight = iWinHeight;

	// Create a new window class
	WNDCLASSEXA wc;
	
	// fill the window class structire
	wc.cbSize			= sizeof(WNDCLASSEXA);
	wc.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= DefWindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= pAppName;
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	
	// Regester the window class
	RegisterClassExA(&wc);

	// Create aa window based on this class
	mWindow = CreateWindowExA
	(
		NULL,
		pAppName,
		pAppName,
		WS_EX_TOPMOST,
		0, 0,
		iWinWidth, iWinHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// Hide the cursor
	SetCursor(NULL);

	// Show the window
	UpdateWindow(mWindow);
	ShowWindow(mWindow, SW_SHOW);
	SetForegroundWindow(mWindow);

	// do any additional initialization
	OnInitialize();
}

//----------------------------------------------------------------------------------------------------

void Application::Terminate()
{
	// Write to log
	CORE::Log::Get().Message("Closing application...");
	
	// any additional clean up
	OnTerminate();

	//Destroy window
	DestroyWindow(mWindow);

	//unregister window class
	UnregisterClassA(mAppName.c_str(), mInstance);

	// Terminate error log
	CORE::Log::Get().Terminate();
}

//----------------------------------------------------------------------------------------------------

void Application::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	//start the message loop
	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			OnMainLoop();
		}
	}

	
}

//----------------------------------------------------------------------------------------------------

void Application::Quit()
{
	PostQuitMessage(0);
}

//----------------------------------------------------------------------------------------------------

}	// namespace Platform
}	// namespace MAGE3D


