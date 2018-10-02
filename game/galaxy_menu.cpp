
#include "galaxy_menu.h"
#include "main_code.h"
#include <algorithm>
#include <math.h>

GalaxyMenu::GalaxyMenu()
{

}

GalaxyMenu::~GalaxyMenu()
{
}

bool GalaxyMenu::InitGalaxyMenu(std::string config_json, float scale) {
	try
	{
		/*..Inner Init..*/
		menuScale = scale;

	/*...Read ptree from config...*/

	boost::property_tree::ptree config_pt = SE::ReadJsonFile(ST::PathToResources + config_json);
	//boost::property_tree::json_parser::read_json(res_path + config_json, config_pt);

	/*..error ptree..*/
	boost::property_tree::ptree error_pt;
	error_pt.put("error", "something goes wrong at InitGalaxyMenu");


	/*..Init Menu..*/
	BOOST_FOREACH(auto menu_pt, config_pt.get_child("Space", error_pt)) {
		Galaxy galax;

		galax.name = menu_pt.second.get<std::string>("name", "error");
		galax.scale = menu_pt.second.get<float>("scale", 0.0f);
		galax.texture = menu_pt.second.get<std::string>("texture", "error");
		galax.position = Eigen::Vector2f(menu_pt.second.get<float>("position.x_coord", 0.0f), menu_pt.second.get<float>("position.y_coord", 0.0f));
		//boost::property_tree::ptree stars = menu_pt.second.get<boost::property_tree::ptree>("Stars", error_pt);

		int levelStar = 0;

		BOOST_FOREACH(auto stars_pt, menu_pt.second.get_child("Stars", error_pt)) {
			StarObject star;

			star.name = stars_pt.second.get<std::string>("name", "error");
			star.scale = stars_pt.second.get<float>("scale", 0.0f);
			star.texture = stars_pt.second.get<std::string>("texture", "error");

			star.textureName = GetFileName(star.texture);
			ResourceManager->TexList.AddTexture(star.texture);

			star.position = Eigen::Vector2f(stars_pt.second.get<float>("position.x_coord", 0.0f), stars_pt.second.get<float>("position.y_coord", 0.0f));

			int levelIndex = 0;
			
			/*..Levels..*/
			BOOST_FOREACH(auto levels_pt, stars_pt.second.get_child("levels")) {

				std::string levelName = levels_pt.second.get<std::string>("name", "empty");
				
				std::shared_ptr<TGameLevel> lvl = std::make_shared<TGameLevel>(levelStar, levelIndex);
				lvl->FillWithFile(ST::PathToResources + levelName + ".txt");

				star.selectionMenu.gameLevels.push_back(lvl);

				++levelIndex;
			}
			galax.Stars.push_back(star);

			++levelStar;
		} 
		galaxies.push_back(galax);
	}


	return true;
	}
	catch (boost::property_tree::ptree_error) {
		return false;
	}
}




void GalaxyMenu::UpdateGalaxyMenu(float s_width, float s_height, size_t dt) {
	/*..Reset..*/
	galaxies_params.clear();
	stars_params.clear();

	/*..Menu ancestor geometry..*/
	float gameScreenWidth = s_width * anchorSize;
	float gameScreenHeight = s_height * anchorSize;
	Eigen::Vector2f gameScreenCenter = Eigen::Vector2f(gameScreenWidth/2,gameScreenHeight/2);

	/*..coefficients calculation..*/
	/*Eigen::Vector2f tap_shift = Eigen::Vector2f(
		totalTapShift(0)/gameScreenWidth,
		totalTapShift(1)/gameScreenHeight
	);
		menu_offset = Eigen::Vector2f(
			negativeV(tap_shift(0)) * val_clamp(abs(menu_offset(0) + (tap_shift(0) * dt / 1000.f)), 0.f, 0.15f),
			negativeV(tap_shift(1)) * val_clamp(abs(menu_offset(1) + (tap_shift(1) * dt / 1000.f)), 0.f, 0.15f)
		);*/

	/*..Menu geometry..*/
	xDimension = menuScale * gameScreenWidth;
	yDimension = menuScale * gameScreenHeight;
	Eigen::Vector2f currentMenuPos = Eigen::Vector2f(gameScreenCenter(0) + (gameScreenWidth/2/*relative to the screen x-dimension*/)*(menuPosition(0) - menu_offset(0)), gameScreenCenter(1) + (gameScreenHeight/2/*relative to the screen y-dimension*/)*(menuPosition(1) - menu_offset(1)));

	/*..Galaxies geometry..*/
	for (int i = 0; i < galaxies.size(); i++) {

		Eigen::Vector2f tex_size = textureSizeNormalize(
			Eigen::Vector2f(
				((float)SE::ResourceManager->TexList.GetTextureOriginalWidth("galaxy_" + std::to_string(i))),
				((float)SE::ResourceManager->TexList.GetTextureOriginalHeight("galaxy_" + std::to_string(i))))
		); // normalized
		galaxies_params.push_back(std::make_pair(
			Eigen::Vector2f(
				currentMenuPos(0) + (xDimension / 2)*galaxies[i].position(0),
				currentMenuPos(1) + (yDimension / 2)*galaxies[i].position(1)),
			Eigen::Vector2f(
				(tex_size(0)*galaxies[i].scale)*menuScale,
				(tex_size(1)*galaxies[i].scale)*menuScale
			)
		));

		/*..Stars geometry..*/
		std::vector<std::pair<Eigen::Vector2f, Eigen::Vector2f>> star_params;
		for (int j = 0; j < galaxies[i].Stars.size(); j++) {

			tex_size = textureSizeNormalize(
				Eigen::Vector2f(
				((float)SE::ResourceManager->TexList.GetTextureWidth(galaxies[i].Stars[j].textureName)),
				((float)SE::ResourceManager->TexList.GetTextureHeight(galaxies[i].Stars[j].textureName)))
			); // normalized
			star_params.push_back(std::make_pair(
				Eigen::Vector2f(
					galaxies_params[i].first(0) + (galaxies_params[i].second(0)/2)*galaxies[i].Stars[j].position(0),
					galaxies_params[i].first(1) + (galaxies_params[i].second(1)/2)*galaxies[i].Stars[j].position(1)
				),
				Eigen::Vector2f(
					(tex_size(0)*galaxies[i].Stars[j].scale)*galaxies[i].scale*menuScale,
					(tex_size(1)*galaxies[i].Stars[j].scale)*galaxies[i].scale*menuScale
				)
			));
		}
		stars_params.push_back(star_params);
	}

}

Eigen::Vector2f GalaxyMenu::textureSizeNormalize(Eigen::Vector2f texVec, int t_type) {
	float tex_ratio = texVec(0)/texVec(1);
	float x_dim, y_dim;
	float Xmax; // Max normalized texture width 
	float Xmin;
	float Ymax; // Max normalized texture height
	float Ymin;
	if (t_type == 0) {
		Xmax = SE::Renderer->GetScreenWidth(); 
		Xmin = Xmax;
		Ymax = SE::Renderer->GetScreenHeight();
		Ymin = Ymax;
	}
	else { // temp for star textures
		Xmax = (((float)SE::Renderer->GetScreenWidth())/2);
		Xmin = Xmax;
		Ymax = (((float)SE::Renderer->GetScreenHeight())/2);
		Ymin = Ymax;
	}

	if (texVec(0) > texVec(1)) {
		x_dim = val_clamp(texVec(0), Xmin, Xmax);
		y_dim = x_dim / tex_ratio;
	}
	else {
		y_dim = val_clamp(texVec(1), Ymin, Ymax);
		x_dim = y_dim * tex_ratio;
	} 

	return Eigen::Vector2f(x_dim, y_dim);
}

float GalaxyMenu::val_clamp(float val, float min, float max) {
	if (val < min)
		return min;
	else if (val > max)
		return max;
	else
		return val;
}

void GalaxyMenu::DrawGalaxyMenu() {

	for (int i = 0; i < galaxies_params.size(); i++) {

		glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList["galaxy_" + std::to_string(i)]);
		SE::Renderer->DrawRect(
			Eigen::Vector2f(
				galaxies_params[i].first(0) - galaxies_params[i].second(0)/2,
				galaxies_params[i].first(1) - galaxies_params[i].second(1)/2
			),
			Eigen::Vector2f(
				galaxies_params[i].first(0) + galaxies_params[i].second(0)/2,
				galaxies_params[i].first(1) + galaxies_params[i].second(1)/2
			)
		); // DrawRect

		/*..Draw stars..*/
		Renderer->PushShader("HoverableButtonShader");
		if (stars_params.size() >= i) {
			for (int j = 0; j < stars_params[i].size(); j++) {
				if (planetHoverIndex == j) {
					RenderUniform1i("Hover", 1);
					glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList[galaxies[i].Stars[j].textureName]);
				}
				else {
					RenderUniform1i("Hover", 0);
					glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList[galaxies[i].Stars[j].textureName]);
				}
				SE::Renderer->DrawRect(
					Eigen::Vector2f(
						stars_params[i][j].first(0) - stars_params[i][j].second(0) / 2,
						stars_params[i][j].first(1) - stars_params[i][j].second(1) / 2
					),
					Eigen::Vector2f(
						stars_params[i][j].first(0) + stars_params[i][j].second(0) / 2,
						stars_params[i][j].first(1) + stars_params[i][j].second(1) / 2
					)
				); // DrawRect
			}
		}
		Renderer->PopShader();

		/*..Draw level selection menu..*/
		//drawSelectionMenu(starIndex);
		
	}

}

void GalaxyMenu::InteractWithGalaxy(size_t dt) {
	if (timer_active) {
		// ::::::::::::: timer active ::::::::::::::
		if (menuState == 0) { // main view
			//if (currentTapShift(0) == 0.f && currentTapShift(1) == 0.f) {
			if (currentTapShift(0) == totalTapShift(0) && currentTapShift(1) == totalTapShift(1)) {
				// OnTapDown->

				/*../hover\..*/
				int phi = findPlanetByPos(lastTapPos - totalTapShift);
				if (phi != -1) {
					planetHoverIndex = phi;
				}
				else {
					planetHoverIndex = -1;
				}
				/*..\hover/..*/
			}
			else {
				// OnTapDown->OnMove->
				//totalTapShift = Eigen::Vector2f(totalTapShift(0) + currentTapShift(0), totalTapShift(1) + currentTapShift(1));

				/*../hover\..*/
				int phi = findPlanetByPos(lastTapPos - totalTapShift);
				if (phi != -1) {
					planetHoverIndex = phi;
				}
				else {
					planetHoverIndex = -1;
				}
				/*..\hover/..*/

			}

		}
		/*
		else if (menuState == 1) { // zoomed galaxy

		}
		else if (menuState == 2) { // level select view
			//if (currentTapShift(0) == 0.f && currentTapShift(1) == 0.f) {
			if (currentTapShift(0) == totalTapShift(0) && currentTapShift(1) == totalTapShift(1)) {
				// OnTapDown->

			}
			else {
				// OnTapDown->OnMove->
				//totalTapShift = Eigen::Vector2f(totalTapShift(0) + currentTapShift(0), totalTapShift(1) + currentTapShift(1));
			}
		}*/

		// \_/\_/\_/\_/ timer active \_/\_/\_/\_/
	}
	else {
		// ::::::::::::: timer inactive ::::::::::::::
		if (lastTapPos != Eigen::Vector2f(-9999.9f, -9999.9f)) {
			if (menuState == 0) {// main view

					// OnTapDown->OnTapUp

					/*..level select menu open..*/
					starIndex = findPlanetByPos(lastTapPos);


					if (starIndex != -1) {
						planetHoverIndex = starIndex;
						Application->SetupGalaxyUi(starIndex);
						ResourceManager->newGuiManager.findWidgetByName("modal_background")->setVisibility(true);
					}

					timer_active = false;

				

			}/*
			else if (menuState == 1) { // zoomed galaxy
				if (totalTapShift(0) == 0.f && totalTapShift(1) == 0.f) {
					// OnTapDown->OnTapUp

				}
				else {
					// OnTapDown->OnMove->OnTapUp

				}

			}
			else if (menuState == 2) { // level select view

				
				if (totalTapShift(0) == 0.f && totalTapShift(1) == 0.f) {
					// OnTapDown->OnTapUp

					auto lvl = findLevelByButtonPos(lastTapPos);
					if (lvl != nullptr) {
						// then if level is available, load it
						starIndex = -1;
						menuState = 0;
						planetHoverIndex = -1;
						Application->GoFromMenuToGame(lvl);
					}
					else if (!checkMenuBound(lastTapPos)) {
						// back to state 0
						starIndex = -1;
						menuState = 0;
						planetHoverIndex = -1;
					}

				}
				else {
					// OnTapDown->OnMove->OnTapUp

				}

			}*/
		}
		// \_/\_/\_/\_/ timer inactive \_/\_/\_/\_/
	}

	/*..main reset..*/
	if (timer_active) {
		interact_timer += (float)dt;
	}
	else if (interact_timer != 0.f) {
		interact_timer = 0.f; // reset
		currentTapShift = Eigen::Vector2f(0.f, 0.f); // reset
		totalTapShift = Eigen::Vector2f(0.f, 0.f); // reset
		lastTapPos = Eigen::Vector2f(-9999.9f, -9999.9f); // reset
	}

}

void GalaxyMenu::tapDown(Eigen::Vector2f pos) {
	if (!timer_active) {
		timer_active = true;
		lastTapPos = pos;
	}
}

void GalaxyMenu::tapUp(Eigen::Vector2f pos) {
	if (timer_active) {
		timer_active = false;
		// lastTapPos = vec(0,0) useless for now
	}
}

void GalaxyMenu::tapMove(Eigen::Vector2f shift) {
	if (timer_active) {

		totalTapShift += shift;
	}
}

Eigen::Vector2f GalaxyMenu::findCorner(int x_c, int y_c) {
	float x_pos = 0.f;
	float y_pos = 0.f;
	float x_p, y_p;
	if (x_c > 0) { // ===
		if (y_c > 0) { // ---
			for (int i = 0; i < galaxies_params.size(); i++) {
				x_p = galaxies_params[i].first(0) + (galaxies_params[i].second(0) / 2);
				y_p = galaxies_params[i].first(1) + (galaxies_params[i].second(1) / 2);
				x_pos = graterV(x_pos, x_p);
				y_pos = graterV(y_pos, y_p);
				for (int j = 0; j < stars_params[i].size(); j++) {
					x_p = stars_params[i][j].first(0) + (stars_params[i][j].second(0));
					y_p = stars_params[i][j].first(1) + (stars_params[i][j].second(1));
					x_pos = graterV(x_pos, x_p);
					y_pos = graterV(y_pos, y_p);
				}
			}
		}
		else {// ---
			for (int i = 0; i < galaxies_params.size(); i++) {
				x_p = galaxies_params[i].first(0) + (galaxies_params[i].second(0) / 2);
				y_p = galaxies_params[i].first(1) - (galaxies_params[i].second(1) / 2);
				x_pos = graterV(x_pos, x_p);
				y_pos = lowerV(y_pos, y_p);
				for (int j = 0; j < stars_params[i].size(); j++) {
					x_p = stars_params[i][j].first(0) + (stars_params[i][j].second(0));
					y_p = stars_params[i][j].first(1) - (stars_params[i][j].second(1));
					x_pos = graterV(x_pos, x_p);
					y_pos = lowerV(y_pos, y_p);
				}
			}
		}// ---
	} 
	else { // ===
		if (y_c > 0) { // ---
			for (int i = 0; i < galaxies_params.size(); i++) {
				x_p = galaxies_params[i].first(0) - (galaxies_params[i].second(0) / 2);
				y_p = galaxies_params[i].first(1) + (galaxies_params[i].second(1) / 2);
				x_pos = lowerV(x_pos, x_p);
				y_pos = graterV(y_pos, y_p);
				for (int j = 0; j < stars_params[i].size(); j++) {
					x_p = stars_params[i][j].first(0) - (stars_params[i][j].second(0));
					y_p = stars_params[i][j].first(1) + (stars_params[i][j].second(1));
					x_pos = lowerV(x_pos, x_p);
					y_pos = graterV(y_pos, y_p);
				}
			}
		}
		else { // ---
			for (int i = 0; i < galaxies_params.size(); i++) {
				x_p = galaxies_params[i].first(0) - (galaxies_params[i].second(0) / 2);
				y_p = galaxies_params[i].first(1) - (galaxies_params[i].second(1) / 2);
				x_pos = lowerV(x_pos, x_p);
				y_pos = lowerV(y_pos, y_p);
				for (int j = 0; j < stars_params[i].size(); j++) {
					x_p = stars_params[i][j].first(0) - (stars_params[i][j].second(0));
					y_p = stars_params[i][j].first(1) - (stars_params[i][j].second(1));
					x_pos = lowerV(x_pos, x_p);
					y_pos = lowerV(y_pos, y_p);
				}
			}
		} // ---
	} // ===
	return Eigen::Vector2f(x_pos, y_pos);
}

float GalaxyMenu::graterV(float first_val, float second_val) {
	if (first_val > second_val)
		return first_val;
	else
		return second_val;
}

float GalaxyMenu::lowerV(float first_val, float second_val) {
	if (first_val < second_val)
		return first_val;
	else
		return second_val;
}

void GalaxyMenu::takeInFocus(int g_index, int s_index) {
	if (s_index != -1) { /*..galaxy zoom..*/

	}
	else { /*..star zoom..*/

	}
}

int GalaxyMenu::findGalaxyByPos(Eigen::Vector2f pos) {

	for (int i = 0; i < galaxies_params.size(); i++) {
		if (pos(0) >= (galaxies_params[i].first(0) - galaxies_params[i].second(0)/2) && pos(0) <= (galaxies_params[i].first(0) + galaxies_params[i].second(0)/2)) {
			if (pos(1) >= (galaxies_params[i].first(1) - galaxies_params[i].second(1) / 2) && pos(1) <= (galaxies_params[i].first(1) + galaxies_params[i].second(1)/2)) {
				return i;
			}
		}
	}

	return -1;
}

int GalaxyMenu::negativeV(float val) {
	if (val >= 0) {
		return 1;
	}
	return -1;
}

int GalaxyMenu::findPlanetByPos(Eigen::Vector2f pos) {

	float minimalDistance = std::numeric_limits<float>::max();
	int index = -1;

	for (int i = 0; i < stars_params.size(); i++)
	{
		for (int j = 0; j < stars_params[i].size(); j++)
		{
			if (pos(0) >= (stars_params[i][j].first(0) - stars_params[i][j].second(0) / 2) && pos(0) <= (stars_params[i][j].first(0) + stars_params[i][j].second(0) / 2))
			{
				if (pos(1) >= (stars_params[i][j].first(1) - stars_params[i][j].second(1) / 2) && pos(1) <= (stars_params[i][j].first(1) + stars_params[i][j].second(1) / 2))
				{
					float dx = pos(0) - stars_params[i][j].first(0);
					float dy = pos(1) - stars_params[i][j].first(1);
					float distance = dx * dx + dy * dy;

					if (distance < minimalDistance)
					{
						minimalDistance = distance;
						index = j;
					}
				}
			}
		}
	}
	return index;
}

bool GalaxyMenu::checkMenuBound(Eigen::Vector2f pos) {
	float x_l = galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons_plane.first(0) - galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons_plane.second(0)*0.5f;
	float x_r = galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons_plane.first(0) + galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons_plane.second(0)*0.5f;
	float y_t = galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons_plane.first(1) + galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons_plane.second(1)*0.5f;
	float y_b = galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons_plane.first(1) - galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons_plane.second(1)*0.5f;

	if (pos(0) >= x_l && pos(0) <= x_r) {
		if (pos(1) >= y_b && pos(1) <= y_t) {
			return true;
		}
	}
	return false;
}

void GalaxyMenu::drawBorder(Eigen::Vector2f lb_, Eigen::Vector2f rt_, float scale, std::string mode) {

	std::string border_tex = "black_square";

	float width = rt_(0) - lb_(0);
	float height = rt_(1) - lb_(1);

	if (mode.compare("inner") == 0) {
		glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList[border_tex]);
		SE::Renderer->DrawRect(
			Eigen::Vector2f(lb_(0), lb_(1)),
			Eigen::Vector2f(lb_(0) + width*scale, rt_(1))
		); // left
		SE::Renderer->DrawRect(
			Eigen::Vector2f(lb_(0), rt_(1) - width*scale),
			Eigen::Vector2f(rt_(0), rt_(1))
		); // top
		SE::Renderer->DrawRect(
			Eigen::Vector2f(rt_(0) - width*scale, lb_(1)),
			Eigen::Vector2f(rt_(0), rt_(1))
		); // right
		SE::Renderer->DrawRect(
			Eigen::Vector2f(lb_(0), lb_(1)),
			Eigen::Vector2f(rt_(0), lb_(1) + width*scale)
		); // bottom
	}
	else if (mode.compare("outer") == 0) {
		glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList[border_tex]);
		SE::Renderer->DrawRect(
			Eigen::Vector2f(lb_(0) - width*scale, lb_(1) - width*scale),
			Eigen::Vector2f(lb_(0), rt_(1) + width*scale)
		); // left
		SE::Renderer->DrawRect(
			Eigen::Vector2f(lb_(0) - width*scale, rt_(1)),
			Eigen::Vector2f(rt_(0) + width*scale, rt_(1)+width*scale)
		); // top
		SE::Renderer->DrawRect(
			Eigen::Vector2f(rt_(0), lb_(1) - width*scale),
			Eigen::Vector2f(rt_(0)+width*scale, rt_(1) + width*scale)
		); // right
		SE::Renderer->DrawRect(
			Eigen::Vector2f(lb_(0) - width*scale, lb_(1) - width*scale),
			Eigen::Vector2f(rt_(0)+width*scale, lb_(1))
		); // bottom
	}
	else if (mode.compare("center") == 0) {
		glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList[border_tex]);
		SE::Renderer->DrawRect(
			Eigen::Vector2f(lb_(0) - width*scale*0.5f, lb_(1) - width * scale*0.5f),
			Eigen::Vector2f(lb_(0) + width * scale*0.5f, rt_(1) + width * scale*0.5f)
		); // left
		SE::Renderer->DrawRect(
			Eigen::Vector2f(lb_(0) - width * scale*0.5f, rt_(1) - width * scale*0.5f),
			Eigen::Vector2f(rt_(0) + width * scale*0.5f, rt_(1) + width * scale*0.5f)
		); // top
		SE::Renderer->DrawRect(
			Eigen::Vector2f(rt_(0) - width * scale*0.5f, lb_(1) - width * scale*0.5f),
			Eigen::Vector2f(rt_(0) + width * scale*0.5f, rt_(1) + width * scale*0.5f)
		); // right
		SE::Renderer->DrawRect(
			Eigen::Vector2f(lb_(0) - width * scale*0.5f, lb_(1) - width * scale*0.5f),
			Eigen::Vector2f(rt_(0) + width * scale*0.5f, lb_(1) + width * scale*0.5f)
		); // bottom
	}
}

void GalaxyMenu::drawLevelInterior(int star, int button) {
	glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList[CONST_BLOCK_TEXTURE1]);
	for (int i = 0; i < galaxies[0].Stars[star].selectionMenu.levelInterior[button].size(); i++) {
		Renderer->DrawRect(
			Eigen::Vector2f(
				galaxies[0].Stars[star].selectionMenu.levelInterior[button][i].position(0) - galaxies[0].Stars[star].selectionMenu.levelInterior[button][i].dimensions(0)*0.5f,
				galaxies[0].Stars[star].selectionMenu.levelInterior[button][i].position(1) - galaxies[0].Stars[star].selectionMenu.levelInterior[button][i].dimensions(1)*0.5f
			),
			Eigen::Vector2f(
				galaxies[0].Stars[star].selectionMenu.levelInterior[button][i].position(0) + galaxies[0].Stars[star].selectionMenu.levelInterior[button][i].dimensions(0)*0.5f,
				galaxies[0].Stars[star].selectionMenu.levelInterior[button][i].position(1) + galaxies[0].Stars[star].selectionMenu.levelInterior[button][i].dimensions(1)*0.5f
			)
		);
	}
	
}