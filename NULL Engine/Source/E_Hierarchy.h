#ifndef __E_HIERARCHY_H__
#define __E_HIERARCHY_H__

#include "E_Panel.h"

class E_Hierarchy : public E_Panel
{
public:
	E_Hierarchy();
	~E_Hierarchy();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:
	int a;
};

#endif // !__E_HIERARCHY_H__