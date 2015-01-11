#ifndef GAME_H
#define GAME_H

//====================================================================================================
//	Filename:		Game.h
//	Created by:		Matt Stinnen
//	Discription:	class for a game application
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <MAGE3D.h>


using namespace MAGE3D;

//====================================================================================================
// class declerations
//====================================================================================================

class Game: public Platform::Application
{
public:
	Game();
	~Game();
protected:
	//implements application
	virtual void OnInitialize();	
	virtual void OnTerminate();
	virtual void OnMainLoop();	
	
	void Update(float fSeconds);
	void Render();

	

	CORE::Timer mTimer;

	Graphics::GraphicsManager	mGraphicsManager;
	Graphics::Camera			mCamera;
	Graphics::Font				mFont;

	D3DXHANDLE mWorldMatrixHandle;
	D3DXHANDLE mViewMatrixHandle;
	D3DXHANDLE mProjMatrixHandle;
	D3DXHANDLE mViewPositionHandle;
	D3DXHANDLE mTimerHandle;
	

	float mRotation;

	D3DLIGHT9 light;
	
	
	bool mUseShader;
	float mTime;
	int techNum;

	Input::InputManager		mInputManager;
};

#endif // #ifndef GAME_H