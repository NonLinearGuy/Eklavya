//
// Created by Ankit Singh on 11-11-2018.
//
#include "Timer.h"


	using namespace std;

	Timer::Timer() : 
		Singleton(),
		m_DeltaTime(0.0), 
		m_ElapsedTime(0.0f)
	{

	}

	Timer::~Timer()
	{

	}

	void Timer::Update() 
	{
		UpdateDelta();
		UpdateFPS();
	}

	void Timer::UpdateDelta()
	{
		mCurrentTime = Clock::now();
		m_DeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>((mCurrentTime - mLastTime)).count();
		mLastTime = mCurrentTime;
	}

	void Timer::UpdateFPS()
	{
		m_ElapsedTime += GetDeltaTimeInSeconds();
		m_FPSCounter++;
		if ( m_ElapsedTime > 1.0f)
		{
			m_FPS = m_FPSCounter;
			m_ElapsedTime = 0.0f;
			m_FPSCounter = 0;
		}
	}

	void Timer::Reset() 
	{
		mLastTime = mCurrentTime = Clock::now();
	}
