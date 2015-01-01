//====================================================================================================
// Filename:	InputManager.cpp
// Created by:	MAttStinnen
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Input/InputManager.h"

#include "Core/Log.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace MAGE3D
{

namespace Input
{

//====================================================================================================
// Callback Function
//====================================================================================================

BOOL CALLBACK EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext)
{
	InputManager* pIM = (InputManager*)pContext;

	// Obtain an interface to the enumerated joystick
	HRESULT hr = pIM->mpDI->CreateDevice
	(
		pDIDeviceInstance->guidInstance,
		&pIM->mpGamePad,
		NULL
	);

	if (FAILED(hr))
	{
		// Write to log
		CORE::Log::Get().Message("[InputManager] Failed to create game pad device.",CORE::kError);
	}

	return DIENUM_STOP;
}

//====================================================================================================
// Class Definitions
//====================================================================================================

InputManager::InputManager(void)
	: mpDI(NULL)
	, mpKeyboard(NULL)
	, mpMouse(NULL)
	, mpGamePad(NULL)
	, mMouseX(0)
	, mMouseY(0)
	, mScreenWidth(0)
	, mScreenHeight(0)
	, mInitialized(false)
{
	ZeroMemory(mCurrKeyBuffer, kKeyBufferSize);
	ZeroMemory(mPrevKeyBuffer, kKeyBufferSize);
	ZeroMemory(&mCurrMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mPrevMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mCurrGamePadState, sizeof(DIJOYSTATE));
	ZeroMemory(&mPrevGamePadState, sizeof(DIJOYSTATE));
}

//----------------------------------------------------------------------------------------------------

InputManager::~InputManager(void)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

void InputManager::Initialize(HWND hWindow, int iWidth, int iHeight)
{
	// Check if we have already initialized the system
	ASSERT(!mInitialized, "[InputManager] System already initialized.");

	// Write to log
	CORE::Log::Get().Message("[InputManager] Initializing...",CORE::kSystem);

	// Obtain an interface to DirectInput
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mpDI, NULL)))
	{
		ASSERT(false, "[InputManager] Failed to create DirectInput object.");
	}

	//----------------------------------------------------------------------------------------------------
	// Create keyboard device
	if (FAILED(mpDI->CreateDevice(GUID_SysKeyboard, &mpKeyboard, NULL)))
	{
		ASSERT(false, "[InputManager] Failed to create keyboard device.");
	}

	// Set the keyboard data format
	if (FAILED(mpKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		ASSERT(false, "[InputManager] Failed to set keyboard data format.");
	}

	// Set the keyboard cooperative level
	if (FAILED(mpKeyboard->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY)))
	{
		ASSERT(false, "[InputManager] Failed to set keyboard cooperative level.");
	}

	// Acquire the keyboard device
	if (FAILED(mpKeyboard->Acquire()))
	{
		ASSERT(false, "[InputManager] Failed to acquire keyboard device.");
	}
	//----------------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------------
	// Create mouse device
	if (FAILED(mpDI->CreateDevice(GUID_SysMouse, &mpMouse, NULL)))
	{
		ASSERT(false, "[InputManager] Failed to create mouse device.");
	}

	// Set the mouse data format
	if (FAILED(mpMouse->SetDataFormat(&c_dfDIMouse)))
	{
		ASSERT(false, "[InputManager] Failed to set mouse data format.");
	}

	// Set the mouse cooperative level
	if (FAILED(mpMouse->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		ASSERT(false, "[InputManager] Failed to set mouse cooperative level.");
	}

	// Acquire the mouse device
	if (FAILED(mpMouse->Acquire()))
	{
		ASSERT(false, "[InputManager] Failed to acquire mouse device.");
	}

	// Calculate starting mouse position
	mScreenWidth = iWidth;
	mScreenHeight = iHeight;
	mMouseX = mScreenWidth >> 1;
	mMouseY = mScreenHeight >> 1;
	//----------------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------------
	// Enumerate for game pad device
	if (FAILED(mpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumGamePadCallback, this, DIEDFL_ATTACHEDONLY)))
	{
		// Write to log
		CORE::Log::Get().Message("[InputManager] Failed to enumerate for game pad devices.",CORE::kError);
	}

	// Check if we have a game pad detected
	if (NULL != mpGamePad)
	{
		// Set the game pad data format
		if (FAILED(mpGamePad->SetDataFormat(&c_dfDIJoystick)))
		{
			ASSERT(false, "[InputManager] Failed to set game pad data format.");
		}

		// Set the game pad cooperative level
		if (FAILED(mpGamePad->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			ASSERT(false, "[InputManager] Failed to set game pad cooperative level.");
		}

		// Acquire the game pad device
		if (FAILED(mpGamePad->Acquire()))
		{
			ASSERT(false, "[InputManager] Failed to acquire game pad device.");
		}
	}
	else
	{
		// Write to log
		CORE::Log::Get().Message("[InputManager] No game pad attached.",CORE::kError);
	}

	// Set flag
	mInitialized = true;

	// Write to log
	CORE::Log::Get().Message("[InputManager] System initialized.",CORE::kSystem);
}

//----------------------------------------------------------------------------------------------------

void InputManager::Terminate(void)
{
	// Check if we have already terminated the system
	ASSERT(mInitialized, "[InputManager] System already terminated.");

	// Write to log
	CORE::Log::Get().Message("[InputManager] Terminating...",CORE::kSystem);

	// Release devices
	if (NULL != mpGamePad)
	{
		mpGamePad->Unacquire();
		mpGamePad->Release();
		mpGamePad = NULL;
	}
	if (NULL != mpMouse)
	{
		mpMouse->Unacquire();
		mpMouse->Release();
		mpMouse = NULL;
	}
	if (NULL != mpKeyboard)
	{
		mpKeyboard->Unacquire();
		mpKeyboard->Release();
		mpKeyboard = NULL;
	}

	// Release DirectInput interface
	SafeRelease(mpDI);

	// Clear flag
	mInitialized = false;

	// Write to log
	CORE::Log::Get().Message("[InputManager] System terminated",CORE::kSystem);
}

//----------------------------------------------------------------------------------------------------

void InputManager::Update(void)
{
	// Check if the system is initialized
	ASSERT(mInitialized, "[InputManager] Failed to update input devices. System not initialized.");

	// Update keyboard
	if (NULL != mpKeyboard)
	{
		UpdateKeyboard();
	}

	// Update mouse
	if (NULL != mpMouse)
	{
		UpdateMouse();
	}

	// Update game pad
	if (NULL != mpGamePad)
	{
		UpdateGamePad();
	}
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsKeyDown(int iKey) const
{
	return (mCurrKeyBuffer[iKey] & 0x80) ? true : false;
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsKeyPressed(int iKey) const
{
	bool bCurrState = (mCurrKeyBuffer[iKey] & 0x80) ? true : false;
	bool bPrevState = (mPrevKeyBuffer[iKey] & 0x80) ? true : false;

	return !bPrevState && bCurrState;
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsMouseDown(int iMouse) const
{
	return (mCurrMouseState.rgbButtons[iMouse] & 0x80) ? true : false;
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsMousePressed(int iMouse) const
{
	bool bCurrState = (mCurrMouseState.rgbButtons[iMouse] & 0x80) ? true : false;
	bool bPrevState = (mPrevMouseState.rgbButtons[iMouse] & 0x80) ? true : false;

	return !bPrevState && bCurrState;
}

//----------------------------------------------------------------------------------------------------

int InputManager::GetMouseScreenX(void) const
{
	return mMouseX;
}

//----------------------------------------------------------------------------------------------------

int InputManager::GetMouseScreenY(void) const
{
	return mMouseY;
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsMouseLeftEdge(void) const
{
	return (mMouseX == 0);
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsMouseRightEdge(void) const
{
	return (mMouseX == mScreenWidth - 1);
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsMouseTopEdge(void) const
{
	return (mMouseY == 0);
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsMouseBottomEdge(void) const
{
	return (mMouseY == mScreenHeight - 1);
}

//----------------------------------------------------------------------------------------------------

int InputManager::GetMouseMoveX(void) const
{
	return mCurrMouseState.lX;
}

//----------------------------------------------------------------------------------------------------

int InputManager::GetMouseMoveY(void) const
{
	return mCurrMouseState.lY;
}

//----------------------------------------------------------------------------------------------------

int InputManager::GetMouseMoveZ(void) const
{
	return mCurrMouseState.lZ;
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsGamePadButtonDown(int iButton) const
{
	if (NULL == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgbButtons[iButton] & 0x80) ? true : false;
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsGamePadButtonPressed(int iButton) const
{
	if (NULL == mpGamePad)
	{
		return false;
	}

	bool bCurrState = (mCurrGamePadState.rgbButtons[iButton] & 0x80) ? true : false;
	bool bPrevState = (mPrevGamePadState.rgbButtons[iButton] & 0x80) ? true : false;

	return !bPrevState && bCurrState;
}
//----------------------------------------------------------------------------------------------------

bool InputManager::IsDPadUp(void) const
{
	if (NULL == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgdwPOV[0] == 0);
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsDPadDown(void) const
{
	if (NULL == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgdwPOV[0] == 18000);
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsDPadLeft(void) const
{
	if (NULL == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgdwPOV[0] == 27000);
}

//----------------------------------------------------------------------------------------------------

bool InputManager::IsDPadRight(void) const
{
	if (NULL == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgdwPOV[0] == 9000);
}

//----------------------------------------------------------------------------------------------------

float InputManager::GetLeftAnalogX(void) const
{
	if (NULL == mpGamePad)
	{
		return 0.0f;
	}

	return (mCurrGamePadState.lX / 32767.5f) - 1.0f;
}

//----------------------------------------------------------------------------------------------------

float InputManager::GetLeftAnalogY(void) const
{
	if (NULL == mpGamePad)
	{
		return 0.0f;
	}

	return -(mCurrGamePadState.lY / 32767.5f) + 1.0f;
}

//----------------------------------------------------------------------------------------------------

float InputManager::GetRightAnalogX(void) const
{
	if (NULL == mpGamePad)
	{
		return 0.0f;
	}

	return (mCurrGamePadState.lZ / 32767.5f) - 1.0f;
}

//----------------------------------------------------------------------------------------------------

float InputManager::GetRightAnalogY(void) const
{
	if (NULL == mpGamePad)
	{
		return 0.0f;
	}

	return -(mCurrGamePadState.lRz / 32767.5f) + 1.0f;
}

//----------------------------------------------------------------------------------------------------

void InputManager::UpdateKeyboard(void)
{
	// Store the previous keyboard state
	memcpy(mPrevKeyBuffer, mCurrKeyBuffer, kKeyBufferSize);

	// Get keyboard state
	static bool sWriteToLog = true;
	HRESULT hr = mpKeyboard->GetDeviceState(kKeyBufferSize, (void*)mCurrKeyBuffer);
	if (FAILED(hr))
	{
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			// Write to log
			if (sWriteToLog)
			{
				CORE::Log::Get().Message("[InputManager] Keyboard device is lost.",CORE::kError);
				sWriteToLog = false;
			}

			// Try to acquire keyboard device again
			mpKeyboard->Acquire();
		}
		else
		{
			// Write to log
			CORE::Log::Get().Message("[InputManager] Failed to get keyboard state.",CORE::kError);
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}
}

//----------------------------------------------------------------------------------------------------

void InputManager::UpdateMouse(void)
{
	// Store the previous mouse state
	memcpy(&mPrevMouseState, &mCurrMouseState, sizeof(DIMOUSESTATE));

	// Get mouse state
	static bool sWriteToLog = true;
	HRESULT hr = mpMouse->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&mCurrMouseState);
	if (FAILED(hr))
	{
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			// Write to log
			if (sWriteToLog)
			{
				CORE::Log::Get().Message("[InputManager] Mouse device is lost.",CORE::kError);
				sWriteToLog = false;
			}

			// Try to acquire mouse device again
			mpMouse->Acquire();
		}
		else
		{
			// Write to log
			CORE::Log::Get().Message("[InputManager] Failed to get mouse state.",CORE::kError);
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;

		// Update mouse position
		mMouseX += mCurrMouseState.lX;
		mMouseY += mCurrMouseState.lY;

		// Check bound
		if (mMouseX < 0)				mMouseX = 0;
		if (mMouseX >= mScreenWidth)	mMouseX = mScreenWidth - 1;
		if (mMouseY < 0)				mMouseY = 0;
		if (mMouseY >= mScreenHeight)	mMouseY = mScreenHeight - 1;
	}
}

//----------------------------------------------------------------------------------------------------

void InputManager::UpdateGamePad(void)
{
	// Store the previous game pad state
	memcpy(&mPrevGamePadState, &mCurrGamePadState, sizeof(DIJOYSTATE));

	// Poll the game pad device
	static bool sWriteToLog = true;
	HRESULT hr = mpGamePad->Poll();
	if (FAILED(hr))
	{
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			// Write to log
			if (sWriteToLog)
			{
				CORE::Log::Get().Message("[InputManager] Game pad device is lost.",CORE::kError);
				sWriteToLog = false;
			}

			// Try to acquire game pad device again
			mpGamePad->Acquire();
		}
		else
		{
			// Write to log
			CORE::Log::Get().Message("[InputManager] Failed to get game pad state.",CORE::kError);
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}

	// Get game pad state
	hr = mpGamePad->GetDeviceState(sizeof(DIJOYSTATE), (void*)&mCurrGamePadState);
	if (FAILED(hr))
	{
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			// Write to log
			if (sWriteToLog)
			{
				CORE::Log::Get().Message("[InputManager] Game pad device is lost.",CORE::kError);
				sWriteToLog = false;
			}

			// Try to acquire game pad device again
			mpGamePad->Acquire();
		}
		else
		{
			// Write to log
			CORE::Log::Get().Message("[InputManager] Failed to get game pad state.",CORE::kError);
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}
}

} // namespace Input

} // namespace RAGE