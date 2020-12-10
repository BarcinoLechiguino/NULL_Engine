#ifndef __E_SCENE_H__
#define __E_SCENE_H__

#include "EditorPanel.h"

class E_Scene : public EditorPanel
{
public:
	E_Scene();
	~E_Scene();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:
	void CheckSceneIsClicked();
};

#endif // !__E_SCENE_H__