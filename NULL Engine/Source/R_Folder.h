#ifndef __R_FOLDER_H__
#define __R_FOLDER_H__

#include "Resource.h"

class ParsonNode;

class R_Folder : public Resource
{
public:
	R_Folder();
	~R_Folder();

	bool CleanUp() override;

	bool SaveMeta(ParsonNode& meta_root) const override;
	bool LoadMeta(const ParsonNode& meta_root) override;

private:

};

#endif // !_R_FOLDER_H__