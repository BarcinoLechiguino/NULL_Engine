#ifndef __R_ANIMATION_H__
#define __R_ANIMATION_H__

#include "Resource.h"
#include "AnimationSettings.h"

class ParsonNode;

class R_Animation : public Resource
{
public:
	R_Animation();
	~R_Animation();

	bool CleanUp();

	bool SaveMeta(ParsonNode& meta_root) const override;
	bool LoadMeta(const ParsonNode& meta_root) override;

private:
	AnimationSettings animation_settings;
};

#endif // !__R_ANIMATION_H__