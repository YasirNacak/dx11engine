#pragma once
#include <chrono>

namespace s3d{ namespace utility
{
	class Timer
	{
	public:
		Timer();

		double GetMillisecondsElapsed() const;
		void Restart();
		bool Start();
		bool Stop();

	private:
		bool _isRunning = false;
		std::chrono::time_point<std::chrono::steady_clock> _start;
		std::chrono::time_point<std::chrono::steady_clock> _stop;
	};
} }
