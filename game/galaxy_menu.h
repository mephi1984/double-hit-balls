#ifndef GALAXY_MENU_H
#define GALAXY_MENU_H

#include <vector>
#include <string>

#include "galaxy.h"
#include "level_interior.h"

#include "include/Engine.h"

#include <boost/property_tree/json_parser.hpp>
#include <math.h>

class GalaxyMenu {
public:

	GalaxyMenu();
	~GalaxyMenu();

	// ======== All objects =========
	std::vector<Galaxy> galaxies;
	// ======== All objects =========


	// ======== Main Methods ========
	bool InitGalaxyMenu(std::string config_json, float scale = 1.f);
	void DrawGalaxyMenu();
	void UpdateGalaxyMenu(float s_width, float s_height, size_t dt);

	
	// ======== Main Methods ========

	void InteractWithGalaxy(size_t dt); // Prototype for mouse/tap events

	// ::#Params#::
	Eigen::Vector2f menuPosition = Eigen::Vector2f(0.f, 0.f); // relative to the screen center(0.f,0.f means center) (not const!!)
	float menuScale = 1.f; // (not const!!)
	int currentWindowWidth;
	int currentWindowHeight;
	float xDimension;
	float yDimension;
	float anchorSize = 1.f;
	std::vector<std::pair<Eigen::Vector2f, Eigen::Vector2f>> galaxies_params; // ::position/dimensions::
	std::vector<std::vector<std::pair<Eigen::Vector2f, Eigen::Vector2f>>> stars_params;

	/*..Outer Interact..*/
	void tapDown(Eigen::Vector2f pos);
	void tapUp(Eigen::Vector2f pos);
	void tapMove(Eigen::Vector2f shift);

	void setTimerActivity(bool value);


private:


	void readSaveData(); // inner init method

	/*..calc methodes..*/
	Eigen::Vector2f textureSizeNormalize(Eigen::Vector2f texVec, int t_type = 0/*0-galaxy, 1-stars*/);
	float val_clamp(float v, float min, float max);
	Eigen::Vector2f findCorner(int x_c, int y_c);
	float graterV(float first_v, float second_v);
	float lowerV(float first_v, float second_v);
	int negativeV(float val);

	/*..states..*/
	int menuState = 0; // 0 - all galaxies are visible, 1 - zoomed to current galaxy(reserved) , 2 - level select menu
	int starIndex = -1;
	int galaxyIndex = 0; // zoomed galaxy
	int planetHoverIndex = -1;

	/*..Interact params..*/
	bool timer_active = false;
	float interact_timer = 0.f; // reset
	Eigen::Vector2f lastTapPos = Eigen::Vector2f(-9999.9f, -9999.9f); // reset
	Eigen::Vector2f currentTapShift; // reset
	Eigen::Vector2f totalTapShift; // reset

	/*..coefficients..*/
	Eigen::Vector2f menu_offset = Eigen::Vector2f(0, 0);

	/*..Interact methods..*/
	void takeInFocus(int g_index, int s_index = -1);
	int findGalaxyByPos(Eigen::Vector2f pos);
	int findPlanetByPos(Eigen::Vector2f pos);

	bool checkMenuBound(Eigen::Vector2f pos);

	/*..draw methodes..*/
	void drawBorder(Eigen::Vector2f lb_, Eigen::Vector2f rt_, float scale, std::string mode);
	void drawLevelInterior(int star = -1, int button = -1);

};


#endif