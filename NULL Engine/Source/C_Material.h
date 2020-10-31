#ifndef __C_MATERIAL_H__
#define __C_MATERIAL_H__

#include "Component.h"

class GameObject;

class C_Material : public Component
{
public:
	C_Material(GameObject* owner);
	~C_Material();

	bool Update() override;
	bool CleanUp() override;

private:

};

#endif // !__C_MATERIAL_H__