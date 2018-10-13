#include "Timer.h"

s3d::utility::Timer::Timer()
{
	_start = std::chrono::high_resolution_clock::now();
	_stop = std::chrono::high_resolution_clock::now();
}

double s3d::utility::Timer::GetMillisecondsElapsed() const
{
	if(_isRunning)
	{
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - _start);
		return elapsed.count();
	}
	
	auto elapsed = std::chrono::duration<double, std::milli>(_stop - _start);
	return elapsed.count();
}

void s3d::utility::Timer::Restart()
{
	_isRunning = true;
	_start = std::chrono::high_resolution_clock::now();
}

bool s3d::utility::Timer::Start()
{
	if (_isRunning)
		return false;

	_start = std::chrono::high_resolution_clock::now();
	_isRunning = true;
	return true;
}

bool s3d::utility::Timer::Stop()
{
	if (!_isRunning)
		return false;
	
	_stop = std::chrono::high_resolution_clock::now();
	_isRunning = false;
	return true;
}
