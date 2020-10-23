#ifndef __E_ENGINE_CONFIGURATION_H__
#define __E_ENGINE_CONFIGURATION_H__

#include "E_Panel.h"

#define MAX_HISTOGRAM_SIZE 100

class E_EngineConfiguration : public E_Panel
{
public:
	E_EngineConfiguration();
	~E_EngineConfiguration();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

public:
	void UpdateFrameData(int frames, int ms);

private:
	bool ApplicationMenu();
	bool WindowMenu();
	bool RendererMenu();
	bool CameraMenu();
	bool InputMenu();
	bool FileSystemMenu();
	bool TexturesMenu();
	bool AudioMenu();
	bool PhysicsMenu();
	bool HardwareMenu();

private:
	float FPS_data[MAX_HISTOGRAM_SIZE];
	float ms_data[MAX_HISTOGRAM_SIZE];
};

#endif // !_E_ENGINE_CONFIGURATION_H__