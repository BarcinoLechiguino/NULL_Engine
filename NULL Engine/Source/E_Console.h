#ifndef __E_CONSOLE_H__
#define __E_CONSOLE_H__

#include "E_Panel.h"

class E_Console : public E_Panel
{
public:
	E_Console();
	~E_Console();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:

};

#endif // !__E_CONSOLE_H__