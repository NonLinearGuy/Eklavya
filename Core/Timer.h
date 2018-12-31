//
// Created by Ankit Singh on 11-11-2018.
//

#ifndef _INC_TIMER_HPP
#define _INC_TIMER_HPP

#include<chrono>

namespace HipHop
{
	using Clock = std::chrono::steady_clock;
	using TimeStamp = std::chrono::steady_clock::time_point;

	class Timer
	{
	public:
		static Timer* GetInstance()
		{
			static Timer* instance = new Timer();
			return instance;
		}
		inline double GetDeltaTimeInMS() { return m_DeltaTime; }
		inline double GetDeltaTimeInSeconds() { return m_DeltaTime * .001f; }
		inline int GetFramesPerSecond() { return m_FPSCounter; }
		void Update();
		void Reset();
	private:

		void UpdateDelta();
		void UpdateFPS();

		Timer();
		
		//for dt
		TimeStamp mCurrentTime, mLastTime;
		double m_DeltaTime;
		
		//for fps
		double m_ElapsedTime;
		int m_FPSCounter;
	};
}


#endif //BURNOUTENGINE_Timer_HPP
