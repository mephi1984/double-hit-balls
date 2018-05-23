#ifndef GALAXY_MENU_H
#define GALAXY_MENU_H

#include <vector>
#include <string>

#include "galaxy.h"

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
	void UpdateGalaxyMenu();
	// ======== Main Methods ========

	void InteractWithGalaxy(/*..Vector/Vector/Int..*/); // Prototype for mouse/tap events

	// ::#Params#::
	Eigen::Vector2f menuPosition = Eigen::Vector2f(0.f, 0.f); // relative to the screen center(0.f,0.f means center) (not const!!)
	float menuScale = 1.f; // (not const!!)
	float xDimension;
	float yDimension;

private:

	void readSaveData(); // inner init method

	Eigen::Vector2f textureSizeNormalize(Eigen::Vector2f texVec, int t_type = 0/*0-galaxy, 1-stars*/);
	float val_clamp(float v, float min, float max);

};


#endif