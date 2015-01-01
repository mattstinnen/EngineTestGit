//====================================================================================================
// Filename:	SkinnedMesh.h
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Graphics/SkinnedMesh.h"

#include "Graphics/GraphicsManager.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace MAGE3D
{

namespace Graphics
{
	
//====================================================================================================
// Structs
//====================================================================================================

struct Bone : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};

//----------------------------------------------------------------------------------------------------

struct BoneMesh : public D3DXMESHCONTAINER
{
	ID3DXMesh* pOriginalMesh;
	std::vector<D3DMATERIAL9> Materials;
	std::vector<IDirect3DTexture9*> Textures;

	DWORD NumAttributeGroups;
	D3DXATTRIBUTERANGE* pAttributeTable;
	D3DXMATRIX** pBoneMatrices;
	D3DXMATRIX* pBoneOffsetMatrices;
	D3DXMATRIX* pCurrentBoneMatrices;
};

//====================================================================================================
// Class
//====================================================================================================

class SkinnedMeshLoader : public ID3DXAllocateHierarchy
{
public:
	SkinnedMeshLoader(const char* pPath)
		: mPathname(pPath)
	{}

	STDMETHOD(CreateFrame)
	(
		LPCSTR Name,
		LPD3DXFRAME* ppNewFrame
	);

	STDMETHOD(CreateMeshContainer)
	(
		LPCSTR Name,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD iNumMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer
	);

	STDMETHOD(DestroyFrame)
	(
		LPD3DXFRAME pFrameToFree
	);

	STDMETHOD(DestroyMeshContainer)
	(
		LPD3DXMESHCONTAINER pMeshContainerBase
	);

private:
	std::string mPathname;
};

//----------------------------------------------------------------------------------------------------

HRESULT SkinnedMeshLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	// Create new bone
	Bone* pBone = new Bone;
	memset(pBone, 0, sizeof(Bone));

	// Set the bone name
	if (NULL != Name)
	{
		const int kLength = (int)strlen(Name) + 1;
		pBone->Name = new char[kLength];
		strcpy_s(pBone->Name, kLength, Name);
	}

	// Reset the transformation matrices
	D3DXMatrixIdentity(&pBone->TransformationMatrix);
	D3DXMatrixIdentity(&pBone->CombinedTransformationMatrix);

	// Return the new bone
	*ppNewFrame = static_cast<D3DXFRAME*>(pBone);

	return S_OK;
}

//----------------------------------------------------------------------------------------------------

HRESULT SkinnedMeshLoader::CreateMeshContainer
(
	LPCSTR Name,
	CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD iNumMaterials,
	CONST DWORD* pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppNewMeshContainer
)
{
	IDirect3DDevice9* pDevice = NULL;
	pMeshData->pMesh->GetDevice(&pDevice);

	// Create new Bone Mesh
	BoneMesh* pBoneMesh = new BoneMesh;
	memset(pBoneMesh, 0, sizeof(BoneMesh));

	// Set mesh data
	pBoneMesh->pOriginalMesh = pMeshData->pMesh;
	pBoneMesh->MeshData.pMesh = pMeshData->pMesh;
	pBoneMesh->MeshData.Type = pMeshData->Type;

	// Add Reference so that the mesh isn't deallocated
	pMeshData->pMesh->AddRef();

	// Check if we need to generate normals
	const DWORD fvf = pMeshData->pMesh->GetFVF();
	if (!(fvf & D3DFVF_NORMAL))
	{
		// Clone the mesh to make room for the normals
		pMeshData->pMesh->CloneMeshFVF
		(
			D3DXMESH_MANAGED,
			fvf | D3DFVF_NORMAL, 
			pDevice,
			&pBoneMesh->MeshData.pMesh
		);
		
		// Generate the normals
		D3DXComputeNormals(pBoneMesh->MeshData.pMesh, NULL);
	}
	else
	{
		// Clone mesh and store in pBoneMesh->MeshData.pMesh
		pMeshData->pMesh->CloneMeshFVF
		(
			D3DXMESH_MANAGED,
			fvf, 
			pDevice,
			&pBoneMesh->MeshData.pMesh
		);
	}

	// Copy mMaterials and load textures
	pBoneMesh->Materials.reserve(iNumMaterials);
	pBoneMesh->Textures.reserve(iNumMaterials);
	for (unsigned int i = 0; i < iNumMaterials; ++i)
	{
		D3DXMATERIAL mtrl;
		memcpy(&mtrl, &pMaterials[i], sizeof(D3DXMATERIAL));
		pBoneMesh->Materials.push_back(mtrl.MatD3D);

		if (NULL != mtrl.pTextureFilename)
		{
			char textureFullname[kMaxStringSize];
			strcpy_s(textureFullname, kMaxStringSize, mPathname.c_str());
			strcat_s(textureFullname, kMaxStringSize, mtrl.pTextureFilename);

			// Load texture
			IDirect3DTexture9* pTexture = NULL;
			D3DXCreateTextureFromFileA(pDevice, textureFullname, &pTexture);
			pBoneMesh->Textures.push_back(pTexture);
		}
	}

	if (NULL != pSkinInfo)
	{
		// Get Skin Info
		pBoneMesh->pSkinInfo = pSkinInfo;

		//Add reference so that the SkinInfo isn't deallocated
		pSkinInfo->AddRef();

		// Get attribute table
		pBoneMesh->MeshData.pMesh->GetAttributeTable(NULL, &pBoneMesh->NumAttributeGroups);
		pBoneMesh->pAttributeTable = new D3DXATTRIBUTERANGE[pBoneMesh->NumAttributeGroups];
		pBoneMesh->MeshData.pMesh->GetAttributeTable(pBoneMesh->pAttributeTable, NULL);

		// Create bone offset and current matrices
		const int kNumBones = pSkinInfo->GetNumBones();
		pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[kNumBones];
		pBoneMesh->pCurrentBoneMatrices = new D3DXMATRIX[kNumBones];

		// Get bone offset matrices
		for (int i = 0; i < kNumBones; ++i)
		{
			pBoneMesh->pBoneOffsetMatrices[i] = *(pBoneMesh->pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}

	// Set ppNewMeshContainer to the newly created pBoneMesh container
	*ppNewMeshContainer = pBoneMesh;

	return S_OK;
}

//----------------------------------------------------------------------------------------------------

HRESULT SkinnedMeshLoader::DestroyFrame(LPD3DXFRAME pFrame)
{
	if (NULL != pFrame)
	{
		SafeDeleteArray(pFrame->Name);
	}
	SafeDelete(pFrame);

	return S_OK;
}

//----------------------------------------------------------------------------------------------------

HRESULT SkinnedMeshLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	BoneMesh* pBoneMesh = static_cast<BoneMesh*>(pMeshContainerBase);

	// Release textures
	const int kNumTextures = pBoneMesh->Textures.size();
	for (int i = 0; i < kNumTextures; ++i)
	{
		SafeRelease(pBoneMesh->Textures[i]);
	}

	// Release mesh data
	SafeRelease(pBoneMesh->MeshData.pMesh);
	SafeRelease(pBoneMesh->pSkinInfo);
	SafeRelease(pBoneMesh->pOriginalMesh);
	SafeDelete(pBoneMesh);

	return S_OK;
}

//====================================================================================================
// SkinnedMesh Class
//====================================================================================================

SkinnedMesh::SkinnedMesh(void)
	: mpRootBone(NULL)
	, mpAnimController(NULL)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

SkinnedMesh::~SkinnedMesh(void)
{
	// Release everything
	Unload();
}

//----------------------------------------------------------------------------------------------------

void SkinnedMesh::Load(IDirect3DDevice9* pDevice, const char* pFilename)
{
	// Extract the path name
	std::string path = pFilename;
	int iLastSlash = static_cast<int>(path.rfind('/'));
	path.erase(iLastSlash + 1);

	// Create a hierarchy loader
	SkinnedMeshLoader skinnedMeshLoader(path.c_str());

	// Load the mesh hierarchy
	D3DXLoadMeshHierarchyFromXA
	(
		pFilename,
		D3DXMESH_MANAGED,
		pDevice,
		&skinnedMeshLoader,
		NULL,
		&mpRootBone,
		&mpAnimController
	);

	// Setup bone matrices
	SetupBoneMatrixPointersRecursive(static_cast<Bone*>(mpRootBone));

	// Initialize the matrices
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	UpdateBoneMatricesRecursive(static_cast<Bone*>(mpRootBone), &matIdentity);
}

//----------------------------------------------------------------------------------------------------

void SkinnedMesh::Unload(void)
{
	if (NULL != mpRootBone)
	{
		SkinnedMeshLoader skinnedMeshLoader("");
		skinnedMeshLoader.DestroyFrame(mpRootBone);
		mpRootBone = NULL;
	}

	SafeRelease(mpAnimController);
}

//----------------------------------------------------------------------------------------------------

void SkinnedMesh::Update(float fSeconds, const D3DXMATRIX* matWorld)
{
	// Check if we have a matrix to transform with
	D3DXMATRIX matToUse;
	if (NULL != matWorld)
	{
		matToUse = *matWorld;
	}
	else
	{
		D3DXMatrixIdentity(&matToUse);
	}

	// Check if we have a valid animation controller
	if (NULL != mpAnimController)
	{
		mpAnimController->AdvanceTime(fSeconds, NULL);
	}
	// Update matrices
	Bone* pBone = static_cast<Bone*>(mpRootBone);
	UpdateBoneMatricesRecursive(pBone, &matToUse) ;
}


//----------------------------------------------------------------------------------------------------

void SkinnedMesh::Render(IDirect3DDevice9* pDevice)
{
	// Call the recursive render function
	Bone* pBone = static_cast<Bone*>(mpRootBone);
	RenderRecursive(pDevice, pBone);
}

//----------------------------------------------------------------------------------------------------

void SkinnedMesh::SetAnimation(int iAnimationIndex)
{
	// Check if the index if valid
	const int kNumAnimations = static_cast<int>(mpAnimController->GetMaxNumAnimationSets());
	if (iAnimationIndex >= 0 && iAnimationIndex < kNumAnimations)
	{
		ID3DXAnimationSet* pAnimationSet = NULL;
		mpAnimController->GetAnimationSet(iAnimationIndex, &pAnimationSet);

		// Check if we have a valid animation
		if (NULL != pAnimationSet)
		{
			// Set this as the current animation
			mpAnimController->SetTrackAnimationSet(0, pAnimationSet);
		}
		
		// Release the animation set
		pAnimationSet->Release();
	}
}

//----------------------------------------------------------------------------------------------------

void SkinnedMesh::SetAnimationByName(const char* pName)
{
	// Loop through all the animations
	const int kNumAnimations = mpAnimController->GetMaxNumAnimationSets();
	for (int i = 0; i < kNumAnimations; ++i)
	{
		// Get the next animation
		ID3DXAnimationSet* pAnimationSet = NULL;
		mpAnimController->GetAnimationSet(i, &pAnimationSet);

		// Check if we have a valid animation
		if (NULL != pAnimationSet)
		{
			// Check the name of the animation and see if it matches
			if (0 == strcmp(pName, pAnimationSet->GetName()))
			{
				// Set this as the current animation
				mpAnimController->SetTrackAnimationSet(0, pAnimationSet);
			}
			
			// Release the animation set
			pAnimationSet->Release();
		}
	}
}

//----------------------------------------------------------------------------------------------------

void SkinnedMesh::GetAnimationNames(std::vector<std::string>& animations) const
{
	// Clear names
	animations.clear();

	// Loop through all the animations
	const int kNumAnimations = mpAnimController->GetMaxNumAnimationSets();
	for (int i = 0; i < kNumAnimations; ++i)
	{
		// Get the next animation
		ID3DXAnimationSet* pAnimationSet = NULL;
		mpAnimController->GetAnimationSet(i, &pAnimationSet);

		// Check if we have a valid animation
		if (NULL != pAnimationSet)
		{
			// Add animation name to the array
			animations.push_back(pAnimationSet->GetName());

			// Release the animation set
			pAnimationSet->Release();
		
		}
	}
}

//----------------------------------------------------------------------------------------------------

const D3DXFRAME* SkinnedMesh::GetRootBone(void) const
{
	return mpRootBone;
}

//----------------------------------------------------------------------------------------------------

void SkinnedMesh::UpdateBoneMatricesRecursive(Bone* pBone, D3DXMATRIX* pParentMatrix)
{
	// Check to see if we have a valid bone pointer
	if (NULL == pBone)
	{
		return;
	}

	// Calculate the combined transform matrix
	D3DXMatrixMultiply(&pBone->CombinedTransformationMatrix,
						&pBone->TransformationMatrix,
						pParentMatrix);

	// Update sibling with the parent matrix
	if (NULL != pBone->pFrameSibling)
	{
		Bone* pSibling = static_cast<Bone*>(pBone->pFrameSibling);
		UpdateBoneMatricesRecursive(pSibling, pParentMatrix);
	}

	// Update child with the combined matrix
	if (NULL != pBone->pFrameFirstChild)
	{
		Bone* pChild = static_cast<Bone*>(pBone->pFrameFirstChild);
		UpdateBoneMatricesRecursive(pChild, &pBone->CombinedTransformationMatrix);
	}
}

//----------------------------------------------------------------------------------------------------

void SkinnedMesh::RenderRecursive(IDirect3DDevice9* pDevice, Bone* pBone)
{
	// Check if we have a valid bone
	if (NULL == pBone)
	{
		return;
	}

	// Check if bone has a mesh
	if (NULL != pBone->pMeshContainer)
	{
		BoneMesh* pBoneMesh = static_cast<BoneMesh*>(pBone->pMeshContainer);

		// Check if bone mesh has any skin info
		if (NULL != pBoneMesh->pSkinInfo)
		{
			// Set up bone transforms
			const int kNumBones = pBoneMesh->pSkinInfo->GetNumBones();
			for (int i = 0; i < kNumBones; ++i)
			{
				D3DXMatrixMultiply
				(
					&pBoneMesh->pCurrentBoneMatrices[i],
					&pBoneMesh->pBoneOffsetMatrices[i], 
					pBoneMesh->pBoneMatrices[i]
				);
			}

			// Update the skinned mesh vertices
			BYTE* pSrcVertices = NULL;
			BYTE* pDestVertices = NULL;
			pBoneMesh->pOriginalMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pSrcVertices);
			pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&pDestVertices);

			pBoneMesh->pSkinInfo->UpdateSkinnedMesh(pBoneMesh->pCurrentBoneMatrices, NULL, pSrcVertices, pDestVertices);

			pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
			pBoneMesh->pOriginalMesh->UnlockVertexBuffer();

			// Render the mesh
			for (unsigned int i = 0; i < pBoneMesh->NumAttributeGroups; ++i)
			{
				int mtrlIndex = pBoneMesh->pAttributeTable[i].AttribId;

				// Set the material to use
				pDevice->SetMaterial(&(pBoneMesh->Materials[mtrlIndex]));

				// Set the texture to use
				pDevice->SetTexture(0, pBoneMesh->Textures[mtrlIndex]);

				// Render the subset
				pBoneMesh->MeshData.pMesh->DrawSubset(mtrlIndex);
			}
		}
	}

	// Render sibling
	if (NULL != pBone->pFrameSibling)
	{
		Bone* pSibling = static_cast<Bone*>(pBone->pFrameSibling);
		RenderRecursive(pDevice, pSibling);
	}

	// Render child
	if (NULL != pBone->pFrameFirstChild)
	{
		Bone* pChild = static_cast<Bone*>(pBone->pFrameFirstChild);
		RenderRecursive(pDevice, pChild);
	}
}

//----------------------------------------------------------------------------------------------------

void SkinnedMesh::SetupBoneMatrixPointersRecursive(Bone* pBone)
{
	// Check if we have a valid bone
	if (NULL == pBone)
	{
		return;
	}

	// Check if bone has a valid mesh
	if (NULL != pBone->pMeshContainer)
	{
		BoneMesh* pBoneMesh = static_cast<BoneMesh*>(pBone->pMeshContainer);

		// Check if bone mesh has a valid skin info
		if (NULL != pBoneMesh->pSkinInfo)
		{
			const int kNumBones = pBoneMesh->pSkinInfo->GetNumBones();

			// Create new matrix pointers
			pBoneMesh->pBoneMatrices = new D3DXMATRIX*[kNumBones];
			for (int i = 0; i < kNumBones; ++i)
			{
				// Get the next bone that affects this bone's mesh
				Bone* b = static_cast<Bone*>(D3DXFrameFind(mpRootBone, pBoneMesh->pSkinInfo->GetBoneName(i)));
				if (NULL != b)
				{
					pBoneMesh->pBoneMatrices[i] = &b->CombinedTransformationMatrix;
				}
				else
				{
					pBoneMesh->pBoneMatrices[i] = NULL;
				}
			}
		}
	}

	// Update sibling
	if (NULL != pBone->pFrameSibling)
	{
		Bone* pSibling = static_cast<Bone*>(pBone->pFrameSibling);
		SetupBoneMatrixPointersRecursive(pSibling);
	}

	// Update child
	if (NULL != pBone->pFrameFirstChild)
	{
		Bone* pChild = static_cast<Bone*>(pBone->pFrameFirstChild);
		SetupBoneMatrixPointersRecursive(pChild);
	}
}

} // namespace Graphics

} // namespace Engine