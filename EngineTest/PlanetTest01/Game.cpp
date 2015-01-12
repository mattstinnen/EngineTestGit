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
	//testing stuff
	, mpVertexBuffer(NULL)
	, mpVertexBuffer2(NULL)

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
	mGraphicsManager.Device()->SetRenderState(D3DRS_LIGHTING, false);
	mGraphicsManager.Device()->SetRenderState(D3DRS_CULLMODE, true);
	
	//skybox
	mSkybox.Create(mGraphicsManager.Device(), "../Data/Textures/Skybox/Clouds1/Skybox.txt");

	//testing stuff
	mGraphicsManager.Device()->CreateVertexBuffer
		(
		4 * sizeof(Vertex),		//buffer size bytes
		D3DUSAGE_WRITEONLY,		// usage
		Vertex::FVF,
		D3DPOOL_MANAGED,		//memory pool
		&mpVertexBuffer,		// pointer to vertex buffer interface
		NULL					//shared handle
		);

	Vertex* pVerticies = NULL;
	mpVertexBuffer->Lock(0, 0, (void**)&pVerticies, 0);// get buffer ready to input
	// input positions and uvs
	pVerticies[0].position = Math::Vector3(-1.0f, -1.0f, -1.0f);
	pVerticies[0].u0 = 0.0f;
	pVerticies[0].v0 = 0.0f;
	pVerticies[1].position = Math::Vector3(-1.0f, -1.0f, 1.0f);
	pVerticies[1].u0 = 0.0f;
	pVerticies[1].v0 = 1.0f;
	pVerticies[2].position = Math::Vector3(1.0f, -1.0f, -1.0f);
	pVerticies[2].u0 = 1.0f;
	pVerticies[2].v0 = 0.0f;
	pVerticies[3].position = Math::Vector3(1.0f, -1.0f, 1.0f);
	pVerticies[3].u0 = 1.0f;
	pVerticies[3].v0 = 1.0f;

	mpVertexBuffer->Unlock(); // finish input

	// load in the texture
	pPyramideTexture = NULL;
	D3DXCreateTextureFromFile(mGraphicsManager.Device(), "../data/textures/crate.jpg", &pPyramideTexture);
	mGraphicsManager.Device()->SetTexture(0, pPyramideTexture);

}

//----------------------------------------------------------------------------------------------------

void Game::OnTerminate()
{

	//terminate subsystems
	mInputManager.Terminate();
	mGraphicsManager.Terminate();
	mTimer.Terminate();
	mFont.Terminate();

	//skybox
	mSkybox.Destroy();


	//testing stuff
	SafeRelease(mpVertexBuffer);
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


	pDevice->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&matView);
	pDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&matProj);

	// begin render
	pDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL , 0x00000000, 1.0f, 0);
	pDevice->BeginScene();

	
	//set world transform
	Math::Matrix4 matWorld;
	matWorld.SetIdentity();
	matWorld.SetRotateY(mRotation);
	
	
	// all rendering goes here
	//skybox
	mSkybox.Render(pDevice);
	
	//testing stuff
	mGraphicsManager.Device()->SetTexture(0, pPyramideTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetStreamSource(0, mpVertexBuffer, 0, sizeof(Vertex));
	pDevice->SetFVF(Vertex::FVF);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


	//fps
	char buffer[kMaxStringSize];
	sprintf_s(buffer, kMaxStringSize, "FPS: %.2f", mTimer.GetFramesPerSecond());
	mFont.PrintText(buffer);
	// end render

	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);

}
