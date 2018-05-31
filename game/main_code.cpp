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
	ResourceManager->ShaderManager.AddShader("SimpleShading", "simple_shading.vertex", "simple_shading.fragment");
	Renderer->PushShader("DefaultShader");

	ResourceManager->TexList.AddTexture("console_bkg.bmp");

	ResourceManager->TexList.AddTexture("background.jpg");
	ResourceManager->TexList.AddTexture("pimgpsh.jpg");
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

	auto findPlaneBasis = [] (const Vector3f &normal) {
		std::vector<Vector3f> result;

		Vector3f e0, e1;
		if(normal.z() != 0)
		{
			e0 = Vector3f(1, 0, -normal.x() / normal.z()).normalized();
			e1 = Vector3f(0, 1, -normal.y() / normal.z());
		}
		else if(normal.y() != 0)
		{
			e0 = Vector3f(1, -normal.x() / normal.y(), 0).normalized();
			e1 = Vector3f(0, -normal.z() / normal.y(), 1);
		}
		else
		{
			e0 = Vector3f(-normal.y() / normal.x(), 1, 0).normalized();
			e1 = Vector3f(-normal.z() / normal.x(), 0, 1);
		}

		e1 = (e1 - (e1.dot(e0) / e0.dot(e0)) * e0).normalized();

		result.push_back(e0);
		result.push_back(e1);

		return result;
	};

	{
		float const R = 3;
		float const r = 4;
		size_t const threadsCount = 3;
		size_t const edgesCount = 6;
		float const angle = pi / 6;
		Vector3f up(0, 1, 0); up.normalize();
		size_t const step = 5;
		auto g = [this, findPlaneBasis, R, r, threadsCount, edgesCount, up, angle, step] (Vector3f start, Vector3f end, const Vector3f &color) {
			Vector3f direction = (end - start).normalized();
			end = end - (r + R) * direction;
			size_t iterationsCount = (end - start).norm() / step;
			auto e = findPlaneBasis(up);

			std::vector<Vector3f> threadCenters;
			std::vector<std::vector<Vector4f>> threads;
			for(auto i = 0; i < threadsCount; i++) {
				std::vector<Vector4f> edges;
				Vector3f threadCenter = R * (e[0] * cosf(i * 2 * pi / threadsCount) + e[1] * sinf(i * 2 * pi / threadsCount));
				threadCenters.push_back(threadCenter);

				for(auto j = 0; j < edgesCount; j++) {
					auto verticeCenter = threadCenter + r * (e[0] * cosf(j * 2 * pi / edgesCount) + e[1] * sinf(j * 2 * pi / edgesCount));
					edges.push_back(Vector4f(verticeCenter.x(), verticeCenter.y(), verticeCenter.z(), 1));
				}

				threads.push_back(edges);
			}

			auto rotateMatrix1 = (
				Translation3f((r + R) * direction) *
				AngleAxis<float>(
					-pi / 8,
					direction.cross(up).normalized()
				) *
				Translation3f(-(r + R) * direction)
			).matrix();
			auto matrix = (
				Translation3f((r + R) * up) *
				AngleAxis<float>(
					angle,
					direction
				) *
				Translation3f(-(r + R) * up) *
				Translation3f(static_cast<int>(step) * direction)
			).matrix();
			auto rotateMatrix2 = (
				Translation3f(static_cast<int>(step) * direction * iterationsCount) *
				Translation3f((r + R) * direction) *	
				AngleAxis<float>(
					-pi / 8,
					direction.cross(up).normalized()
					) *
				Translation3f(-(r + R) * direction) *
				Translation3f(-static_cast<int>(step) * direction * iterationsCount)
			).matrix();

			for(int i = -4; i < static_cast<int>(iterationsCount) + 4; i++) {
				std::vector<std::vector<Vector4f>> newThreads;

				for(auto j = 0; j < threadsCount; j++) {
					auto edges = threads[j];
					std::vector<Vector4f> newEdges;

					for(auto k = 0; k < edgesCount; k++) {
						if(i < 0) {
							newEdges.push_back(rotateMatrix1 * edges[k]);
						} else if(i < iterationsCount) {
							newEdges.push_back(matrix * edges[k]);
						} else {
							newEdges.push_back(rotateMatrix2 * edges[k]);
						}
					}

					newThreads.push_back(newEdges);

					auto threadCenter_ = Vector4f(threadCenters[j].x(), threadCenters[j].y(), threadCenters[j].z(), 1);
					auto threadCenter = threadCenter_.head(3);

					Vector3f newThreadCenter;
					if (i < 0) {
						newThreadCenter = (rotateMatrix1 * threadCenter_).head(3);
					} else if (i < iterationsCount) {
						newThreadCenter = (matrix * threadCenter_).head(3);
					} else {
						newThreadCenter = (rotateMatrix2 * threadCenter_).head(3);
					}

					threadCenters[j] = newThreadCenter;
					
					for(auto k = 0; k < edgesCount; k++) {
						auto vk = edges[k].head(3);
						auto vk1 = edges[(k + 1) % edgesCount].head(3);
						auto nvk = newEdges[k].head(3);
						auto nvk1 = newEdges[(k + 1) % edgesCount].head(3);

						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + vk);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + vk1);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + nvk);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + vk1);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + nvk1);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(start + nvk);

						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB].push_back((vk - threadCenter));
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB].push_back((vk1 - threadCenter));
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB].push_back((nvk - newThreadCenter));
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB].push_back((vk1 - threadCenter));
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB].push_back((nvk1 - newThreadCenter));
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB].push_back((nvk - newThreadCenter));

						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0.1, 0.1));
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0.2, 0.1));
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0.1, 0.2));
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0.2, 0.1));
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0.2, 0.2));
						fabricRender.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(Vector2f(0.1, 0.2));

						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(color);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(color);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(color);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(color);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(color);
						fabricRender.second.Data.Vec3CoordArr[CONST_STRING_COLOR_ATTRIB].push_back(color);

					}
				}

				threads = newThreads;
			}
		};

		{
			namespace pt = boost::property_tree;
			pt::ptree root;
			pt::read_json(ST::PathToResources + "lines.json", root);

			for(auto line: root.get_child("lines")) {
				std::vector<int> start;
				std::vector<int> end;
				std::vector<float> color;

				for(auto value: line.second.get_child("start")) {
					start.push_back(value.second.get_value<int>());
				}

				for (auto value : line.second.get_child("end")) {
					end.push_back(value.second.get_value<int>());
				}

				for (auto value : line.second.get_child("color")) {
					color.push_back(value.second.get_value<float>());
				}

				g(Vector3f(start[0], 0, start[1]), Vector3f(end[0], 0, end[1]), Vector3f(color[0], color[1], color[2]));
			}

		}
	}

	background.first.ShaderName ="DefaultShader";
	fabricRender.first.ShaderName = "SimpleShading";
	
	/*
	 *	Line below should be in tes-engine/include/ShaderManager/ShaderManager.h
	 */
	std::string const CONST_STRING_HEIGHTMAP_UNIFORM = "HeightMap";

	background.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "pimgpsh.jpg";

	fabricRender.first.SamplerMap[CONST_STRING_NORMALMAP_UNIFORM] = "NormalMap.png";
	fabricRender.first.SamplerMap[CONST_STRING_HEIGHTMAP_UNIFORM] = "HeightMap.png";
	fabricRender.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "clean-fabric-texture-4-780x585.jpg";
	fabricRender.first.Vec4Map[CONST_STRING_LIGHT_DIRECTION_UNIFORM] = Vector4f(0, -1, 0, 0);

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
