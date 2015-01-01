//====================================================================================================
//	Filename:		Timer.cpp
//	Created by:		Matt Stinnen
//	Decription:		Class for a high resolution timer
//====================================================================================================

//====================================================================================================
// includes
//====================================================================================================
#include "precompiled.h"
#include "Timer.h"
#include "Core/Log.h"


//====================================================================================================
// namespace
//====================================================================================================
namespace MAGE3D
{
namespace CORE
{

//====================================================================================================
// definitions
//====================================================================================================

Timer::Timer()
	:mFramesSinceLastSecond(0.0f)
	,mAccumulatedSecond(0.0f)
	,mElapsedSecond(0.0f)
	,mFramePerSecond(0.0f)
	,mInitialized(false)
{
	mTicksPerSecond.QuadPart = 0;
	mLastTick.QuadPart = 0;
	mCurrentTick.QuadPart = 0;
}

//------------------------------------------------------------------------------------------

Timer::~Timer()
{
	if(mInitialized)
	{
		Terminate();
	}
}

//------------------------------------------------------------------------------------------

void Timer::Initialize()
{
	// check if we have already initalized timer
	ASSERT(!mInitialized, "[timer] Already Initilized.");

	// write to log
	Log::Get().Message("[Timer] Initializing...",kSystem);

	// get the system clock frequency for time calc
	QueryPerformanceFrequency(&mTicksPerSecond);
	QueryPerformanceCounter(&mCurrentTick);
	mLastTick = mCurrentTick;

	// set flag
	mInitialized = true;

	// write to log

	Log::Get().Message("[Timer] Initalized",kSystem);

}

//------------------------------------------------------------------------------------------

void Timer::Terminate()
{
	// check if we haveent initalized timer
	ASSERT(mInitialized, "[timer] not Initilized.");

	// write to log
	Log::Get().Message("[Timer] Terminating...",kSystem);

	// clear flag
	mInitialized = false;

	// write to log
	Log::Get().Message("[Timer] Terminated",kSystem);

}

//------------------------------------------------------------------------------------------

void Timer::Update()
{
	// Make sure the timer is initalized
	ASSERT(mInitialized, "[Timer] Initialize() muset be called first before using timer.");

	// get the current tick count
	QueryPerformanceCounter(&mCurrentTick);

	// calculate the elapsed timer
	mElapsedSecond = static_cast<float>(mCurrentTick.QuadPart - mLastTick.QuadPart) / mTicksPerSecond.QuadPart;
	mLastTick = mCurrentTick;
	// update frame count
	mAccumulatedSecond += mElapsedSecond;
	mFramesSinceLastSecond += 1.0f;

	// clalculate fps
	if(mAccumulatedSecond >= 1.0f)
	{
		mFramePerSecond = mFramesSinceLastSecond / mAccumulatedSecond;
		mAccumulatedSecond = 0.0f;
		mFramesSinceLastSecond = 0.0f;
	}
}

//------------------------------------------------------------------------------------------

float Timer::GetElapsedSeconds() const
{
	return mElapsedSecond;
}

//------------------------------------------------------------------------------------------

float Timer::GetFramesPerSecond() const
{
	return mFramePerSecond;
}

//------------------------------------------------------------------------------------------


}
}