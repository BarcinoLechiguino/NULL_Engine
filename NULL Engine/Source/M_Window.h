#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Dependencies/SDL/include/SDL.h"

class Application;
class Configuration;

class M_Window : public Module
{
public:

	M_Window(bool is_active = true);
	virtual ~M_Window();							// Destructor

	bool Init(Configuration& config) override;
	bool CleanUp() override;
	
public:
	int Width() const;
	int Height() const;

	void SetTitle(const char* title);

public:
	SDL_Window* window;								// The window we'll be rendering to
	SDL_Surface* screen_surface;					// The surface contained by the window
};

#endif // __ModuleWindow_H__