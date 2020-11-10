// ----------------------------------------------------
// PerfectTimer.cpp --- CPU Tick Timer class.
// Slow timer with microsecond precision.
// ----------------------------------------------------

#include "SDL/include/SDL.h"

#include "T_PerfectTimer.h"

uint64 T_PerfectTimer::frequency = 0;

// ---------------------------------------------
T_PerfectTimer::T_PerfectTimer() : running(false), started_at(0), stopped_at(0)
{
	if (frequency == 0)
	{
		frequency = SDL_GetPerformanceFrequency();								// Sets the frequency cycles in seconds (coming from microseconds).
	}

	Start();
}

// ---------------------------------------------
void T_PerfectTimer::Start()
{
	running		= true;																		// Starts the timer.
	started_at	= SDL_GetPerformanceCounter();												// Registers the starting time in microseconds.
}

// ---------------------------------------------
void T_PerfectTimer::Stop()
{
	running		= false;																	// Stops the timer.
	stopped_at	= SDL_GetPerformanceCounter();												// Registers the stopping time in microseconds.
}

// ---------------------------------------------
double T_PerfectTimer::ReadMs() const
{
	if (running)
	{
		return (SDL_GetPerformanceCounter() - started_at) / (frequency / 1000.0f);			// Returns the time that has elapsed since the start in milliseconds.
	}
	else
	{
		return (stopped_at - started_at) / (frequency / 1000.0f);							// Returns the time that has elapsed since the stop in milliseconds.
	}
}

// ---------------------------------------------
uint64 T_PerfectTimer::ReadTicks() const
{
	if (running)
	{
		return (SDL_GetPerformanceCounter() - started_at);									// Returns the time that has elapsed since the start in ticks (or microseconds).
	}
	else
	{
		return (SDL_GetPerformanceCounter() - stopped_at);									// Returns the time that has elapsed since the stop in ticks (or microseconds).
	}
}