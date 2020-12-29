#include "Timer.hpp"
#include <cmath>
#include <iostream>

namespace SDA
{
	Timer::Timer(const OnTimeOutFunc& func, bool isRepeat, Timer::long_t timeOutInterval)
		: mIsRunning(false), mIsRepeat(isRepeat), mTimeOutInterval(timeOutInterval)
		, mStartTime(), mEndTime(), mWorker(), mOnTimeOut(func)
	{
		mWorker = std::thread([this] { Run(); });
	}

	Timer::~Timer()
	{
		// we let it run until it finishes
		// otherwise if mIsRunning becomes false
		// then it may exit Run() earlier then the
		// expected timeout interval !

		if (mWorker.joinable())
			mWorker.join();
	}

	void Timer::Start()
	{
		if (mIsRunning)
			return;

		mIsRunning = true;
		mStartTime = std::chrono::high_resolution_clock::now();

	//	std::cout << "Timer Start!" << std::endl;
	}

	void Timer::Restart()
	{
	//	std::cout << "Timer Restart!" << std::endl;

		if (mIsRunning)
		{
			Stop();
		}
		else
		{
			Start();
		}

	}

	void Timer::Stop()
	{
		if (false == mIsRunning)
			return;

		mEndTime = std::chrono::high_resolution_clock::now();
		mIsRunning = false;

	//	std::cout << "Timer Stop!" << std::endl;
	}

	void Timer::Run()
	{
		while (mIsRunning)
		{
			// Timeout

			double timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mStartTime).count();
	
			if (timeDiff >= static_cast<double>(mTimeOutInterval))
			{
				if (mOnTimeOut)
					mOnTimeOut();

				if (mIsRepeat)
				{
					mStartTime = std::chrono::high_resolution_clock::now();
				}
				else
				{
					Stop();
				}
			}
		}
	}

	Timer::long_t Timer::ElapsedTimeInNanoseconds()
	{
		Timer::long_t elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(mEndTime - mStartTime).count();

		return elapsedTime;
	}

	Timer::long_t Timer::ElapsedTimeInMicroseconds()
	{
		Timer::long_t elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(mEndTime - mStartTime).count();

		return elapsedTime;
	}

	Timer::long_t Timer::ElapsedTimeInMiliseconds()
	{
		Timer::long_t elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(mEndTime - mStartTime).count();

		return elapsedTime;
	}

	Timer::long_t Timer::ElapsedTimeInSeconds()
	{
		Timer::long_t elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(mEndTime - mStartTime).count();

		return elapsedTime;
	}

	bool Timer::IsRunning() const
	{
		return mIsRunning;
	}

	bool Timer::IsRepeat() const
	{
		return mIsRepeat;
	}

	Timer::long_t Timer::TimeoutInterval() const
	{
		return mTimeOutInterval;
	}
}