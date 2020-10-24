#ifndef __E_INSPECTOR_H__
#define __E_INSPECTOR_H__

#include "E_Panel.h"

class E_Inspector : public E_Panel
{
public:
	E_Inspector();
	~E_Inspector();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:
	int a;
};

#endif // !__E_INSPECTOR_H__