#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "Component.h"

class GameObject;

class C_Mesh : public Component
{
public:
	C_Mesh(GameObject* owner);
	~C_Mesh();

	bool Update() override;
	bool CleanUp() override;

private:

};

#endif // !__C_MESH_H__