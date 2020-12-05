#ifndef __C_LIGHT_H__
#define __C_LIGHT_H__

#include "Component.h"

class ParsonNode;
class GameObject;

class C_Light : public Component
{
public:
	C_Light(GameObject* owner);
	~C_Light();

	bool Update		() override;
	bool CleanUp	() override;

	bool SaveConfiguration(ParsonNode& configuration) const override;
	bool LoadConfiguration(ParsonNode& configuration) override;

private:

};

#endif // !__C_LIGHT_H__