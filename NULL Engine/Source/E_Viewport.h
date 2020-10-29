#ifndef __E_VIEWPORT_H__
#define __E_VIEWPORT_H__

#include "E_Panel.h"

class E_Viewport : public E_Panel
{
public:
	E_Viewport();
	~E_Viewport();

	bool Draw(ImGuiIO& io) override;
	
	bool CleanUp() override;

private:

};

#endif // !__E_VIEWPORT_H__