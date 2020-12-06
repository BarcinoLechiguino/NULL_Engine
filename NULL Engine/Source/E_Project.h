#ifndef __E_PROJECT_H__
#define __E_PROJECT_H__

#include "EditorPanel.h"

class E_Project : public EditorPanel
{
public:
	E_Project();
	~E_Project();

	bool Draw		(ImGuiIO& io) override;
	bool CleanUp	() override;

private:


};

#endif // !__E_PROJECT_H__