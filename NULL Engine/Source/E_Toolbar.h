#ifndef __E_TOOLBAR_H__
#define __E_TOOLBAR_H__

#include "E_Panel.h"

class E_Toolbar : public E_Panel
{
public:
	E_Toolbar();
	~E_Toolbar();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:
	bool show_close_app_popup;
	bool show_about_popup;
};

#endif // !__E_TOOLBAR_H__