//====================================================================================================
//	Filename:		Font.h
//	Created by:		Matt Stinnen
//====================================================================================================

//====================================================================================================
//Includes
//====================================================================================================
#include "precompiled.h"
#include "Font.h"
#include "CORE/Log.h"

//====================================================================================================
//namespace
//====================================================================================================
namespace MAGE3D
{
namespace Graphics
{

static const char* kFontName[] = 
{
	"Arial",
	"Courier New",
	"Times New Roman",
};
//====================================================================================================
//class definitons
//====================================================================================================

Font::Font()
	:mFont(NULL)
	,mColor(0xFFFFFFFF)
	,mpMessage("Message Not Set!")
	,mFormat(DT_LEFT | DT_TOP | DT_NOCLIP)
{
	D3DXFONT_DESC temp = { 24,
                      0,
                      400,
                      0,
                      false,
                      DEFAULT_CHARSET,
                      OUT_TT_PRECIS,
                      CLIP_DEFAULT_PRECIS,
                      DEFAULT_PITCH,
					  "Arial"};
	mFontDesc = temp;

	mFontPosition.top = 0;
	mFontPosition.left = 0;
	mFontPosition.bottom = 10000;
	mFontPosition.right = 10000;
}

//----------------------------------------------------------------------------------------------------

Font::~Font()
{
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void Font::Initilize(Type type, IDirect3DDevice9* pDevice , int size, bool bold, bool italic)
{
	HRESULT hr = D3DXCreateFont(
								pDevice,	// Direct3D device
								size,							  // Font height
								0,								  // Font width (0 = default based on height)
								(bold) ? 800 : 500,			  // Weight (0 - 1000)
								1,								  // MipMap level
								italic,							// Use italic?
								DEFAULT_CHARSET,					// Character set
								OUT_DEFAULT_PRECIS,				 // Output precision
								DEFAULT_QUALITY,					// Quality
								DEFAULT_PITCH | FF_DONTCARE,		// Pitch and font family
								kFontName[type],
								&mFont
							);
	if (FAILED(hr))
	{
		// Write to log
		CORE::Log::Get().Message("[Font] Failed to create Direct3D font.",CORE::kError);
		return;
	}
}

//----------------------------------------------------------------------------------------------------

void Font::Terminate()
{
	SafeRelease(mFont);
}

//----------------------------------------------------------------------------------------------------

void Font::SetColor(int r, int g, int b)
{
	// Set color
	mColor = D3DCOLOR_ARGB(255, r, g, b);
}

//----------------------------------------------------------------------------------------------------

void Font::SetColor(int a, int r, int g, int b)
{
	// Set color
	mColor = D3DCOLOR_ARGB(a, r, g, b);
}

//----------------------------------------------------------------------------------------------------

void Font::PrintText(const char* pMessage)
{
	if (NULL == mFont)
	{
		// Write to log
		CORE::Log::Get().Message("[Font] Cannot draw text without a font.",CORE::kError);
		return;
	}
	// Render the string
	mFont->DrawText(NULL, pMessage, -1, &mFontPosition, mFormat, mColor);

}

//----------------------------------------------------------------------------------------------------
void Font::SetPosition(int Left, int Top, int Right, int Bottom) 
{
	mFontPosition.left = Left; 
	mFontPosition.top = Top; 
	mFontPosition.right = Right; 
	mFontPosition.bottom = Bottom;
}

}//namespace MAGE3D
}//namespace Graphics



