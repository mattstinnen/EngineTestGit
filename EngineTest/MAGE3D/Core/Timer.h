#ifndef TIMER_H
#define TIMER_H

  
namespace MAGE3D
{
namespace CORE
{

class Timer
{
public: 
	Timer();
	~Timer();

	// functions to initializer terminate the timer
	void Initialize();
	void Terminate();

	// function to update timer and track the time between calls
	void Update();

	// accessors
	float GetElapsedSeconds() const;
	float GetFramesPerSecond() const;

private:
	LARGE_INTEGER mTicksPerSecond; // system clock frequency
	LARGE_INTEGER mLastTick;
	LARGE_INTEGER mCurrentTick;

	float mFramesSinceLastSecond;
	float mAccumulatedSecond;

	float mElapsedSecond;
	float mFramePerSecond;

	bool mInitialized;
};


}// namespace Core
}// namespace MAGE3D


#endif //TIMER_H