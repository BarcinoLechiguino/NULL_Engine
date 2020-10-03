#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum class MAIN_STATUS
{
	CREATION,
	START,
	UPDATE,
	FINISH,
	EXIT
};

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	MAIN_STATUS state = MAIN_STATUS::CREATION;
	Application* App = NULL;

	while (state != MAIN_STATUS::EXIT)
	{
		switch (state)
		{
		case MAIN_STATUS::CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_STATUS::START;
			break;

		case MAIN_STATUS::START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_STATUS::EXIT;
			}
			else
			{
				state = MAIN_STATUS::UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_STATUS::UPDATE:
		{
			UPDATE_STATUS update_return = App->Update();							// THIS HERE

			if (update_return == UPDATE_STATUS::THROW_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_STATUS::EXIT;
			}

			if (update_return == UPDATE_STATUS::STOP)
			{
				state = MAIN_STATUS::FINISH;
			}
		}
			break;

		case MAIN_STATUS::FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
			{
				main_return = EXIT_SUCCESS;
			}

			state = MAIN_STATUS::EXIT;

			break;

		}
	}

	delete App;
	LOG("Exiting game '%s'...\n", TITLE);
	return main_return;
}