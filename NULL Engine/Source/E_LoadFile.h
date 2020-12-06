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


};

#endif // !__E_LOAD_FILE_H__