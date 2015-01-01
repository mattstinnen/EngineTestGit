#ifndef COMMON_H
#define COMMON_H

//====================================================================================================
//	Filename:		Common.h
//	Created by:		Matt Stinnen
//	Discription:	a header file containing commonly used includes, constants, macros, ect
//====================================================================================================

//====================================================================================================
// Defines
//====================================================================================================

#define DIRECTINPUT_VERSION	0x0800
#define WIN32_LEAN_AND_MEAN	 

//====================================================================================================
// Includes
//====================================================================================================

// Standard headers
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

// STL headers
#include <list>
#include <map>
#include <string>
#include <vector>

// DirectX headers
#include "../DX9/Include/d3dx9.h"
#include "../DX9/Include/dinput.h"

//====================================================================================================
// Libraries
//====================================================================================================

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//====================================================================================================
// Typedefs
//====================================================================================================

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;
typedef signed char		s8;
typedef short			s16;
typedef int				s32;

//====================================================================================================
// Namespace
//====================================================================================================

namespace MAGE3D
{

//====================================================================================================
// Constants
//====================================================================================================

static const int kMaxStringSize = 256;

//====================================================================================================
// Macros
//====================================================================================================

// Add this to Tools->options->debugging->symbols:
// http://msdl.microsoft.com/download/symbols
#define ASSERT(exp, msg)\
{\
	if(!(exp))\
	{\
		std::string errorMessage("<<ASSERT>>: ");\
		errorMessage += (#msg);\
		errorMessage += "\n";\
		OutputDebugStringA(errorMessage.c_str());\
		DebugBreak();\
	}\
}\

#define KEYDOWN(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? true : false)
#define KEYUP(vk_code)		((GetAsyncKeyState(vk_code) & 0x8000) ? false : true)

//====================================================================================================
// Inline Functions
//====================================================================================================

template <typename T>
inline void SafeDelete(T*& ptr)
{
	if(NULL != ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SafeDeleteArray(T*& ptr)
{
	if(NULL != ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SafeRelease(T*& ptr)
{
	if(NULL != ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

} // namespace MAGE3D

#endif//#ifndef COMMON_H