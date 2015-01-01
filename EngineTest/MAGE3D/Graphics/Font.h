#ifndef FONT_H
#define FONT_H

//====================================================================================================
//	Filename:		Font.h
//	Created by:		Matt Stinnen
//	Discription:	a class for creating fonts to be shown on screen
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

class Font 
{
public:

enum Type
{
	kArial=0,
	kCourier,
	kTimesNewRoman
};

	Font();
	~Font();
	void Initilize(Type type, IDirect3DDevice9* pDevice , int size, bool bold, bool italic);
	void Terminate();
	void PrintText(const char* pMessage);
	void SetColor(int r, int g, int b);
	void SetColor(int a, int r, int g, int b);
	void SetPosition(int Left, int Top, int Right, int Bottom);

	


private:
	LPD3DXFONT mFont;
	D3DXFONT_DESC mFontDesc;
	RECT mFontPosition;
	D3DCOLOR mColor;
	DWORD mFormat;
	char* mpMessage;


	
};

}//namespace MAGE3D
}//namespace Graphics

#endif // FONT_H