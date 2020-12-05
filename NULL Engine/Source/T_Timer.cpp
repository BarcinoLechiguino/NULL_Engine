// ----------------------------------------------------
// Timer.cpp --- CPU Tick Timer class.
// Fast timer with millisecond precision.
// ----------------------------------------------------

#include "SDL/include/SDL.h"

#include "VariableTypedefs.h"

#include "T_Timer.h"

// ---------------------------------------------
T_Timer::T_Timer() : running(false), started_at(0), stopped_at(0), clock(0, 0, 0.0f)
{
	Start();
}

// ---------------------------------------------
void T_Timer::Start()
{
	running = true;																// Starts the timer.
	started_at = SDL_GetTicks();												// Registers the starting time in milliseconds.
}

// ---------------------------------------------
void T_Timer::Stop()
{
	running = false;															// Stops the timer.
	stopped_at = SDL_GetTicks();												// Registers the stopping time in milliseconds.
}

// ---------------------------------------------
Uint32 T_Timer::Read() const
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
float T_Timer::ReadSec() const
{
	if (running)
	{
		return (float)(SDL_GetTicks() - started_at) / 1000.0f;					// Returns the time that has elapsed since the start in seconds.
	}
	else
	{
		return (float)(stopped_at - started_at);								// Returns the time that has elapsed since the stop in seconds.
	}												
}

bool T_Timer::IsActive() const
{
	return running;
}

void T_Timer::AddTimeToClock()
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