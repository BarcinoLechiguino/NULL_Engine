#ifndef __C_ANIMATION_H__
#define __C_ANIMATION_H__

#include "Component.h"

class ParsonNode;
class GameObject;

class C_Animation : public Component
{
public:
	C_Animation(GameObject* owner);
	~C_Animation();

	bool Update() override;
	bool CleanUp() override;

	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::ANIMATION; }

private:

};

#endif // !__C_ANIMATION_H__