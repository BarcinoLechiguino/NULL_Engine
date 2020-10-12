// ----------------------------------------------------
// Timer.cpp --- CPU Tick Timer class.
// Fast timer with millisecond precision.
// ----------------------------------------------------

#include "SDL/include/SDL.h"
#include "Timer.h"

// ---------------------------------------------
Timer::Timer() : running(false), started_at(0), stopped_at(0), clock(0, 0, 0)
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;																// Starts the timer.
	started_at = SDL_GetTicks();												// Registers the starting time in milliseconds.
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;															// Stops the timer.
	stopped_at = SDL_GetTicks();												// Registers the stopping time in milliseconds.
}

// ---------------------------------------------
Uint32 Timer::Read() const
{
	if (running)
	{
		return SDL_GetTicks() - started_at;										// Returns the time that has elapsed since the start in milliseconds.
	}
	else
	{
		return stopped_at - started_at;											// Returns the time that has elapsed since the stop in milliseconds.
	}
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	if (running)
	{
		return (SDL_GetTicks() - started_at) / 1000.0f;							// Returns the time that has elapsed since the start in seconds.
	}
	else
	{
		return (stopped_at - started_at);										// Returns the time that has elapsed since the stop in seconds.
	}												
}

bool Timer::IsActive() const
{
	return running;
}

void Timer::AddTimeToClock()
{
	clock.AddTime(running);
}

// Clock ---------------------
Clock::Clock()
{
	hours			= 0;
	minutes			= 0;
	seconds			= 0.0f;

	previous_ticks	= 0;
}

Clock::Clock(uint hours, uchar minutes, float seconds)
{
	this->hours		= hours;
	this->minutes	= minutes;
	this->seconds	= seconds;

	previous_ticks	= 0;
}

void Clock::AddTime(bool timer_is_active)
{
	if (timer_is_active)
	{
		seconds += (SDL_GetTicks() - previous_ticks) / 1000.0f;		// Returns the time that has elapsed since the start in seconds.

		previous_ticks = SDL_GetTicks();

		if (seconds >= 60.0f)
		{
			++minutes;
			seconds = 0.0f;
		}

		if (minutes >= 60)
		{
			++hours;
			minutes = 0;
		}
	}
}