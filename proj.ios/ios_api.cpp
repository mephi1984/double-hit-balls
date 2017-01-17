#include "include/Engine.h"
#include "main_code.h"


extern "C" void CustomAppInit()
{
    AppInit<TAndroidApplication>(480, 320);
}
