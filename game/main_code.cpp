#include "main_code.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "include/Engine.h"

TMyApplication* Application;


void TMyApplication::InnerInit()
{

    Application = this;
    
#ifdef TARGET_WIN32
#ifdef NDEBUG
	ST::PathToResources = "resources/";
#else
	ST::PathToResources = "../../../assets/";
#endif
#endif
    
#ifdef TARGET_IOS
    ST::PathToResources = "assets/";
#endif

    if (Console != NULL)
    {
        *Console<<"APP INIT\n";
    }
    srand (static_cast<size_t>(time(NULL)));

	ResourceManager->ShaderManager.AddShader("DefaultShader", "texture-shader.vertex", "texture-shader.fragment");
	Renderer->PushShader("DefaultShader");

	ResourceManager->FrameManager.AddFrameRenderBuffer("FrameBuffer", Renderer->GetMatrixWidth(), Renderer->GetMatrixHeight());

	Renderer->SetOrthoProjection();
	
}

void TMyApplication::InnerDeinit()
{
}

void TMyApplication::InnerOnTapDown(Vector2f p)
{
}

void TMyApplication::InnerOnTapUp(Vector2f p)
{
}

void TMyApplication::InnerOnTapUpAfterMove(Vector2f p)
{
}

void TMyApplication::InnerOnMove(Vector2f p, Vector2f shift)
{
}
	
void TMyApplication::InnerDraw()
{
	glDisable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


}


void TMyApplication::InnerUpdate(size_t dt)
{
}