#ifndef __E_LOAD_FILE_H__
#define __E_LOAD_FILE_H__

#include "EditorPanel.h"

class E_LoadFile : public EditorPanel
{
public:
	E_LoadFile();
	~E_LoadFile();

	bool Draw		(ImGuiIO& io);
	bool CleanUp	();

private:
	void DrawFileBrowser		();
	void DrawFileSelector		();
	
	void DrawDirectoriesTree	(const char* root_directory);

private:
	char*	selected_file;
	bool	ready_to_load;

};

#endif // !__E_LOAD_FILE_H__