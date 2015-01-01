//====================================================================================================
//	Filename:		WinMain.cpp
//	Created by:		Matt Stinnen
//	Discription:	hello Vertex shader demo.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================


#include "Game.h"

//====================================================================================================
// globals
//====================================================================================================

const char* kAppName = "Hello Vertex shader demo!";
const int kWinWidth = 1024;
const int kWinHeight = 768;

//====================================================================================================
// Main
//====================================================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Game myApp;
	myApp.Initialize(kAppName, hInstance, kWinWidth, kWinHeight);
	myApp.Run();
	myApp.Terminate();
	return 0;
}