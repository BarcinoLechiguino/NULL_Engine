#ifndef __E_VIEWPORT_H__
#define __E_VIEWPORT_H__

#include "EditorPanel.h"

class E_Viewport : public EditorPanel
{
public:
	E_Viewport();
	~E_Viewport();

	bool Draw(ImGuiIO& io) override;
	
	bool CleanUp() override;

private:

};

#endif // !__E_VIEWPORT_H__