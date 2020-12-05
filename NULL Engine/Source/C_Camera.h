#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"

class ParsonNode;
class GameObject;

class C_Camera : public Component
{
public:
	C_Camera(GameObject* owner);
	~C_Camera();

	bool Update				() override;
	bool CleanUp			() override;

	bool SaveConfiguration	(ParsonNode& configuration) const override;
	bool LoadConfiguration	(ParsonNode& configuration) override;

private:

};

#endif // !__CAMERA_H__