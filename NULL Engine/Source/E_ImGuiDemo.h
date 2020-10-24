#ifndef __E_TEST_H__
#define __E_TEST_H__

#include "E_Panel.h"

class E_ImGuiDemo : public E_Panel
{
public:
	E_ImGuiDemo();
	~E_ImGuiDemo();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:
	bool			show_demo_window;
	bool			show_another_window;

	int				current_style;
	const char*		styles;

	float			f;
	int				counter;
};

#endif // !_E_TEST_H__