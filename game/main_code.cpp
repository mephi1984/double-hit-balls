#include "main_code.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "include/Engine.h"

#include "main_code.h"

TMyApplication* Application;



Matrix3f quatToMatrix(Vector4f q) {

	Matrix3f result;

	double sqw = q(3)*q(3);
	double sqx = q(0)*q(0);
	double sqy = q(1)*q(1);
	double sqz = q(2)*q(2);

	// invs (inverse square length) is only required if quaternion is not already normalised
	double invs = 1 / (sqx + sqy + sqz + sqw);

	result(0,0) = (sqx - sqy - sqz + sqw)*invs; // since sqw + sqx + sqy + sqz =1/invs*invs
	result(1,1) = (-sqx + sqy - sqz + sqw)*invs;
	result(2,2) = (-sqx - sqy + sqz + sqw)*invs;

	double tmp1 = q(0)*q(1);
	double tmp2 = q(2)*q(3);
	result(1, 0) = 2.0 * (tmp1 + tmp2)*invs;
	result(0, 1) = 2.0 * (tmp1 - tmp2)*invs;

	tmp1 = q(0)*q(2);
	tmp2 = q(1)*q(3);
	result(2,0) = 2.0 * (tmp1 - tmp2)*invs;
	result(0,2) = 2.0 * (tmp1 + tmp2)*invs;
	tmp1 = q(1)*q(2);
	tmp2 = q(0)*q(3);
	result(2, 1) = 2.0 * (tmp1 + tmp2)*invs;
	result(1, 2) = 2.0 * (tmp1 - tmp2)*invs;

	return result;
}


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
	ResourceManager->ShaderManager.AddShader("ParallaxShader", "parallax_vertex.txt", "parallax_fragment.txt");
	Renderer->PushShader("DefaultShader");

	ResourceManager->TexList.AddTexture("console_bkg.bmp");
	ResourceManager->TexList.AddTexture("owl-green.jpg");
	ResourceManager->TexList.AddTexture("owl-green-height.png");
	ResourceManager->TexList.AddTexture("owl-green-normal.png");

	ResourceManager->TexList.AddTexture("5f.jpg");
	ResourceManager->TexList.AddTexture("6f.jpg");
	ResourceManager->TexList.AddTexture("7f.jpg");

	ResourceManager->FrameManager.AddFrameRenderBuffer("LevelBuffer", 512, 512);

	/*
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(-511, -512, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(-512, 512, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 512, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(-512, -512, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 512, 0));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, -512, 0));
	*/
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(-511, 0, -512));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(-512, 0, 512));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 0, 512));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(-512, 0, -512));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 0, 512));
	pair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(Vector3f(512, 0, -512));

	pair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0, 0));
	pair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0, 1));
	pair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(1,1));
	pair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0, 0));
	pair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(1,1));
	pair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(1, 0));



	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(1, 0, 0, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(1, 0, 0, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(0, 1, 0, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(0, 0, 1, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(0, 0, 1, 1));
	pair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(Vector4f(0, 1, 0, 1));

	pair.first.ShaderName = "ParallaxShader";
	
	pair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "owl-green.jpg";
	pair.first.SamplerMap["HeightMap"] = "owl-green-height.png";
	pair.first.SamplerMap["NormalMap"] = "owl-green-normal.png";
	//pair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "6f.jpg";
	//pair.first.SamplerMap["HeightMap"] = "5f.jpg";
	//pair.first.SamplerMap["NormalMap"] = "7f.jpg";

	pair.second.RefreshBuffer();



	/*
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


	rect.second.RefreshBuffer();*/



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
	phi += shift(1)*0.02f;

	if (phi < pi/12)
	{
		phi = pi / 12;
	}

	if (phi > pi / 2)
	{
		phi = pi / 2;
	}

	alpha -= shift(0)*0.02f;
}
	
void TMyApplication::OnFling(Vector2f v) 
{
}

void TMyApplication::OnMouseWheel(short int delta)
{
	distance += delta;

	if (distance > 2500)
	{
		distance = 2500;
	}

	if (distance < 100)
	{
		distance = 100;
	}
}
	
void TMyApplication::InnerDraw()
{

	Renderer->SetPerspectiveProjection(pi / 6, 10.f, 10000.f);

	Renderer->SetFullScreenViewport();

	Renderer->PushMatrix();

	Renderer->TranslateMatrix(Vector3f(0, 0, -distance));

	Vector4f quat1 = Vector4f(sin(phi / 2), 0, 0, cos(phi / 2));
	Vector4f quat2 = Vector4f(0, sin(alpha / 2), 0, cos(alpha / 2));


	Renderer->RotateMatrix(quat1);

	Renderer->RotateMatrix(quat2);



	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CheckGlError("");

	auto mat1 = quatToMatrix(quat1);

	auto mat2 = quatToMatrix(quat2);

	Vector3f lightPos = {0.f, 1.f, 1.f};

	Vector3f eye = mat2 * (mat1 * Vector3f(0.0f, 0.f, -distance));


	{
		TRenderParamsSetter params(pair.first);

		RenderUniform3fv("eye", eye.data());
		RenderUniform3fv("lightPos", lightPos.data());

		Matrix3f normMatrix = Renderer->GetModelviewMatrix().inverse().transpose().block<3,3>(0,0);
		
		RenderUniformMatrix3fv("NormalMatrix", false, normMatrix.data());
		RenderUniformMatrix4fv("ModelViewMatrix", false, Renderer->GetModelviewMatrix().data());
		RenderUniformMatrix3fv("ModelViewMatrix3x3", false, Renderer->GetModelviewMatrix().block<3,3>(0,0).data());

		Renderer->DrawTriangleList(pair.second);
	}

	Renderer->PopMatrix();

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
