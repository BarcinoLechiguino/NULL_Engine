#ifndef __TIME_H__
#define __TIME_H__

#include "T_Timer.h"

typedef unsigned int uint;

namespace Time
{
	namespace Real
	{
		static T_Timer real_time;
		
		static bool pause;

		void	InitRealTime();
		float	GetDt();
		void	UpdateFrameData();											// Will update the frame data for the frame it was called on. frame_count, prev_sec_frames, dt..
	}

	namespace Game
	{
		// See Time Management presentation. Slide nº 5.
		void UpdateFrameData();

		void ActivateClock();
		void DeactivateClock();

		bool Play();
		bool Pause();
		bool Step(uint num_steps);
		bool Stop();
	}
}

#endif // !__TIME_H__