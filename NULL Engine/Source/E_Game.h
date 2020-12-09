#ifndef __E_GAME_H__
#define __E_GAME_H__

#include "EditorPanel.h"

class E_Game : public EditorPanel
{
public:
	E_Game();
	~E_Game();

	bool Draw		(ImGuiIO& io) override;
	bool CleanUp	() override;

private:


};

#endif // !__E_GAME_H__