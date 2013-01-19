#include "include/Engine.h"
#include "main_code.h"

TAndroidApplication* App = NULL;


extern "C" void AppInit()
{
    App = new TAndroidApplication;
    //App->OuterInit(320, 480, 320, 480);
    App->OuterInit(480, 320, 480, 320);
}


extern "C" void AppDeinit()
{
    App->OuterDeinit();
    delete App;
}


extern "C" void AppUpdate(int dt)
{
    App->OuterUpdate(dt);
}



extern "C" void AppDraw()
{
    App->OuterDraw();
}

extern "C" void AppOnTapDown(int posx, int posy)
{
    App->OuterOnTapDown(vec2(posx, posy));
}

extern "C" void AppOnTapUp(int posx, int posy)
{
    App->OuterOnTapUp(vec2(posx, posy));
}

extern "C" void AppOnScroll(int shiftx, int shifty)
{
    App->OuterOnMove(vec2(shiftx, shifty));
}

