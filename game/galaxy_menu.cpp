
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
	std::string res_path = "../../../assets/";
	boost::property_tree::ptree config_pt;
	boost::property_tree::json_parser::read_json(res_path + config_json, config_pt);

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
		BOOST_FOREACH(auto stars_pt, menu_pt.second.get_child("Stars", error_pt)) {
			StarObject star;

			star.name = stars_pt.second.get<std::string>("name", "error");
			star.scale = stars_pt.second.get<float>("scale", 0.0f);
			star.texture = stars_pt.second.get<std::string>("texture", "error");
			star.position = Eigen::Vector2f(stars_pt.second.get<float>("position.x_coord", 0.0f), stars_pt.second.get<float>("position.y_coord", 0.0f));
			/*..Levels..*/
			BOOST_FOREACH(auto levels_pt, stars_pt.second.get_child("levels")) {

				star.selectionMenu.levels.push_back(levels_pt.second.get<std::string>("name", "empty"));

			}
			galax.Stars.push_back(star);
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
		*SE::Console << "galaxy_" + std::to_string(i);
		Eigen::Vector2f tex_size = textureSizeNormalize(
			Eigen::Vector2f(
				((float)SE::ResourceManager->TexList.GetTextureWidth("galaxy_" + std::to_string(i))),
				((float)SE::ResourceManager->TexList.GetTextureHeight("galaxy_" + std::to_string(i))))
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
		*SE::Console << "dimensions: " << std::to_string(((tex_size(0)*galaxies[i].scale)*menuScale)) << " " << std::to_string(((tex_size(1)*galaxies[i].scale)*menuScale));

		/*..Stars geometry..*/
		std::vector<std::pair<Eigen::Vector2f, Eigen::Vector2f>> star_params;
		for (int j = 0; j < galaxies[i].Stars.size(); j++) {
			*SE::Console << "star_" + std::to_string(i) + "_" + std::to_string(j);
			tex_size = textureSizeNormalize(
				Eigen::Vector2f(
				((float)SE::ResourceManager->TexList.GetTextureWidth("star_" + std::to_string(i) + "_" + std::to_string(j))),
				((float)SE::ResourceManager->TexList.GetTextureHeight("star_" + std::to_string(i) + "_" + std::to_string(j))))
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

	/*..Level list geometry..*/
	for (int i = 0; i < galaxies.size(); i++) {
		for (int j = 0; j < galaxies[i].Stars.size(); j++) {
			float button_x_dim = ((1.f - (galaxies[i].Stars[j].selectionMenu.border_x_offset * 2 + (galaxies[i].Stars[j].selectionMenu.columns - 1)*galaxies[i].Stars[j].selectionMenu.buttons_offset)) / galaxies[i].Stars[j].selectionMenu.columns); // relative size
			int rows_count = (int)ceil((float)galaxies[i].Stars[j].selectionMenu.levels.size() / (float)galaxies[i].Stars[j].selectionMenu.columns);
			galaxies[i].Stars[j].selectionMenu.params = std::make_pair(
				Eigen::Vector2f(
					gameScreenCenter(0) + (galaxies[i].Stars[j].selectionMenu.offset(0) * gameScreenWidth / 2),
					gameScreenCenter(1) + (galaxies[i].Stars[j].selectionMenu.offset(1) * gameScreenHeight / 2)
				),
				Eigen::Vector2f(
					gameScreenWidth * galaxies[i].Stars[j].selectionMenu.dim(0),
					//gameScreenHeight * galaxies[i].Stars[j].selectionMenu.dim(1)
					//gameScreenHeight * galaxies[i].Stars[j].selectionMenu.dim(1) * (galaxies[i].Stars[j].selectionMenu.border_y_offset * 2 + (ceil(galaxies[i].Stars[j].selectionMenu.levels.size() / galaxies[i].Stars[j].selectionMenu.columns) - 1)*galaxies[i].Stars[j].selectionMenu.buttons_offset) + galaxies[i].Stars[j].selectionMenu.dim(0)*(ceil(galaxies[i].Stars[j].selectionMenu.levels.size() / galaxies[i].Stars[j].selectionMenu.columns)*(button_x_dim / galaxies[i].Stars[j].selectionMenu.buttons_ratio))
					gameScreenWidth * galaxies[i].Stars[j].selectionMenu.dim(0) * (galaxies[i].Stars[j].selectionMenu.border_y_offset * 2 + rows_count*(button_x_dim/galaxies[i].Stars[j].selectionMenu.buttons_ratio) + (rows_count-1)*galaxies[i].Stars[j].selectionMenu.border_y_offset)
				)
			);
			/*..buttons plane..*/
			galaxies[i].Stars[j].selectionMenu.buttons_plane = std::make_pair(
				Eigen::Vector2f(
					galaxies[i].Stars[j].selectionMenu.params.first(0) + galaxies[i].Stars[j].selectionMenu.params.second(0) / 2 * galaxies[i].Stars[j].selectionMenu.plane_pos(0),
					galaxies[i].Stars[j].selectionMenu.params.first(1) + galaxies[i].Stars[j].selectionMenu.params.second(1) / 2 * galaxies[i].Stars[j].selectionMenu.plane_pos(1)
				),
				Eigen::Vector2f(
					galaxies[i].Stars[j].selectionMenu.params.second(0) * galaxies[i].Stars[j].selectionMenu.plane_size(0),
					galaxies[i].Stars[j].selectionMenu.params.second(1) * galaxies[i].Stars[j].selectionMenu.plane_size(1)
				)
			);

			// buttons
			std::vector<std::pair<Eigen::Vector2f, Eigen::Vector2f>> buttons_params;
			buttons_params.resize(galaxies[i].Stars[j].selectionMenu.levels.size());
			for (int y = 0; y < buttons_params.size(); y++) {
				
				float x_rpos = (galaxies[i].Stars[j].selectionMenu.border_x_offset + (y - floor((float)y / (float)galaxies[i].Stars[j].selectionMenu.columns)*(float)galaxies[i].Stars[j].selectionMenu.columns)*(button_x_dim + galaxies[i].Stars[j].selectionMenu.buttons_offset) + button_x_dim/2);
				//float y_rpos = (/**/ galaxies[i].Stars[j].selectionMenu.border_y_offset + (floor((float)y / (float)galaxies[i].Stars[j].selectionMenu.columns))*(/*..*/ galaxies[i].Stars[j].selectionMenu.buttons_offset + button_x_dim / galaxies[i].Stars[j].selectionMenu.buttons_ratio /*..*/) + button_x_dim/galaxies[i].Stars[j].selectionMenu.buttons_ratio/2 /**/);
				float y_rpos = (galaxies[i].Stars[j].selectionMenu.border_y_offset + ((int)floor(((float)y)/((float)galaxies[i].Stars[j].selectionMenu.columns)))*(button_x_dim/galaxies[i].Stars[j].selectionMenu.buttons_ratio) + (((int)floor(((float)y) / ((float)galaxies[i].Stars[j].selectionMenu.columns)))) * galaxies[i].Stars[j].selectionMenu.border_y_offset + ((button_x_dim/galaxies[i].Stars[j].selectionMenu.buttons_ratio)/2.f));

				buttons_params[y] = std::make_pair(
					Eigen::Vector2f(
						(galaxies[i].Stars[j].selectionMenu.buttons_plane.first(0) - galaxies[i].Stars[j].selectionMenu.buttons_plane.second(0)/2) + galaxies[i].Stars[j].selectionMenu.buttons_plane.second(0)*x_rpos,
						(galaxies[i].Stars[j].selectionMenu.buttons_plane.first(1) + galaxies[i].Stars[j].selectionMenu.buttons_plane.second(1)/2) - galaxies[i].Stars[j].selectionMenu.buttons_plane.second(0)*y_rpos
					),
					Eigen::Vector2f(
						button_x_dim*galaxies[i].Stars[j].selectionMenu.buttons_plane.second(0),
						(button_x_dim / galaxies[i].Stars[j].selectionMenu.buttons_ratio)*galaxies[i].Stars[j].selectionMenu.buttons_plane.second(0)

					)
				);
			}
			galaxies[i].Stars[j].selectionMenu.buttons = buttons_params;
		}
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
	*SE::Console << "==============";
	*SE::Console << std::to_string(texVec(0));
	*SE::Console << std::to_string(texVec(1));
	*SE::Console << "if value 0.0000 - must be textures not inited at main_code.cpp or texture-name is wrong";
	*SE::Console << "--------------";
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
		if (stars_params.size() >= i) {
			for (int j = 0; j < stars_params[i].size(); j++) {
				if (planetHoverIndex == j) {
					glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList["star_" + std::to_string(i) + "_" + std::to_string(j) + "_hover"]);
				}
				else {
					glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList["star_" + std::to_string(i) + "_" + std::to_string(j)]);
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

		/*..Draw level selection menu..*/
		drawSelectionMenu(starIndex);
		
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
		}

		// \_/\_/\_/\_/ timer active \_/\_/\_/\_/
	}
	else {
		// ::::::::::::: timer inactive ::::::::::::::
		if (lastTapPos != Eigen::Vector2f(-9999.9f, -9999.9f)) {
			if (menuState == 0) {// main view
				if (totalTapShift(0) == 0.f && totalTapShift(1) == 0.f) {
					// OnTapDown->OnTapUp

					/*..level select menu open..*/
					starIndex = findPlanetByPos(lastTapPos);
					if (starIndex != -1) {
						planetHoverIndex = starIndex;
						menuState = 2;
					}
					else {
						planetHoverIndex = -1;
					}

				}
				else {
					// OnTapDown->OnMove->OnTapUp

					/*..level select menu open..*/
					starIndex = findPlanetByPos(lastTapPos - totalTapShift);
					if (starIndex != -1) {
						planetHoverIndex = starIndex;
						menuState = 2;
					}
					else {
						planetHoverIndex = -1;
					}

				}

			}
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

					int lvl = findLevelButtonByPos(lastTapPos);
					if (lvl != -1) {
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

			}
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
		//*SE::Console << "shift = " + std::to_string(shift(0)) + " " + std::to_string(shift(1)); // mt issue
		//currentTapShift = shift; // shift need to be fixed
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
	for (int i = 0; i < stars_params.size(); i++) {
		for (int j = 0; j < stars_params[i].size(); j++) {
			if (pos(0) >= (stars_params[i][j].first(0) - stars_params[i][j].second(0) / 2) && pos(0) <= (stars_params[i][j].first(0) + stars_params[i][j].second(0) / 2)) {
				if (pos(1) >= (stars_params[i][j].first(1) - stars_params[i][j].second(1) / 2) && pos(1) <= (stars_params[i][j].first(1) + stars_params[i][j].second(1) / 2)) {
					return j;
				}
			}
		}
	}
	return -1;
}

void GalaxyMenu::drawSelectionMenu(int index) {
	int i = index;
	if (i != -1) {

		glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList["matte_screen"]);
		SE::Renderer->DrawRect(
			Eigen::Vector2f(
				0.f,
				0.f
			),
			Eigen::Vector2f(
				(float)SE::Renderer->GetScreenWidth(),
				(float)SE::Renderer->GetScreenHeight()
			)
		); // DrawRect

		glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList["red_square"]);
		SE::Renderer->DrawRect(
			Eigen::Vector2f(
				galaxies[0].Stars[i].selectionMenu.buttons_plane.first(0) - galaxies[0].Stars[i].selectionMenu.buttons_plane.second(0) / 2,
				galaxies[0].Stars[i].selectionMenu.buttons_plane.first(1) - galaxies[0].Stars[i].selectionMenu.buttons_plane.second(1) / 2
			),
			Eigen::Vector2f(
				galaxies[0].Stars[i].selectionMenu.buttons_plane.first(0) + galaxies[0].Stars[i].selectionMenu.buttons_plane.second(0) / 2,
				galaxies[0].Stars[i].selectionMenu.buttons_plane.first(1) + galaxies[0].Stars[i].selectionMenu.buttons_plane.second(1) / 2
			)
		); // DrawRect

		/*..border..*/
		drawBorder(
			Eigen::Vector2f(
				galaxies[0].Stars[i].selectionMenu.buttons_plane.first(0) - galaxies[0].Stars[i].selectionMenu.buttons_plane.second(0) / 2,
				galaxies[0].Stars[i].selectionMenu.buttons_plane.first(1) - galaxies[0].Stars[i].selectionMenu.buttons_plane.second(1) / 2
			),
			Eigen::Vector2f(
				galaxies[0].Stars[i].selectionMenu.buttons_plane.first(0) + galaxies[0].Stars[i].selectionMenu.buttons_plane.second(0) / 2,
				galaxies[0].Stars[i].selectionMenu.buttons_plane.first(1) + galaxies[0].Stars[i].selectionMenu.buttons_plane.second(1) / 2
			),
			0.005f,
			"outer" /*..inner/outer/center..mode */
		);
		
		// buttons
		for (int j = 0; j < galaxies[0].Stars[i].selectionMenu.buttons.size(); j++) {
			glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList["black_square"]);
			SE::Renderer->DrawRect(
				Eigen::Vector2f(
					galaxies[0].Stars[i].selectionMenu.buttons[j].first(0) - galaxies[0].Stars[i].selectionMenu.buttons[j].second(0) / 2,
					galaxies[0].Stars[i].selectionMenu.buttons[j].first(1) - galaxies[0].Stars[i].selectionMenu.buttons[j].second(1) / 2
				),
				Eigen::Vector2f(
					galaxies[0].Stars[i].selectionMenu.buttons[j].first(0) + galaxies[0].Stars[i].selectionMenu.buttons[j].second(0) / 2,
					galaxies[0].Stars[i].selectionMenu.buttons[j].first(1) + galaxies[0].Stars[i].selectionMenu.buttons[j].second(1) / 2
				)
			); // DrawRect
		}

	}
}

int GalaxyMenu::findLevelButtonByPos(Eigen::Vector2f pos) {
	for (int i = 0; i < galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons.size(); i++) {
		float x_l = galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons[i].first(0) - galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons[i].second(0)*0.5f;
		float x_r = galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons[i].first(0) + galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons[i].second(0)*0.5f;
		float y_t = galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons[i].first(1) + galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons[i].second(1)*0.5f;
		float y_b = galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons[i].first(1) - galaxies[galaxyIndex].Stars[starIndex].selectionMenu.buttons[i].second(1)*0.5f;

		if (pos(0) >= x_l && pos(0) <= x_r) {
			if (pos(1) >= y_b && pos(1) <= y_t) {
				return i;
			}
		}
	}
	return -1;
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