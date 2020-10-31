#ifndef __E_PANEL_H__
#define __E_PANEL_H__

#include <string>
#include <vector>

#include "ImGui.h"
#include "Globals.h"

class E_Panel
{
public:
	E_Panel(const char* name, bool is_active = true);
	virtual ~E_Panel();

	virtual bool	Draw(ImGuiIO& io);
	virtual bool	CleanUp();

public:
	void			Enable();
	void			Disable();

	bool			IsActive() const;
	const char*		GetName() const;

	void			HelpMarker(const char* description);

private:
	const char*		name;
	bool			is_active;
};

#endif // !__E_PANEL_H__