#include "main_code.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "include/Engine.h"

#include <boost/property_tree/json_parser.hpp>

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
	ResourceManager->ShaderManager.AddShader("ColorShader", "color-shader.vertex", "color-shader.fragment");
	ResourceManager->ShaderManager.AddShader(ParticleEffect::PARTICLE_SHADER, "particle-shader.vertex", "particle-shader.fragment");
	Renderer->PushShader("ColorShader");

	float width = Renderer->GetScreenWidth();
	float height = Renderer->GetScreenHeight();

	Renderer->SetOrthoProjection();
	Renderer->PushProjectionMatrix(width, height, -1, 1);
	//Renderer->SetFullScreenViewport();

	boost::property_tree::ptree JSONsource;
	boost::property_tree::json_parser::read_json(ST::PathToResources + "test.json", JSONsource);

	sparkler.parse(JSONsource); // parse JSON

	sparkler.load(); // load textures

	sparkler.setCoords({ width / 2, height / 2, 0 });
}

void TMyApplication::InnerDeinit()
{
}

void TMyApplication::InnerOnMouseDown(TMouseState& mouseState)
{
	if (mouseState.LeftButtonPressed)
	{
		sparkler.setCoords({ (float)mouseState.X, (float)mouseState.Y, 0 });
	}
	else if (mouseState.RightButtonPressed)
	{
		if (sparkler.isSpawning())
		{
			sparkler.stopSpawn();
		}
		else
		{
			sparkler.startSpawn();
		}
	}
}

void TMyApplication::InnerOnMove(Vector2f p, Vector2f shift)
{
	sparkler.setCoords({ p[0], p[1], 0 });
}
	
void TMyApplication::InnerDraw()
{
	glDisable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	sparkler.draw();
}


void TMyApplication::InnerUpdate(size_t dt)
{
	if (dt > 50)
	{
		dt = 50;
	}

	sparkler.update(dt / 1000.f);
}