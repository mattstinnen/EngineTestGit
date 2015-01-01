#ifndef APPLICATION_H
#define APPLICATION_H

//====================================================================================================
//	Filename:		Application.h
//	Created by:		Matt Stinnen
//	Discription:	a class for creating an application window
//====================================================================================================

//====================================================================================================
// Namespace
//====================================================================================================
namespace MAGE3D
{

namespace Platform
{
//====================================================================================================
// Class declarations
//====================================================================================================
class Application
{
public:
	// Constructor
	Application();

	// Destructer
	virtual ~Application();
	
	//Functions to initialize/terminate the application
	void Initialize(const char* pAppName, HINSTANCE hInstance, int iWinWidth, int iWinHeight);
	void Terminate();
	
	// function to enter main loop
	void Run();

	// function to be called when we want to quit the application
	void Quit();

	int GetWinWidth() {return mWinWidth;}
	int GetWinHeight() {return mWinHeight;}

protected:
	// customizable functions for specific tasks
	virtual void OnInitialize()	= 0;
	virtual void OnTerminate()	= 0;
	virtual void OnMainLoop()	= 0;

	std::string mAppName;

	HINSTANCE mInstance;
	HWND mWindow;
	
	int mWinWidth;
	int mWinHeight;
};



}	// namespace Platform
}	// namespace MAGE3D


#endif // #ifndef APPLICATION_H