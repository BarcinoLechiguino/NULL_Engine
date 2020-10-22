#ifndef __E_ENGINE_CONFIGURATION_H__
#define __E_ENGINE_CONFIGURATION_H__

#include "E_Panel.h"

class E_EngineConfiguration : public E_Panel
{
public:
	E_EngineConfiguration();
	~E_EngineConfiguration();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:

};

#endif // !_E_ENGINE_CONFIGURATION_H__