#ifndef __E_PANEL_H__
#define __E_PANEL_H__

#include <string>
#include <vector>

#include "ImGui.h"
#include "Log.h"

class E_Panel
{
public:
	E_Panel(const char* name, bool is_active = true);
	virtual ~E_Panel();

	virtual bool	Draw	(ImGuiIO& io);
	virtual bool	CleanUp	();

public:
	void			Enable			();
	void			Disable			();

	const char*		GetName			() const;
	bool			IsActive		() const;
	bool			IsHovered		() const;
	void			SetIsHovered	();													// Trying out having a Set() function without any parameters. Kinda dirty.

	void			HelpMarker		(const char* description);

private:
	const char*		name;
	bool			is_active;
	bool			is_hovered;
};

#endif // !__E_PANEL_H__