#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"

class Timer
{
public:
	Timer();								// Constructor

	void Start();							// Initializes the timer.
	void Stop();							// Stops the timer.

	uint32 Read() const;					// Will return the registered time in milliseconds.
	float ReadSec() const;					// Will return the registered time in seconds.

private:
	bool	running;						// Will keep track of whether or not the timer is still active.
	uint32	started_at;						// Will keep track of the exact moment at which the timer started running.
	uint32	stopped_at;						// Will keep track of the exact moment at which the timer stopped running.
};

#endif //__TIMER_H__