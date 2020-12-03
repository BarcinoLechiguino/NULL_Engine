#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"

class Configuration;
class GameObject;

class C_Camera : public Component
{
public:
	C_Camera(GameObject* owner);
	~C_Camera();

	bool Update				() override;
	bool CleanUp			() override;

	bool SaveConfiguration	(Configuration& configuration) const override;
	bool LoadConfiguration	(Configuration& configuration) override;

private:

};

#endif // !__CAMERA_H__