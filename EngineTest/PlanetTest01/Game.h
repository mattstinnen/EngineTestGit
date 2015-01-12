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

	
	// core stuff
	CORE::Timer mTimer;

	Graphics::GraphicsManager	mGraphicsManager;
	Graphics::Camera			mCamera;
	Graphics::Font				mFont;
	Input::InputManager		mInputManager;
	Graphics::Skybox mSkybox;

	//dx9stuff
	D3DXHANDLE mWorldMatrixHandle;
	D3DXHANDLE mViewMatrixHandle;
	D3DXHANDLE mProjMatrixHandle;
	D3DXHANDLE mViewPositionHandle;
	D3DXHANDLE mTimerHandle;
	D3DLIGHT9 light;


	// variables
	float mRotation;
	float mTime;

	


	// testing stuff
	IDirect3DVertexBuffer9* mpVertexBuffer;
	IDirect3DVertexBuffer9* mpVertexBuffer2;

	Math::Matrix4 matTransform;
	Math::Matrix4 matRotationX;
	Math::Matrix4 matRotationY;
	Math::Matrix4 matRotationZ;
	Math::Matrix4 matScaleY;

	LPDIRECT3DTEXTURE9 pPyramideTexture;
};

#endif // #ifndef GAME_H