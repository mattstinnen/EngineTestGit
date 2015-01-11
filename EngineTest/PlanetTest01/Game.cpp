#include "Game.h"
#include <time.h>


struct Vertex
{
	Math::Vector3 position;
	float u0,v0;
	float u1,v1;
	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_TEX2;
//====================================================================================================
// class definition
//====================================================================================================
Game::Game()
	:mWorldMatrixHandle(NULL)
	,mViewMatrixHandle(NULL)
	,mProjMatrixHandle(NULL)
	,mViewPositionHandle(NULL)
	,mTimerHandle(NULL)
	,mTime(0.0f)
	
	,mRotation(0.0f)
	,techNum(1)

{
	
}

Game::~Game()
{
	//empty
}
	//implements application
void Game::OnInitialize()
{
	
	//intitialize subsystems
	mTimer.Initialize();
	mGraphicsManager.Initalize(mWindow,mWinWidth,mWinHeight,false);
	mInputManager.Initialize(mWindow,mWinWidth,mWinHeight);
	//set camera
	mCamera.SetPosition(Math::Vector3(0.0f,0.0f,-5.0f));
	mCamera.SetDirection(Math::Vector3(0.0f,0.0f,1.0f));
	mCamera.SetFOV(45.0f* Math::kDegToRad);
	mCamera.SetAspectRatio((float)mWinWidth / (float)mWinHeight);
	mCamera.SetNearPlane(0.01f);
	mCamera.SetFarPlane(10000.0f);
	mFont.Initilize(Graphics::Font::kArial,mGraphicsManager.Device(),34,true,true);
	srand(time(NULL));



	//set render states
	mGraphicsManager.Device()->SetRenderState(D3DRS_LIGHTING,false);
	



}

//----------------------------------------------------------------------------------------------------

void Game::OnTerminate()
{

	//terminate subsystems
	mInputManager.Terminate();
	mGraphicsManager.Terminate();
	mTimer.Terminate();
	mFont.Terminate();

	
}

//----------------------------------------------------------------------------------------------------

void Game::OnMainLoop()
{
	//
	mTimer.Update();
	const float kSeconds = mTimer.GetElapsedSeconds();
	mInputManager.Update();
	Update(kSeconds);
	Render();

	
}

//----------------------------------------------------------------------------------------------------

void Game::Update(float fSeconds)
{
	
	const float kMoveSpeed = 5.0f;
	const float kTurnSpeed = 2.0f;

	if(mInputManager.IsKeyDown(Keys::A))
	{
		mCamera.Strafe(-kMoveSpeed * fSeconds);
	}
	if(mInputManager.IsKeyDown(Keys::D))
	{
		mCamera.Strafe(kMoveSpeed * fSeconds);
	}
	if(mInputManager.IsKeyDown(Keys::W))
	{
		mCamera.Walk(kMoveSpeed * fSeconds);
	}
	if(mInputManager.IsKeyDown(Keys::S))
	{
		mCamera.Walk(-kMoveSpeed * fSeconds);
	}
	if(mInputManager.IsKeyDown(Keys::LEFT))
	{
		mRotation += fSeconds * kTurnSpeed;
	}
	if(mInputManager.IsKeyDown(Keys::RIGHT))
	{
		mRotation -= fSeconds * kTurnSpeed;
	}

	mCamera.Yaw(mInputManager.GetMouseMoveX() * fSeconds * kTurnSpeed);
	mCamera.Pitch(mInputManager.GetMouseMoveY() * fSeconds * kTurnSpeed);

	//check esc key
	if(KEYDOWN(VK_ESCAPE))
	{
		Quit();
	}

	// update timer
	mTime += fSeconds;
}

void Game::Render()
{
	IDirect3DDevice9* pDevice = mGraphicsManager.Device();

	//set  matrices
	
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();

	
	
	Math::Vector3 camPosition(mCamera.GetPosition());
	D3DXVECTOR4 viewPosition(camPosition.x,camPosition.y,camPosition.z,1.0f);


	// begin render
	pDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL , 0x00000000, 1.0f, 0);
	pDevice->BeginScene();

	// all rendering goes here
	//set world transform
	Math::Matrix4 matWorld;
	matWorld.SetIdentity();
	matWorld.SetRotateY(mRotation);

	char buffer[kMaxStringSize];
	sprintf_s(buffer, kMaxStringSize, "FPS: %.2f", mTimer.GetFramesPerSecond());
	
	
	mFont.PrintText(buffer);

	// end render

	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);

}
