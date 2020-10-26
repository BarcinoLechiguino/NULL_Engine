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
	bool ConfigurationMenuBar();										// Will generate the elements of the configuration window's toolbar.
	
	bool ApplicationMenu();												// Menu for the Application module.
	bool WindowMenu();													// Menu for the Window module.
	bool RendererMenu();												// Menu for the Renderer3D module.
	bool CameraMenu();													// Menu for the Camera3D module.
	bool InputMenu();													// Menu for the Input module.
	bool FileSystemMenu();												// Menu for the FileSystem module.
	bool TexturesMenu();												// 
	bool AudioMenu();													// 
	bool PhysicsMenu();													// 
	bool HardwareMenu();												// Menu that will display the hardware state of the system that is running the application.

	// ------- MENU METHODS -------
	void PlotFrameDataHistogram();										// Application menu: Will plot the frame data histogram according to the current frame data values.
	void GenerateFrameCapSlider();										// Application menu: Will generate a slider that will modify the frame cap value. From 0 (no cap) to 60.

	void RendererFlags();												// Renderer menu: Will generate the checkboxes that will modify the renderer flags.

	void InputLogOutput();												// Input menu: Will generate the text of the Log. The logs are stored in the input_logs std::vector.
	void ReduceInputLog();												// Input menu: Will reduce the size of the input_logs vector down to a more manageable size, memory and framerate wise.
	void InputLogScrollToBottom();										// Input menu: Will scroll down to the bottommost part of the Log text whenever it is called.
	void ClearInputLog();												// Input menu: Will delete every log and empty the input_logs std::vector.

private:
	float FPS_data[MAX_HISTOGRAM_SIZE];
	float ms_data[MAX_HISTOGRAM_SIZE];

	std::vector<char*> input_logs;
	bool input_log_scroll_to_bottom;
};

#endif // !_E_ENGINE_CONFIGURATION_H__