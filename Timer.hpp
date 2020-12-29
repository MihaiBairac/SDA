#ifndef TIMER_HPP
#define TIMER_HPP

#include "ClassHelper.h"
#include <chrono>
#include <thread>
#include <functional>
#include <cstddef> // size_t
// IMPLEMENT SOMETHING PLATFORM INDEPENDENT
// Non-multi thread safe!
// More info: https://github.com/eglimi/cpptime/blob/master/cpptime.h


namespace SDA
{
	class Timer
	{
	public:
		typedef long long long_t;
		typedef std::function<void()> OnTimeOutFunc;

		Timer(const OnTimeOutFunc& func = {}, bool isRepeat = false, Timer::long_t timeOutInterval = DEFAULT_TIMEOUT_INTERVAL);
		virtual ~Timer();

		void Start();
		void Restart();
		void Stop();

		Timer::long_t ElapsedTimeInNanoseconds();
		Timer::long_t ElapsedTimeInMicroseconds();
		Timer::long_t ElapsedTimeInMiliseconds();
		Timer::long_t ElapsedTimeInSeconds();

		bool IsRunning() const;
		bool IsRepeat() const;
		Timer::long_t TimeoutInterval() const;

	private:
		NON_COPY_AND_MOVE(Timer)

		std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime, mEndTime;
		bool mIsRunning;
		bool mIsRepeat;
		Timer::long_t mTimeOutInterval;

		std::thread mWorker;
		OnTimeOutFunc mOnTimeOut;

		static const Timer::long_t DEFAULT_TIMEOUT_INTERVAL = 1000; // miliseconds

		void Run();
	};
}
#endif /* TIMER_HPP */