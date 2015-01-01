//====================================================================================================
//	Filename:		HeightMap.cpp
//	Created by:		Matt Stinnen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================
#include "precompiled.h"
#include "TerrainBruteForce.h"
#include "CORE/Log.h"
#include "Math/Math.h"

//====================================================================================================
//namespace
//====================================================================================================
namespace MAGE3D
{
namespace Graphics
{

//====================================================================================================
//Structs
//====================================================================================================
struct TerrainVertex
{
	Math::Vector3 position;
	Math::Vector3 normal;
	float u, v;
	float u2, v2;

	static const u32 FVF;
};

const u32 TerrainVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2;

//====================================================================================================
//class definitons
//====================================================================================================

TerrainBruteForce::TerrainBruteForce()
	:mpVertexBuffer(NULL)
	,mpIndexBuffer(NULL)
	,mpTexture(NULL)
	,mpTexture2(NULL)
	,mpAlphaTex(NULL)
	,mCellSpacing(0.0f)
	,mMaxHeight(0.0f)
	,mWidth(0.0f)
	,mLength(0.0f)
	,mNumVerticies(0)
	,mNumTriangles(0)
{
	// empty
}

//----------------------------------------------------------------------------------------------------

TerrainBruteForce::~TerrainBruteForce()
{
	//release the kraken!
	Destroy();
}

//----------------------------------------------------------------------------------------------------

void TerrainBruteForce::Create(IDirect3DDevice9* pDevice, const char* pFileName)
{
	Destroy();

	// open terrain file
	FILE* pFile = NULL;
	fopen_s(&pFile, pFileName, "r");
	if(NULL == pFile)
	{
		CORE::Log::Get().Message("[Terrain] failed to open terrain file.",CORE::kError);
		return;
	}

	//Validate file
	char buffer[kMaxStringSize];
	fscanf_s(pFile, "%s", buffer, kMaxStringSize);
	if(0 != strcmp(buffer, "[TERRAIN_DATA]"))
	{
		CORE::Log::Get().Message("[Terrain] invalid terrain file",CORE::kError);
		fclose(pFile);
		return;
	}

	// now read the data
	char heightMapName[kMaxStringSize];
	char textureName[kMaxStringSize];
	char textureName2[kMaxStringSize];
	char alphaMapName[kMaxStringSize];
	float tileCount;

	//read the terrain data
	fscanf_s(pFile, "%*s %f",&mCellSpacing);
	fscanf_s(pFile, "%*s %f",&mMaxHeight);
	fscanf_s(pFile, "%*s %s",heightMapName, kMaxStringSize);
	fscanf_s(pFile, "%*s %s",textureName, kMaxStringSize);
	fscanf_s(pFile, "%*s %s",textureName2, kMaxStringSize);
	fscanf_s(pFile, "%*s %s",alphaMapName, kMaxStringSize);
	fscanf_s(pFile, "%*s %f",&tileCount);

	fclose(pFile);

	// extraxct the path
	std::string path = pFileName;
	int lastSlash = static_cast<int>(path.rfind('/',path.length()));
	path.erase(lastSlash + 1);

	// load height data
	char fullHeightMapName[kMaxStringSize];
	sprintf_s(fullHeightMapName,kMaxStringSize,"%s%s", path.c_str(),heightMapName);
	mHeightMap.GenerateFromFile(pDevice,fullHeightMapName,mMaxHeight);

	// load texture
	char fullTextureName[kMaxStringSize];
	sprintf_s(fullTextureName,kMaxStringSize,"%s%s",path.c_str(),textureName);
	D3DXCreateTextureFromFileA(pDevice,fullTextureName,&mpTexture);

	sprintf_s(fullTextureName,kMaxStringSize,"%s%s",path.c_str(),textureName2);
	D3DXCreateTextureFromFileA(pDevice,fullTextureName,&mpTexture2);
	sprintf_s(fullTextureName,kMaxStringSize,"%s%s",path.c_str(),alphaMapName);
	D3DXCreateTextureFromFileA(pDevice,fullTextureName,&mpAlphaTex);
	
	//calculate terrain attributes
	u32 numCol		= mHeightMap.GetNumColumn();
	u32 numRow		= mHeightMap.GetNumRow();
	u32 cellPerCol	= numRow - 1;
	u32 cellPerRow	= numCol - 1;

	mWidth			= cellPerRow * mCellSpacing;
	mLength			= cellPerCol * mCellSpacing;

	mNumVerticies	= numCol * numRow;
	mNumTriangles	= (cellPerRow * cellPerCol) * 2;

	pDevice->CreateVertexBuffer
	(
		mNumVerticies * sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&mpVertexBuffer,
		NULL
	);
	
	//Calculate the uv increment size
	float uIncrementSize = tileCount / static_cast<float>(cellPerRow);
	float vIncrementSize = tileCount / static_cast<float>(cellPerCol);
	float uIncrementSizeAlpha = 1.0f / static_cast<float>(cellPerRow);
	float vIncrementSizeAlpha = 1.0f / static_cast<float>(cellPerCol);
	
	// calculate the offset so the terrain is centered at the origin
	offsetX = mWidth * -0.5f;
	offsetZ = mLength * -0.5f;

	//lock the vertex buffer
	TerrainVertex* pVerticies = NULL;
	mpVertexBuffer->Lock(0,0,(void**)&pVerticies,0);

	// fill the vertexData
	for(u32 j = 0; j < numRow; ++j)
	{
		for(u32 i = 0; i<numCol; ++i)
		{
			const u32 index					= i + (j * numCol);
			pVerticies[index].position.x	= (i * mCellSpacing) + offsetX;
			pVerticies[index].position.y	= mHeightMap.GetHeightEntry(i,j);
			pVerticies[index].position.z	= (j * mCellSpacing) + offsetZ;
			pVerticies[index].normal.Set(0.0f,1.0f,0.0f);
			pVerticies[index].u				= i * uIncrementSizeAlpha;
			pVerticies[index].v				= j * vIncrementSizeAlpha;
			pVerticies[index].u2			= i * uIncrementSize;
			pVerticies[index].v2			= j * vIncrementSize;
		}
	}

	// unlock the vertex buffer
	mpVertexBuffer->Unlock();

	// create the index buffer
	pDevice->CreateIndexBuffer
	(
		mNumTriangles * 3 *sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&mpIndexBuffer,
		NULL
	);

	// lock the index buffer
	WORD* pIndices = NULL;
	mpIndexBuffer->Lock(0,0,(void**)&pIndices,0);

	// loop through and compute the triangles of each quad
	u32 index = 0;
	for(u32 j =0; j < cellPerCol; ++j)
	{
		for(u32 i = 0; i < cellPerRow; ++i)
		{
			//1----2
			//|  / |
			//| /  |
			//0----3

			// 0 - 1 - 2
			pIndices[index]		= i			+ (j		*numCol);
			pIndices[index+ 1 ]	= i			+ ((j + 1)	*numCol);
			pIndices[index + 2]	= (i + 1)	+ ((j + 1)	*numCol);

			// 0 - 2 - 3
			pIndices[index + 3]	= i			+ (j		*numCol);
			pIndices[index + 4]	= (i + 1)	+ ((j + 1)	*numCol);
			pIndices[index + 5]	= (i + 1)	+ (j		*numCol);
			
			// next quad
			index += 6;
		}
	}

	// unlock index buffer
	mpIndexBuffer->Unlock();
}

//----------------------------------------------------------------------------------------------------

void TerrainBruteForce::Destroy()
{
	// release
	SafeRelease(mpTexture2);
	SafeRelease(mpAlphaTex);
	SafeRelease(mpTexture);
	SafeRelease(mpIndexBuffer);
	SafeRelease(mpVertexBuffer);
	// clear height data
	mHeightMap.Purge();
}

//----------------------------------------------------------------------------------------------------

void TerrainBruteForce::Render(IDirect3DDevice9* pDevice)
{
	// set stream source
	pDevice->SetStreamSource(0,mpVertexBuffer,0,sizeof(TerrainVertex));
	pDevice->SetFVF(TerrainVertex::FVF);
	pDevice->SetIndices(mpIndexBuffer);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetTexture(0, mpTexture);
	pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0, mNumVerticies, 0, mNumTriangles);
	
	// Alphamap: take the alpha from the alphamap, we don't care about the color
	pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	// Texture: take the color from the texture, take the alpha from the previous stage
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

	pDevice->SetTexture(0, mpAlphaTex);
	pDevice->SetTexture(1, mpTexture2);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0, mNumVerticies, 0, mNumTriangles);

	pDevice->SetTexture(0,NULL);
	pDevice->SetTexture(1,NULL);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//----------------------------------------------------------------------------------------------------

float TerrainBruteForce::GetHeight(float posX, float posZ)
{
	float localX = posX + (mWidth * 0.5f);
	float localZ = posZ + (mLength * 0.5f);

	// convert position back to row/col indicies
	int col = static_cast<int>(localX / mCellSpacing);
	int row = static_cast<int>(localZ / mCellSpacing);
	
	// ge the height entries
	//
	// C  D
	// *--*
	// | /|
	// *--*
	// A  B

	float A = mHeightMap.GetHeightEntry(col    , row    );
	float B = mHeightMap.GetHeightEntry(col + 1, row    );
	float C = mHeightMap.GetHeightEntry(col	   , row + 1);
	float D = mHeightMap.GetHeightEntry(col + 1, row + 1);

	// get the offset from the top - left of the quad
	float deltaX = (localX / mCellSpacing) - col;
	float deltaZ = 1.0f - ((localZ / mCellSpacing) - row);

	// determine whether the point is with the upper left or the lower right 
	// triangle of the quad. then calculate the height by interpolation
	float height = 0.0f;

	if(deltaZ < 1.0f - deltaX)
	{
		float heightDeltaX = D - C; //C->D
		float heightDeltaZ = A - C; //C->A
		height = C + (deltaX * heightDeltaX) + (deltaZ * heightDeltaZ);
	}
	else
	{
		float heightDeltaX = A - B; //B->A
		float heightDeltaZ = D - B; //B->D
		height = B + ((1.0f - deltaX) * heightDeltaX) + ((1.0f - deltaZ) * heightDeltaZ);
	}

	return height;	
}

//----------------------------------------------------------------------------------------------------

}//namespace MAGE3D
}//namespace Graphics



