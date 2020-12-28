#ifndef __E_PROJECT_H__
#define __E_PROJECT_H__

#include "Icons.h"

#include "EditorPanel.h"

class E_Project : public EditorPanel
{
public:
	E_Project();
	~E_Project();

	bool Draw		(ImGuiIO& io) override;
	bool CleanUp	() override;

private:
	void GenerateDockspace		(ImGuiIO& io) const;										// 
	
	void DrawMenuBar			() const;													// 
	void DrawAssetsTree			();															// 
	void DrawFolderExplorer		() const;													// 

	void DrawDirectoriesTree	(const char* root_node, const char* extension_to_filter);

private:
	char*	directory_to_display;

	Icons	engine_icons;
	bool	icons_are_loaded;
};

#endif // !__E_PROJECT_H__