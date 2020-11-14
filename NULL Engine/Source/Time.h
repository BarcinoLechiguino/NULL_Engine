#ifndef __TIME_H__
#define __TIME_H__

typedef unsigned int uint;

namespace Time
{
	namespace Real
	{
		void UpdateFrameData();											// Will update the frame data for the frame it was called on. frame_count, prev_sec_frames, dt..
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