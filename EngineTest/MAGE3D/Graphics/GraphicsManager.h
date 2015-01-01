#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

//====================================================================================================
//	Filename:		GraphicsManager.h
//	Created by:		Matt Stinnen
//	Discription:	a manager for graphics
//====================================================================================================
#include <Core/NoCopyAssign.h>

namespace MAGE3D
{
namespace Graphics
{

class GraphicsManager : public CORE::NoCopyAssign
{
public:

	GraphicsManager();
	~GraphicsManager();

	void Initalize(HWND hWindow, u32 iWidth, u32 iHeight, bool bFullscreen);
	void Terminate();

	// function to return Direct3D device interface
	IDirect3DDevice9* Device(void);

private:
	IDirect3D9* mpD3D9;				// pointer to Direct3d interface
	IDirect3DDevice9* mpDevice;		// pointer to Direct3d  device interface

	D3DCAPS9 mD3DCaps;
	D3DPRESENT_PARAMETERS mD3DPP;	//direct3d presentation parameters

	bool mInitalized;
};

}//namespace MAGE3D
}//namespace Graphics



#endif //GRAPHICSMANAGER_H