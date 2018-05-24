
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
	BOOST_FOREACH(auto menu_pt, config_pt.get_child("Space")) {
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
			/*..strings replace by objects..*/
			BOOST_FOREACH(auto levels_pt, stars_pt.second.get_child("levels")) {
				star.levels.push_back(levels_pt.second.get<std::string>("name", "empty"));
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

void GalaxyMenu::UpdateGalaxyMenu(float s_width, float s_height) {
	/*..Reset..*/
	galaxies_params.clear();
	stars_params.clear();

	/*..Menu ancestor geometry..*/
	float gameScreenWidth = s_width;
	float gameScreenHeight = s_height;
	Eigen::Vector2f gameScreenCenter = Eigen::Vector2f(gameScreenWidth/2,gameScreenHeight/2);

	/*..Menu geometry..*/
	xDimension = menuScale * gameScreenWidth;
	yDimension = menuScale * gameScreenHeight;
	Eigen::Vector2f currentMenuPos = Eigen::Vector2f(gameScreenCenter(0) + (gameScreenWidth/2/*relative to the screen x-dimension*/)*menuPosition(0), gameScreenCenter(1) + (gameScreenHeight/2/*relative to the screen y-dimension*/)*menuPosition(1));

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
				currentMenuPos(0) + (xDimension/2)*galaxies[i].position(0),
				currentMenuPos(1) + (yDimension / 2)*galaxies[i].position(1)),
			Eigen::Vector2f(
				(tex_size(0)*galaxies[i].scale)*menuScale,
				(tex_size(1)*galaxies[i].scale)*menuScale
			)
		));
		
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


}

Eigen::Vector2f GalaxyMenu::textureSizeNormalize(Eigen::Vector2f texVec, int t_type) {
	float tex_ratio = texVec(0)/texVec(1);
	float x_dim, y_dim;
	float Xmax; // Max normalized texture width 
	float Xmin;
	float Ymax; // Max normalized texture height
	float Ymin;
	if (t_type == 0) {
		Xmax = 1280.f; 
		Xmin = 800.f;
		Ymax = 1024.f;
		Ymin = 640.f;
	}
	else { // temp for star textures
		Xmax = 512.f;
		Xmin = 320.f;
		Ymax = 512.f;
		Ymin = 320.f;
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
		/*SE::Console << "c_out::";
		*SE::Console << std::to_string((galaxies_params[i].second(0)));
		*SE::Console << std::to_string((galaxies_params[i].first(1) - galaxies_params[i].second(1) / 2));
		*SE::Console << std::to_string((galaxies_params[i].first(0) + galaxies_params[i].second(0) / 2));
		*SE::Console << std::to_string((galaxies_params[i].first(1) + galaxies_params[i].second(1) / 2));*/
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

void GalaxyMenu::InteractWithGalaxy() {

}