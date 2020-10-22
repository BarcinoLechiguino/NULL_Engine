#ifndef __E_CONSOLE_H__
#define __E_CONSOLE_H__

#include "E_Panel.h"

class E_Console : public E_Panel
{
public:
	E_Console();
	~E_Console();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

public:
	void AddLog(const char* log);
	
private:
	void ClearLog();

	void ConsoleMenuBar();
	void ConsoleOutput();
	void ConsoleScrollToBottom();
	
private:
	std::vector<char*> logs;

	bool scroll_to_bottom;
	bool show_console;
};

#endif // !__E_CONSOLE_H__