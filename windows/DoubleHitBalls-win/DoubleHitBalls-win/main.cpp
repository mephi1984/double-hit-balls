#include "main.h"
#include "main_code.h"

TAndroidApplication* App = NULL;

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
	//Create application
	
	TAndroidApplication Application;
	Application.Width = 480;
	Application.Height = 320;

	App = &Application;

	//Start application
	return MainLoop(Application);
}