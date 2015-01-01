#ifndef SKYBOX_H
#define SKYBOX_H

//====================================================================================================
//	Filename:		Skybox.h
//	Created by:		Matt Stinnen
//	Discription:	a class a skybox
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================



//====================================================================================================
//namespace
//====================================================================================================
namespace MAGE3D
{
namespace Graphics
{

//====================================================================================================
//class declarations
//====================================================================================================

class Skybox
{
public:


	Skybox();
	~Skybox();

	void Create(IDirect3DDevice9* pDevice, const char* pFilename);
	void Destroy();

	void Render(IDirect3DDevice9* pDevice);

protected:

	LPDIRECT3DTEXTURE9 mpTextures[6];
	IDirect3DVertexBuffer9* mpVertexBuffer;
	
};

}//namespace MAGE3D
}//namespace Graphics

#endif // SKYBOX_H