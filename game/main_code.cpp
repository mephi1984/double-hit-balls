#include "main_code.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "include/Engine.h"

#include "main_code.h"

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

	ResourceManager->ShaderManager.AddShader("DefaultShader", "shader1vertex.txt", "shader1fragment.txt");
	ResourceManager->ShaderManager.AddShader("FrameShader", "frameshader_vertex.txt", "frameshader_fragment.txt");
	ResourceManager->ShaderManager.AddShader("ColorShader", "color_vertex.txt", "color_fragment.txt");
	ResourceManager->ShaderManager.AddShader("SSAA_4X", "SSAA_4X.vertex", "SSAA_4X.frag");
	Renderer->PushShader("DefaultShader");

	ResourceManager->TexList.AddTexture("console_bkg.bmp");

	ResourceManager->FrameManager.AddFrameRenderBuffer("LevelBuffer", 512, 512);

	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(0, 0, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(0, 512, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 512, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(0, 0, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 512, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 0, 0));
	

	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(1, 0, 0, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(1, 0, 0, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(0, 1, 0, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(0, 0, 1, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(0, 0, 1, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(0, 1, 0, 1));

	pair.first.ShaderName = "ColorShader";

	pair.second.RefreshBuffer();




	rect.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(0, 0, 0));
	rect.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(0, 512, 0));
	rect.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 512, 0));
	rect.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 512, 0));
	rect.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 0, 0));
	rect.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(0, 0, 0));

	rect.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0, 0));
	rect.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0, 1));
	rect.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(1, 1));
	rect.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(1, 1));
	rect.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(1, 0));
	rect.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0, 0));


	rect.second.RefreshBuffer();

	Renderer->SetOrthoProjection();

	Renderer->SetFullScreenViewport();

	Inited = true;



}

void TMyApplication::InnerDeinit()
{
    Inited = false;
    Loaded = false;
    if (Console != NULL)
    {
        *Console<<"APP DEINIT\n";
    }

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
	
void TMyApplication::OnFling(Vector2f v) 
{
}

	
void TMyApplication::InnerDraw()
{

	Renderer->SwitchToFrameBuffer("LevelBuffer");
	Renderer->SetProjectionMatrix(512.f, 512.f);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CheckGlError("");


	{
		TRenderParamsSetter params(pair.first);
		Renderer->DrawTriangleList(pair.second);
	}

	CheckGlError("");


	Renderer->SwitchToScreen();
	Renderer->SetFullScreenViewport();


	const float cos30 = sqrt(3) / 2;
	const float sin30 = 0.5f;
	const float sampleRadiusX = 0.75 / 512;
	const float sampleRadiusY = 0.75 / 512;

	Matrix2f rotate30Matrix;
	rotate30Matrix(0, 0) = cos30;
	rotate30Matrix(0, 1) = sin30;
	rotate30Matrix(1, 0) = -sin30;
	rotate30Matrix(1, 1) = cos30;


	Renderer->PushShader("SSAA_4X");


	std::array<Vector2f, 4> sampleVector = {
		Vector2f(sampleRadiusX, sampleRadiusY),
		Vector2f(-sampleRadiusX, sampleRadiusY),
		Vector2f(-sampleRadiusX, -sampleRadiusY),
		Vector2f(sampleRadiusX, -sampleRadiusY)
	};

	for (size_t i = 0; i < 4; i++)
	{
		sampleVector[i] = rotate30Matrix * sampleVector[i];

		RenderUniform2fv("samplesOffset[" + boost::lexical_cast<std::string>(i) + "]", &sampleVector[i][0]);
	}
	
	
	glBindTexture(GL_TEXTURE_2D, ResourceManager->FrameManager.GetFrameTexture("LevelBuffer"));
	Renderer->DrawTriangleList(rect.second);

	Renderer->PopShader();

	CheckGlError("");

}


void TMyApplication::InnerUpdate(size_t dt)
{

}

bool TMyApplication::IsLoaded()
{
    return Loaded;
}

bool TMyApplication::IsInited()
{
    return Inited;
}
