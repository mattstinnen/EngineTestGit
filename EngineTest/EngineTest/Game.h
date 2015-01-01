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
	
	int testX;
	int testY;
	int xInc;
	int yInc;
	int random;
	float test;
	int r;
	int g;
	int b;

	ID3DXEffect* mpEffect;
	
	ID3DXMesh* mpMesh;
	IDirect3DTexture9* mpTexture;
	IDirect3DTexture9* mpSpecMapTexture;
	

	D3DXHANDLE mWorldMatrixHandle;
	D3DXHANDLE mViewMatrixHandle;
	D3DXHANDLE mProjMatrixHandle;
	D3DXHANDLE mViewPositionHandle;
	D3DXHANDLE mTimerHandle;
	D3DXHANDLE mTextureHandle;
	D3DXHANDLE mSpecTextureHandle;
	D3DXHANDLE mFFPTechniqueHandle;
	D3DXHANDLE mPPTechniqueHandle;
	D3DXHANDLE mPPLTechniqueHandle;
	D3DXHANDLE mSMTechniqueHandle;
	D3DXHANDLE mTechniqueToUse;

	float mRotation;

	D3DLIGHT9 light;
	
	
	bool mUseShader;
	float mTime;
	int techNum;

	Input::InputManager		mInputManager;
};

#endif // #ifndef GAME_H