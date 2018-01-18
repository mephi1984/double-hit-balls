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
	ResourceManager->TexList.AddTexture("fiber_texture.jpg");

	ResourceManager->FrameManager.AddFrameRenderBuffer("LevelBuffer", 512, 512);

	auto addRectInData = [this] (const Vector3f &a, const Vector3f &b, const Vector3f &c, const Vector3f &d) {

		fabricRenderPair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(a);
		fabricRenderPair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(b);
		fabricRenderPair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(c);

		fabricRenderPair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(c);
		fabricRenderPair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(d);
		fabricRenderPair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(a);

		fabricRenderPair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(0, 0));
		fabricRenderPair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(1, 0));
		fabricRenderPair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(1, 1));

		fabricRenderPair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(1, 1));
		fabricRenderPair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(0, 1));
		fabricRenderPair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(0, 0));


		//it is true that it does nothing? :thinking:
		fabricRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 1, 1));
		fabricRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(0, 1, 1, 1));
		fabricRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 0, 1));

		fabricRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 0, 1));
		fabricRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(0, 0, 0, 1));
		fabricRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 1, 1));
		//end of :thinking:
	};

	auto genRect = [] (Vector3f &a, Vector3f &b, float thick) {
		Vector3f AB = b - a;
		Vector3f ortho(AB[2], 0, -AB[0]);
		ortho.normalize();
		thick /= 2;
		Vector3f r1 = a - ortho * thick;
		Vector3f r2 = a + ortho * thick;
		Vector3f r3 = b + ortho * thick;
		Vector3f r4 = b - ortho * thick;
		return std::make_tuple(r1, r2, r3, r4);
	};

	int const n = 1000;
	float const stripeLength = 100;
	float const stepOnAsix = 7;
	float const thick = 5;


	Vector3f stripeDirection = {0, 0, -1};
	Vector3f asix = {1, 0, 0};
	Vector3f p1 = {-512, 0, 0};

	Vector3f a, b, c, d;

	for (int i = 0; i < n; ++i) {

		Vector3f p2 = p1 + stripeDirection * stripeLength;

		std::tie(a, b, c, d) = genRect(p1, p2, thick);
		addRectInData(a, b, c, d);

		Vector3f p3 = p1 + stepOnAsix * asix;

		std::tie(a, b, c, d) = genRect(p2, p3, thick);
		addRectInData(a, b, c, d);

		p1 = p3;
	}


	fabricRenderPair.first.ShaderName = "ParallaxShader";
	
	/*
	 *	Line below should be in tes-engine/include/ShaderManager/ShaderManager.h
	 */
	std::string const CONST_STRING_HEIGHTMAP_UNIFORM = "HeightMap";

	fabricRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "fiber_texture.jpg";
	//fabricRenderPair.first.SamplerMap[CONST_STRING_HEIGHTMAP_UNIFORM] = "owl-green-height.png";
	//fabricRenderPair.first.SamplerMap[CONST_STRING_NORMALMAP_UNIFORM] = "owl-green-normal.png";

	fabricRenderPair.second.RefreshBuffer();


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
		TRenderParamsSetter params(fabricRenderPair.first);

		RenderUniform3fv("eye", eye.data());
		RenderUniform3fv("lightPos", lightPos.data());

		Matrix3f normMatrix = Renderer->GetModelviewMatrix().inverse().transpose().block<3,3>(0,0);
		
		RenderUniformMatrix3fv("NormalMatrix", false, normMatrix.data());
		RenderUniformMatrix4fv("ModelViewMatrix", false, Renderer->GetModelviewMatrix().data());
		RenderUniformMatrix3fv("ModelViewMatrix3x3", false, Renderer->GetModelviewMatrix().block<3,3>(0,0).data());

		Renderer->DrawTriangleList(fabricRenderPair.second);
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
