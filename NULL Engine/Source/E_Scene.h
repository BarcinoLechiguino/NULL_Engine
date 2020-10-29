#ifndef __E_SCENE_H__
#define __E_SCENE_H__

#include "E_Panel.h"

class E_Scene : public E_Panel
{
public:
	E_Scene();
	~E_Scene();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:

};

#endif // !__E_SCENE_H__