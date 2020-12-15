#ifndef __E_RESOURCES_H__
#define __E_RESOURCES_H__

#include "EditorPanel.h"

class E_Resources : public EditorPanel
{
public:
	E_Resources();
	~E_Resources();

	bool Draw(ImGuiIO& io);
	bool CleanUp();

private:

};

#endif // !__E_RESOURCES_H__