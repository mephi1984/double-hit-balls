
#include "galaxy_menu.h"
#include <algorithm>

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
			galaxies[i].Stars[j].selectionMenu.params = std::make_pair(
				Eigen::Vector2f(
					currentMenuPos(0) + (galaxies[i].Stars[j].selectionMenu.offset(0) * gameScreenWidth / 2),
					currentMenuPos(1) + (galaxies[i].Stars[j].selectionMenu.offset(1) * gameScreenHeight / 2)
				),
				Eigen::Vector2f(
					gameScreenWidth * galaxies[i].Stars[j].selectionMenu.dim(0),
					gameScreenHeight * galaxies[i].Stars[j].selectionMenu.dim(1)
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
			// buttons plane dimensions build, relative to buttons #from here
			// buttons
			for (int y = 0; y < galaxies[i].Stars[j].selectionMenu.levels.size(); y++) {
				galaxies[i].Stars[j].selectionMenu.buttons.push_back(std::make_pair(
					Eigen::Vector2f(

					),
					Eigen::Vector2f(

					)
				));
			}
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
				glBindTexture(GL_TEXTURE_2D, SE::ResourceManager->TexList["star_" + std::to_string(i) + "_" + std::to_string(j)]);
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
	}

}

void GalaxyMenu::InteractWithGalaxy(size_t dt) {
	if (timer_active) {
		// ::::::::::::: timer active ::::::::::::::
		if (menuState == 0) { // main view
			if (currentTapShift(0) == 0.f && currentTapShift(1) == 0.f) {
				// OnTapDown->
				//int y = findGalaxyByPos(Eigen::Vector2f());
			}
			else {
				// OnTapDown->OnMove->
				totalTapShift = Eigen::Vector2f(totalTapShift(0) + currentTapShift(0), totalTapShift(1) + currentTapShift(1));
			}

		}
		if (menuState == 1) { // zoomed galaxy

		}
		if (menuState == 2) { // level select view

		}

		// \_/\_/\_/\_/ timer active \_/\_/\_/\_/
	}
	else {
		// ::::::::::::: timer inactive ::::::::::::::
		if (lastTapPos != Eigen::Vector2f(-9999.9f, -9999.9f)) {
			if (menuState == 0) {// main view
				if (totalTapShift(0) == 0.f && totalTapShift(1) == 0.f){
					// OnTapDown->OnTapUp

					int y = findPlanetByPos(lastTapPos);
					if (y != -1) {
						/*..level selec menu open..*/
						
					}

				}
				else {
					// OnTapDown->OnMove->OnTapUp

				}

			}
			if (menuState == 1) { // zoomed galaxy
				if ((currentTapShift(0) == 0.f) && (currentTapShift(1) == 0.f)) {
					// OnTapDown->OnTapUp

				}
				else {
					// OnTapDown->OnMove->OnTapUp

				}

			}
			if (menuState == 2) { // level select view
				if ((currentTapShift(0) == 0.f) && (currentTapShift(1) == 0.f)) {
					// OnTapDown->OnTapUp

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
		currentTapShift = shift; // shift need to be fixed
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

void GalaxyMenu::showLevelSelectMenu(int index) {



}