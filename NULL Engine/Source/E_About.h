#ifndef __E_ABOUT_H__
#define __E_ABOUT_H__

#include "E_Panel.h"

class E_About : public E_Panel
{
public:
	E_About();
	~E_About();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:
	int a;
};

#endif // !__E_ABOUT_H__