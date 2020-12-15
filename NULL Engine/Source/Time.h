#ifndef __TIME_H__
#define __TIME_H__

#include "Timer.h"
#include "Hourglass.h"
#include "FrameData.h"

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

namespace Time
{
	void Sleep(uint ms);
	
	namespace Real
	{
		void		InitRealTimers		();
		void		Update				();								// Will update the frame data for the frame it was called on. frame_count, prev_sec_frames, dt..
		void		DelayUntilFrameCap	(uint frame_cap);

		Hourglass	GetClock			();
		FrameData	GetFrameData		();

		float		GetDT				();
		uint64		GetFrameCount		();
		float		GetTimeSinceStart	();
		uint32		GetFramesLastSecond	();
		float		GetAverageFPS		();
		uint32		GetMsLastFrame		();

		namespace Utilities
		{
			//static	Timer			startup_timer;
			static	Timer			frame_timer;
			static	Hourglass		clock;
			static	FrameData		frame_data;
		}
	}

	namespace Game
	{
		// See Time Management presentation. Slide nº 5.
		void	UpdateFrameData		();

		void	ActivateClock		();
		void	DeactivateClock		();

		bool	Play				();
		bool	Pause				();
		bool	Step				(uint num_steps);
		bool	Stop				();

		namespace Utilities
		{
			static float			time_scale;
			
			static Timer			startup_timer;
			static Timer			frame_timer;
			static Hourglass		clock;
			static FrameData		frame_data;
		}
	}
}

#endif // !__TIME_H__