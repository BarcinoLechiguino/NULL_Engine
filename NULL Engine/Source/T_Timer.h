#ifndef __T_TIMER_H__
#define __T_TIMER_H__

#include "Globals.h"

struct Clock
{
	Clock();
	Clock(uint hours = 0, uchar minutes = 0, float seconds = 0.0f);
	
	void AddTime(bool timer_is_active);

	uint	hours;
	uchar	minutes;
	float	seconds;

	uint previous_ticks;
};

class T_Timer
{
public:
	T_Timer();								// Constructor

	void Start();							// Initializes the timer.
	void Stop();							// Stops the timer.

	uint32 Read() const;					// Will return the registered time in milliseconds.
	float ReadSec() const;					// Will return the registered time in seconds.

	bool IsActive() const;
	void AddTimeToClock();

public:
	Clock	clock;

private:
	bool	running;						// Will keep track of whether or not the timer is still active.
	uint32	started_at;						// Will keep track of the exact moment at which the timer started running.
	uint32	stopped_at;						// Will keep track of the exact moment at which the timer stopped running.
};

#endif //__T_TIMER_H__