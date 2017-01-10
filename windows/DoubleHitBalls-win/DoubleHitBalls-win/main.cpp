#include "main.h"

#include "main_code.h"

TAndroidApplication* App = NULL;

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{

	int width = 480;
	int height = 320;

	if (CreateEngine(width, height))
	{
		
		
		App = new TAndroidApplication;

		App->OuterInit(width, height, width, height);

		MainLoop(App);
			
		App->OuterDeinit();
		
		delete App;

		App = NULL;

		DestroyEngine();
	}

	return 0;
}