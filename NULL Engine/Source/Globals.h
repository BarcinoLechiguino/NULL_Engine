#ifndef __GLOBALS_H__
#define __GLOBALS_H__

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>

// Defining NULL just in case
#ifdef NULL
#undef NULL
#endif

#define NULL 0

// Log macros
#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

// Performance macros
#define TIMER_START(timer) timer.Start();
#define TIMER_PEEK(timer) LOG("%s took %f s", __FUNCTION__, timer.ReadSec())
#define PERF_TIMER_START(perf_timer) perf_timer.Start()
#define PERF_TIMER_PEEK(perf_timer) LOG("%s took %f ms", __FUNCTION__, perf_timer.ReadMs())

// Utility macros
#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define IN_RANGE( value, minimum, maximum ) ( ((value) >= (minimum) && (value) <= (maximum)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a ) ( (a != 0) ? true : false )


// Math Definitions
#define DEGTORAD	0.0174532925199432957f
#define RADTODEG	57.295779513082320876f
#define PI			3.14159265358979323846f
#define TWO_PI		6.28318530717958647692f
#define HALF_PI		1.57079632679489661923f
#define QUARTER_PI	0.78539816339744830961f
#define INV_PI		0.31830988618379067154f
#define INV_TWO_PI	0.15915494309189533576f
#define HAVE_M_PI

// Physics Definitions
#define GRAVITY		9.80665f					// Change later to 3D vector.

// String Size Definitions
#define SHORT_STRING	32
#define MEDIUM_STRING	512
#define LARGE_STRING	2048
#define HUGE_STRING		10240

// Type Definitions
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;


enum class UPDATE_STATUS						// Defines all the states that the application can be at when updating.
{
	CONTINUE = 1,
	STOP,
	THROW_ERROR
};

// Configuration ------------------------------------
#define SCREEN_WIDTH			1280
#define SCREEN_HEIGHT			720
#define SCREEN_MIN_WIDTH		640
#define SCREEN_MIN_HEIGHT		480
#define SCREEN_MAX_WIDTH		3840
#define SCREEN_MAX_HEIGHT		2160
#define SCREEN_SIZE				1

#define WIN_MAXIMIZED			true
#define WIN_FULLSCREEN			false
#define WIN_RESIZABLE			true
#define WIN_BORDERLESS			false
#define WIN_FULLSCREEN_DESKTOP	false
#define VSYNC					false
#define FRAMES_ARE_CAPPED		true

#define TITLE					"NULL Engine"
#define ORGANIZATION			"UPC - CITM - TTC"

// File Directories ---------------------------------					// Necessary as a global definition?
#define LIBRARY_PATH			"Library/"
#define ANIMATIONS_PATH			"Library/Animations/"
#define BONES_PATH				"Library/Bones/"
#define FOLDERS_PATH			"Library/Folders/"
#define MATERIALS_PATH			"Library/Materials/"
#define MESHES_PATH				"Library/Meshes/"
#define MODELS_PATH				"Library/Models/"
#define PARTICLES_PATH			"Library/Particles/"
#define SCENES_PATH				"Library/Scenes/"
#define SHADERS_PATH			"Library/Shaders/"

// Deletes a buffer
#define RELEASE( x )			\
	{							\
		if ( x != nullptr )		\
		{						\
			delete x;			\
			x = nullptr;		\
		}						\
	}

// Deletes an array of buffers
#define RELEASE_ARRAY( x )		\
	{							\
		if (x != nullptr)		\
		{						\
			delete[] x;			\
			x = nullptr;		\
		}						\
	}

#endif // !__GLOBALS_H__