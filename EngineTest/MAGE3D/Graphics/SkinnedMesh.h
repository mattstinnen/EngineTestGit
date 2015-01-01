#ifndef INCLUDED_SKINNEDMESH_H
#define INCLUDED_SKINNEDMESH_H

//====================================================================================================
// Filename:	SkinnedMesh.h
// Created by:	Peter Chan
// Description:	Class for a Direct3D skinned mesh.
//====================================================================================================

//====================================================================================================
// Namespace
//====================================================================================================

namespace MAGE3D
{

namespace Graphics
{

//====================================================================================================
// Forward Declarations
//====================================================================================================

struct Bone;

//====================================================================================================
// Class
//====================================================================================================

class SkinnedMesh
{
public:
	// Constructor
	SkinnedMesh(void);

	// Destructor
	virtual ~SkinnedMesh(void);

	// Function to load/unload the skinned mesh
	void Load(IDirect3DDevice9* pDevice, const char* pFilename);
	void Unload(void);

	// Function to update the skinned mesh
	void Update(float fSeconds, const D3DXMATRIX* matWorld = NULL);

	// Function to render the skinned mesh
	void Render(IDirect3DDevice9* pDevice);

	// Functions to set/get an animation
	void SetAnimation(int iAnimationIndex);
	void SetAnimationByName(const char* pName);
	void GetAnimationNames(std::vector<std::string>& animations) const;
	void ResetTime()	{mpAnimController->ResetTime(); mpAnimController->SetTrackPosition(0,0);}

	// Function to get the root bone of the skinned mesh
	const D3DXFRAME* GetRootBone(void) const;

private:
	// Recursive function to update all the matrices in the bone hierarchy
	void UpdateBoneMatricesRecursive(Bone* pBone, D3DXMATRIX* pParentMatrix);

	// Recursive function to render the mesh
	void RenderRecursive(IDirect3DDevice9* pDevice, Bone* pBone);

	// Recursive function to setup the matrices that affect a bone
	void SetupBoneMatrixPointersRecursive(Bone* pBone);

	// Members
	ID3DXAnimationController* mpAnimController;	// Pointer to animation controller interface

	D3DXFRAME* mpRootBone;						// Root bone of the skinned mesh
};

} // namespace Graphics

} // namespace Engine

#endif // #ifndef INCLUDED_SKINNEDMESH_H