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
	,mTextureHandle(NULL)
	,mSpecTextureHandle(NULL)
	,mFFPTechniqueHandle(NULL)
	,mPPLTechniqueHandle(NULL)
	,mSMTechniqueHandle(NULL)
	,mTime(0.0f)
	,mPPTechniqueHandle(NULL)
	,mTechniqueToUse(NULL)
	,mpEffect(NULL)
	,mpMesh(NULL)
	,mpTexture(NULL)
	,mpSpecMapTexture(NULL)
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
	testX = 0;
	testY = 0;
	xInc = 1;
	yInc = 1;
	test= 0;
	r = 0;
	g = 0;
	b = 0;
	random=0;
	srand(time(NULL));



	// compile effect
	ID3DXBuffer* pErrorBuffer = NULL;
	D3DXCreateEffectFromFileA
	(
		mGraphicsManager.Device(),
		"../Data/Shaders/Effex.fx",	// filename
		NULL,							// macros
		NULL,							// Includes
		D3DXSHADER_DEBUG,				// compile options
		NULL,
		&mpEffect,
		&pErrorBuffer
	);

	// check if we have any errors 
	if(pErrorBuffer != NULL)
	{
		MessageBoxA(mWindow, (const char*)pErrorBuffer->GetBufferPointer(), "ERROR", MB_OK | MB_ICONERROR);
		Quit();
		return;
	}

	
	// release buffer
	SafeRelease(pErrorBuffer);

	//create mesh
	ID3DXBuffer* pAdjacencyBuffer = NULL;
	D3DXLoadMeshFromXA
	(
		"../Data/StaticMesh/tiger.x",
		D3DXMESH_MANAGED,
		mGraphicsManager.Device(),
		&pAdjacencyBuffer,
		NULL,
		NULL,
		NULL,
		&mpMesh
	);

	DWORD fvf = mpMesh->GetFVF();
	if(!(fvf & D3DFVF_NORMAL))
	{
		ID3DXMesh* pTemp = NULL;
		mpMesh->CloneMeshFVF(0,fvf | D3DFVF_NORMAL,mGraphicsManager.Device(),&pTemp);
		mpMesh->Release();
		mpMesh = pTemp;
		D3DXComputeNormals(mpMesh,(DWORD*)pAdjacencyBuffer->GetBufferPointer());
	}
	SafeRelease(pAdjacencyBuffer);

	//load texture
	D3DXCreateTextureFromFileA(mGraphicsManager.Device(),"../Data/StaticMesh/Tiger.bmp",&mpTexture);
	D3DXCreateTextureFromFileA(mGraphicsManager.Device(),"../Data/StaticMesh/specMap.bmp",&mpSpecMapTexture);

	
	
	mWorldMatrixHandle = mpEffect->GetParameterByName(0,"WorldMatrix");
	mViewMatrixHandle = mpEffect->GetParameterByName(0,"ViewMatrix");
	mProjMatrixHandle = mpEffect->GetParameterByName(0,"ProjMatrix");
	mViewPositionHandle = mpEffect->GetParameterByName(0,"ViewPosition");
	mTextureHandle = mpEffect->GetParameterByName(0,"Texture0");
	mSpecTextureHandle = mpEffect->GetParameterByName(0,"Texture1");

	mFFPTechniqueHandle = mpEffect->GetTechniqueByName("FFP");
	mPPTechniqueHandle = mpEffect->GetTechniqueByName("PP");
	mPPLTechniqueHandle = mpEffect->GetTechniqueByName("PerPixelLighting");
	mSMTechniqueHandle = mpEffect->GetTechniqueByName("SpecularMap");



	

	//set render states
	mGraphicsManager.Device()->SetRenderState(D3DRS_LIGHTING,false);
	



}

//----------------------------------------------------------------------------------------------------

void Game::OnTerminate()
{

	SafeRelease(mpTexture);
	SafeRelease(mpMesh);
	SafeRelease(mpEffect);
	
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
	if(mInputManager.IsKeyPressed(Keys::F1))
	{
		mTechniqueToUse = mFFPTechniqueHandle;
		techNum = 1;
	}
	if(mInputManager.IsKeyPressed(Keys::F2))
	{
		mTechniqueToUse = mPPTechniqueHandle;
		techNum = 2;
	}
	if(mInputManager.IsKeyPressed(Keys::F3))
	{
		mTechniqueToUse = mPPLTechniqueHandle;
		techNum = 3;
	}
	if(mInputManager.IsKeyPressed(Keys::F4))
	{
		mTechniqueToUse = mSMTechniqueHandle;
		techNum = 4;
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

	
	mpEffect->SetMatrix(mViewMatrixHandle, (D3DXMATRIX*)&matView);
	mpEffect->SetMatrix(mProjMatrixHandle, (D3DXMATRIX*)&matProj);
	
	Math::Vector3 camPosition(mCamera.GetPosition());
	D3DXVECTOR4 viewPosition(camPosition.x,camPosition.y,camPosition.z,1.0f);
	mpEffect->SetVector(mViewPositionHandle,&viewPosition);


	// begin render
	pDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL , 0x00000000, 1.0f, 0);
	pDevice->BeginScene();

	// all rendering goes here
	//set world transform
	Math::Matrix4 matWorld;
	matWorld.SetIdentity();
	matWorld.SetRotateY(mRotation);
	mpEffect->SetMatrix(mWorldMatrixHandle,(D3DXMATRIX*)&matWorld);

	//set texture
	mpEffect->SetTexture(mTextureHandle,mpTexture);
	mpEffect->SetTexture(mSpecTextureHandle,mpSpecMapTexture);

	// set technique
	mpEffect->SetTechnique(mTechniqueToUse);

	UINT numPasses = 0;
	mpEffect->Begin(&numPasses,0);
	for(UINT i = 0; i < numPasses; ++i)
	{
		mpEffect->BeginPass(i);
		mpMesh->DrawSubset(0);
		mpEffect->EndPass();
	}
	mpEffect->End();

	//clear technique
	mpEffect->SetTechnique(NULL);
	//clear tex
	mpEffect->SetTexture(mTextureHandle,NULL);
	mpEffect->SetTexture(mSpecTextureHandle,NULL);

	char buffer[kMaxStringSize];
	sprintf_s(buffer, kMaxStringSize, "FPS: %.2f  F%d", mTimer.GetFramesPerSecond(),techNum);
	
	
	mFont.PrintText(buffer);

	// end render

	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);

}
