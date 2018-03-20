#include "main.h"

#include "main_code.h"

TMyApplication* App = NULL;

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{

	int width = 800;
	int height = 480;


	if (CreateEngine(width, height))
	{
		
		App = new TMyApplication;

		App->OuterInit(width, height, width, height);

		MainLoop(App);

		App->OuterDeinit();

		delete App;

		App = NULL;

		DestroyEngine();
	}

	return 0;
}