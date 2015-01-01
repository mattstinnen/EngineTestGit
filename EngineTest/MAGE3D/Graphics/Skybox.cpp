//====================================================================================================
//	Filename:		Skybox.cpp
//	Created by:		Matt Stinnen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================

#include "Precompiled.h"
#include "Skybox.h"
#include "math/math.h"
#include "CORE/Log.h"


//====================================================================================================
//namespace
//====================================================================================================
namespace MAGE3D
{
namespace Graphics
{

struct Vertex
{
	Math::Vector3 position;
	float fU;
	float fV;
	static const u32 FVF;
};

const u32 Vertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;
//====================================================================================================
//class definitons
//====================================================================================================

Skybox::Skybox()
{

}
Skybox::~Skybox()
{

}
void Skybox::Create(IDirect3DDevice9* pDevice, const char* pFilename)
{
	FILE* pFile = NULL;
	fopen_s(&pFile,pFilename, "r");					// open the texture file
	ASSERT(pFile,"[Skybox] pFile failed to open");	// check incase of a failure

	char buffer[kMaxStringSize];

	fscanf_s(pFile,"%s",buffer,sizeof(buffer));			// read the first line of the file to see if it is a skybox file

	if(strcmp(buffer,"[Skybox_Data]") != 0)
	{
		CORE::Log::Get().Message("[Skybox] the file for the textures is an incorrect file type",CORE::kError);
		return;
	}
	else
	{
		CORE::Log::Get().Message("[Skybox] the file is open and is reading",CORE::kSystem);
	}
	
	for(int i = 0; i < 6; ++i)// getting texture names
	{
		char textureName[kMaxStringSize];
		fscanf_s(pFile,"%s",textureName,sizeof(textureName));
		D3DXCreateTextureFromFile(pDevice, textureName, &mpTextures[i]);		
	}
	
	fclose(pFile);
	
	
	pDevice->CreateVertexBuffer
	(
		24 * sizeof(Vertex),	//buffer size bytes // 24 when done
		D3DUSAGE_WRITEONLY,		// usage
		Vertex::FVF,
		D3DPOOL_MANAGED,		//memory pool
		&mpVertexBuffer,		// pointer to vertex buffer interface
		NULL					//shared handle
	);

	Vertex* pVerticies = NULL;
	mpVertexBuffer->Lock(0,0,(void**)&pVerticies,0);

	// front
	pVerticies[0].position = Math::Vector3(-5.0f, -5.0f, 5.0f);
	pVerticies[0].fU = 0.0f;
	pVerticies[0].fV = 1.0f;
	pVerticies[1].position = Math::Vector3(-5.0f, 5.0f, 5.0f);
	pVerticies[1].fU = 0.0f;
	pVerticies[1].fV = 0.0f;
	pVerticies[2].position = Math::Vector3(5.0f, -5.0f, 5.0f);
	pVerticies[2].fU = 1.0f;
	pVerticies[2].fV = 1.0f;
	pVerticies[3].position = Math::Vector3(5.0f, 5.0f, 5.0f);
	pVerticies[3].fU = 1.0f;
	pVerticies[3].fV = 0.0f;

	//Top
	pVerticies[4].position = Math::Vector3(-5.0f, 5.0f, 5.0f);
	pVerticies[4].fU = 0.0f;
	pVerticies[4].fV = 1.0f;
	pVerticies[5].position = Math::Vector3(-5.0f, 5.0f, -5.0f);
	pVerticies[5].fU = 0.0f;
	pVerticies[5].fV = 0.0f;
	pVerticies[6].position = Math::Vector3(5.0f, 5.0f, 5.0f);
	pVerticies[6].fU = 1.0f;
	pVerticies[6].fV = 1.0f;
	pVerticies[7].position = Math::Vector3(5.0f, 5.0f, -5.0f);
	pVerticies[7].fU = 1.0f;
	pVerticies[7].fV = 0.0f;
	
	//bottom
	pVerticies[8].position = Math::Vector3(5.0f, -5.0f, -5.0f);
	pVerticies[8].fU = 1.0f;
	pVerticies[8].fV = 1.0f;
	pVerticies[9].position = Math::Vector3(-5.0f, -5.0f, -5.0f);
	pVerticies[9].fU = 0.0f;
	pVerticies[9].fV = 1.0f;
	pVerticies[10].position = Math::Vector3(5.0f, -5.0f, 5.0f);
	pVerticies[10].fU = 1.0f;
	pVerticies[10].fV = 0.0f;
	pVerticies[11].position = Math::Vector3(-5.0f, -5.0f, 5.0f);
	pVerticies[11].fU = 0.0f;
	pVerticies[11].fV = 0.0f;

	//Right
	pVerticies[12].position = Math::Vector3(5.0f, 5.0f, 5.0f);
	pVerticies[12].fU = 0.0f;
	pVerticies[12].fV = 0.0f;
	pVerticies[14].position = Math::Vector3(5.0f, -5.0f, 5.0f);
	pVerticies[14].fU = 0.0f;
	pVerticies[14].fV = 1.0f;
	pVerticies[13].position = Math::Vector3(5.0f, 5.0f, -5.0f);
	pVerticies[13].fU = 1.0f;
	pVerticies[13].fV = 0.0f;
	pVerticies[15].position = Math::Vector3(5.0f, -5.0f,-5.0f);
	pVerticies[15].fU = 1.0f;
	pVerticies[15].fV = 1.0f;

	//Left
	pVerticies[16].position = Math::Vector3(-5.0f, -5.0f, 5.0f);
	pVerticies[16].fU = 1.0f;
	pVerticies[16].fV = 1.0f;
	pVerticies[17].position = Math::Vector3(-5.0f, -5.0f,-5.0f);
	pVerticies[17].fU = 0.0f;
	pVerticies[17].fV = 1.0f;
	pVerticies[18].position = Math::Vector3(-5.0f, 5.0f, 5.0f);
	pVerticies[18].fU = 1.0f;
	pVerticies[18].fV = 0.0f;
	pVerticies[19].position = Math::Vector3(-5.0f, 5.0f, -5.0f);
	pVerticies[19].fU = 0.0f;
	pVerticies[19].fV = 0.0f;

	//Back
	pVerticies[20].position = Math::Vector3(-5.0f, -5.0f, -5.0f);
	pVerticies[20].fU = 1.0f;
	pVerticies[20].fV = 1.0f;
	pVerticies[21].position = Math::Vector3(5.0f, -5.0f, -5.0f);
	pVerticies[21].fU = 0.0f;
	pVerticies[21].fV = 1.0f;
	pVerticies[22].position = Math::Vector3(-5.0f, 5.0f, -5.0f);
	pVerticies[22].fU = 1.0f;
	pVerticies[22].fV = 0.0f;
	pVerticies[23].position = Math::Vector3(5.0f, 5.0f, -5.0f);
	pVerticies[23].fU = 0.0f;
	pVerticies[23].fV = 0.0f;
	

	mpVertexBuffer->Unlock();
	CORE::Log::Get().Message("[Skybox] has been initialized",CORE::kSystem);
}
void Skybox::Destroy()
{
	SafeRelease(mpVertexBuffer);

}
void Skybox::Render(IDirect3DDevice9* pDevice)
{
	Math::Matrix4 matView;
	Math::Matrix4 matWorld;
	Math::Matrix4 tempMat;

	pDevice->GetTransform(D3DTS_VIEW,(D3DXMATRIX*)&matView); // get the actual view matrix
	
	pDevice->SetRenderState(D3DRS_ZENABLE,false);			// turn off z buffer stuff
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,false);
	pDevice->SetRenderState(D3DRS_LIGHTING,false);
	
	tempMat = matView;										// temperary matrix for skybox
	tempMat._41 = 0.0f;
	tempMat._42 = 0.0f;
	tempMat._43 = 0.0f;
	pDevice->SetTransform(D3DTS_VIEW,(D3DXMATRIX*)&tempMat);	
															// use it to keep the skybox from moving
	matWorld.SetIdentity();
	pDevice->SetTransform(D3DTS_WORLD,(D3DXMATRIX*)&matWorld);

	pDevice->SetStreamSource(0,mpVertexBuffer,0,sizeof(Vertex));
	
	for(int i = 0; i < 6; ++i) // set the active texture and draw the box panel
	{
		pDevice->SetTexture(0,mpTextures[i]);					
		pDevice->SetFVF(Vertex::FVF);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,(i*4),2);
	}
	
	pDevice->SetTransform(D3DTS_VIEW,(D3DXMATRIX*)&matView); // reset the view matrix so other things can move
	pDevice->SetRenderState(D3DRS_ZENABLE,true);			 // re-enable the z buffer
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,true);
	pDevice->SetRenderState(D3DRS_LIGHTING,true);
	
}

}//namespace MAGE3D
}//namespace Graphics

