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
	//ST::PathToResources = "resources/";
	ST::PathToResources = "../../../assets/";
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

	ResourceManager->TexList.AddTexture("background.jpg");
	ResourceManager->TexList.AddTexture("HeightMap.png");
	ResourceManager->TexList.AddTexture("NormalMap.png");
	ResourceManager->TexList.AddTexture("linesAll.png");
	ResourceManager->TexList.AddTexture("clean-fabric-texture-4-780x585.jpg");

	

	ResourceManager->FrameManager.AddFrameRenderBuffer("LevelBuffer", 512, 512);

	Vector2f const bottomLeft(-500, -500);
	float const W = 1000;
	float const H = 1000;

	Vector2f const backgroundBottomLeft(-1000, -1000);
	float const backgroundW = 2000;
	float const backgroundH = 2000;
	
	{
		//resolution of background image
		float const imageW = 512;
		float const imageH = 512;


		background.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(Vector3f(backgroundBottomLeft[0], 0, backgroundBottomLeft[1]));
		background.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(Vector3f(backgroundBottomLeft[0], 0, backgroundBottomLeft[1] + backgroundH));
		background.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(Vector3f(backgroundBottomLeft[0] + backgroundW, 0, backgroundBottomLeft[1] + backgroundH));
		
		background.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(Vector3f(backgroundBottomLeft[0] + backgroundW, 0, backgroundBottomLeft[0] + backgroundH));
		background.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(Vector3f(backgroundBottomLeft[0] + backgroundW, 0, backgroundBottomLeft[0]));
		background.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].emplace_back(Vector3f(backgroundBottomLeft[0], 0, backgroundBottomLeft[0]));

		float const tw = backgroundW / imageW;
		float const th = backgroundH / imageH;

		background.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(0, th));
		background.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(0, 0));
		background.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(tw, 0));
		
		background.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(tw, 0));
		background.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(tw, th));
		background.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].emplace_back(Vector2f(0, th));

		background.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 1, 1));
		background.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 1, 1));
		background.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 1, 1));

		background.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 1, 1));
		background.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 1, 1));
		background.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].emplace_back(Vector4f(1, 1, 1, 1));
	
	}

	{
		//resolution of linesAll.png
		float const  texW = 900;
		float const  texH = 900;

		float const  step = 12;
		float const  thick = 10;


		float const R = 5;
		float const r = 6;
		size_t const threadsCount = 3;
		size_t const verticesCount = 6;
		float const angle = pi / 18;
		size_t const iterationsCount = 50;
		auto g = [this, R, r, threadsCount, verticesCount, angle, iterationsCount, texW, texH, thick, H, W] (const Vector3f &start, const Vector3f &end) {
			Vector3f direction = end - start;
			Vector3f translate = direction / iterationsCount;
			direction.normalize();

			auto e0 = Vector3f(1, 0, -direction.x() / direction.z());
			auto e1 = Vector3f(0, 1, -direction.y() / direction.z());
			e1 = e1 - (e1.dot(e0) / e0.dot(e0)) * e0;
			
			e0.normalize();
			e1.normalize();

			std::vector<std::vector<Vector4f>> threads;
			for(auto i = 0; i < threadsCount; i++) {
				std::vector<Vector4f> vertices;
				Vector3f threadCenter = R * (e0 * cosf(i * 2 * pi / threadsCount) + e1 * sinf(i * 2 * pi / threadsCount));
				for(auto j = 0; j < verticesCount; j++) {
					auto verticeCenter = threadCenter + r * (e0 * cosf(j * 2 * pi / verticesCount) + e1 * sinf(j * 2 * pi / verticesCount));
					vertices.push_back(Vector4f(verticeCenter.x(), verticeCenter.y(), verticeCenter.z(), 1));
				}

				threads.push_back(vertices);
			}
			
		    auto transform = Translation3f(translate) * AngleAxis<float>(angle, direction);
			auto matrix = transform.matrix();
			
			for(auto i = 0; i < iterationsCount; i++) {
				std::vector<std::vector<Vector4f>> newThreads;

				for(auto j = 0; j < threadsCount; j++) {
					auto vertices = threads[j];
					std::vector<Vector4f> newVertices;

					for(auto k = 0; k < verticesCount; k++) {
						newVertices.push_back(matrix * vertices[k]);
					}

					newThreads.push_back(newVertices);

					for(auto k = 0; k < verticesCount; k++) {
						auto vk = vertices[k];
						auto vk1 = vertices[(k + 1) % verticesCount];
						auto nvk = newVertices[k];
						auto nvk1 = newVertices[(k + 1) % verticesCount];

						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + vk.head(3));
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + vk1.head(3));
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + nvk.head(3));
						
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + vk1.head(3));
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + nvk1.head(3));
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + nvk.head(3));
					
						auto texThick = thick / texW;
						auto m = (start[0] + end[0]) / 2 / texW;
						auto y = H / texH;

						auto texPiece = texH / iterationsCount;

						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0, 0));
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0.01, 0));
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0, 0.01));
						
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0.01, 0));
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0.01, 0.01));
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0, 0.01));
					}
				}

				threads = newThreads;
			}
		};

		Vector3f const stepDirection(18, 0, 0);
		Vector3f p1(bottomLeft[0], 0, -bottomLeft[1]);
		Vector3f p2 = p1 - Vector3f(0, 0, W);

		while(p1[0] < bottomLeft[0] + W)
		{
			g(p1, p2);
			p1 += stepDirection;
			p2 += stepDirection;
		}
	}

	background.first.ShaderName ="DefaultShader";
	fabricRender.first.ShaderName = "ParallaxShader";
	
	/*
	 *	Line below should be in tes-engine/include/ShaderManager/ShaderManager.h
	 */
	std::string const CONST_STRING_HEIGHTMAP_UNIFORM = "HeightMap";

	background.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "background.jpg";

	fabricRender.first.SamplerMap[CONST_STRING_NORMALMAP_UNIFORM] = "NormalMap.png";
	fabricRender.first.SamplerMap[CONST_STRING_HEIGHTMAP_UNIFORM] = "HeightMap.png";
	fabricRender.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "clean-fabric-texture-4-780x585.jpg";

	background.second.RefreshBuffer();
	fabricRender.second.RefreshBuffer();

	glEnable(GL_DEPTH_TEST);
	
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

	Vector3f eye = mat2 * mat1 * Vector3f(0.0f, 0.f, -distance);

	{
		TRenderParamsSetter params(background.first);

		RenderUniform3fv("eye", eye.data());
		RenderUniform3fv("lightPos", lightPos.data());

		Matrix3f normMatrix = Renderer->GetModelviewMatrix().inverse().transpose().block<3, 3>(0, 0);

		RenderUniformMatrix3fv("NormalMatrix", false, normMatrix.data());
		RenderUniformMatrix4fv("ModelViewMatrix", false, Renderer->GetModelviewMatrix().data());
		RenderUniformMatrix3fv("ModelViewMatrix3x3", false, Renderer->GetModelviewMatrix().block<3, 3>(0, 0).data());

		Renderer->DrawTriangleList(background.second);
	}

	//glEnable(GL_CULL_FACE);
	{
		TRenderParamsSetter params(fabricRender.first);

		RenderUniform3fv("eye", eye.data());
		RenderUniform3fv("lightPos", lightPos.data());

		Matrix3f normMatrix = Renderer->GetModelviewMatrix().inverse().transpose().block<3,3>(0,0);
		
		RenderUniformMatrix3fv("NormalMatrix", false, normMatrix.data());
		RenderUniformMatrix4fv("ModelViewMatrix", false, Renderer->GetModelviewMatrix().data());
		RenderUniformMatrix3fv("ModelViewMatrix3x3", false, Renderer->GetModelviewMatrix().block<3,3>(0,0).data());

		Renderer->DrawTriangleList(fabricRender.second);		
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
