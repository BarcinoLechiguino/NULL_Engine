#include "Time.h"

void Time::Real::InitRealTime()
{
	real_time.Start();
}

float Time::Real::GetDt()
{
	return real_time.ReadSec();
}

void Time::Real::UpdateFrameData()
{
	pause = true;
}