#include "SDL/include/SDL_timer.h"

#include "VariableTypedefs.h"
#include "Log.h"

#include "Time.h"

using namespace Time::Real::Utilities;

void Time::Sleep(uint ms)
{
	SDL_Delay(ms);
}

// --- REAL CLOCK METHODS ---
void Time::Real::InitRealTimers()
{
	//startup_timer.Start();
	frame_timer.Start();
}

void Time::Real::Update()
{
	uint ms = frame_timer.Read();

	clock.Update(ms);
	frame_data.Update(ms);

	frame_timer.Start();
}

void Time::Real::DelayUntilFrameCap(uint frame_cap)
{
	if (frame_cap == 0)														// If frame_cap = 0, then no delay will be applied.
	{
		return;
	}
	
	uint32 current_frame_ms = Utilities::frame_timer.Read();
	uint32 frame_cap_ms		= 1000 / frame_cap;

	if (current_frame_ms < frame_cap_ms)
	{
		//precise_delay_timer.Start();

		uint required_delay = frame_cap_ms - current_frame_ms;

		Sleep(required_delay);
	}
}

Hourglass Time::Real::GetClock()
{
	return clock;
}

FrameData Time::Real::GetFrameData()
{
	return frame_data;
}

uint64 Time::Real::GetFrameCount()
{
	return frame_data.frame_count;
}

float Time::Real::GetTimeSinceStart()
{
	return frame_data.time_since_start;
}

uint32 Time::Real::GetFramesLastSecond()
{
	return frame_data.frames_last_second;
}

float Time::Real::GetAverageFPS()
{
	return frame_data.avg_fps;
}

uint32 Time::Real::GetMsLastFrame()
{
	return frame_data.ms_last_frame;
}

float Time::Real::GetDT()
{
	return frame_data.dt;
}

// --- GAME CLOCK METHODS ---