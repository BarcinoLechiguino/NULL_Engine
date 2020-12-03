#ifndef __C_LIGHT_H__
#define __C_LIGHT_H__

#include "Component.h"

class Configuration;
class GameObject;

class C_Light : public Component
{
public:
	C_Light(GameObject* owner);
	~C_Light();

	bool Update		() override;
	bool CleanUp	() override;

	bool SaveConfiguration(Configuration& configuration) const override;
	bool LoadConfiguration(Configuration& configuration) override;

private:

};

#endif // !__C_LIGHT_H__