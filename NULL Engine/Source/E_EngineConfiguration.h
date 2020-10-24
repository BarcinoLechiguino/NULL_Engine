#ifndef __E_ENGINE_CONFIGURATION_H__
#define __E_ENGINE_CONFIGURATION_H__

#include "E_Panel.h"

#define MAX_HISTOGRAM_SIZE 100
#define MAX_INPUT_LOG_SIZE 100000

class E_EngineConfiguration : public E_Panel
{
public:
	E_EngineConfiguration();
	~E_EngineConfiguration();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

public:
	void UpdateFrameData(int frames, int ms);

	void AddInputLog(const char* log);

private:
	bool ConfigurationMenuBar();
	
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

	// ------- MENU METHODS -------
	void PlotFrameDataHistogram();										// Application menu: Will plot the frame data histogram according to the current frame data values.
	void GenerateFrameCapSlider();										// Application menu: Will generate a slider that will modify the frame cap value. From 0 (no cap) to 60.

	void InputLogOutput();
	void ReduceInputLog();
	void InputLogScrollToBottom();
	void ClearInputLog();

private:
	float FPS_data[MAX_HISTOGRAM_SIZE];
	float ms_data[MAX_HISTOGRAM_SIZE];

	std::vector<char*> input_logs;
	bool input_log_scroll_to_bottom;
};

#endif // !_E_ENGINE_CONFIGURATION_H__