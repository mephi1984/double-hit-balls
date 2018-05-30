#ifndef GALAXY_OBJECTS_H
#define GALAXY_OBJECTS_H

#include "include/Engine.h"
#include <string>

class LevelSelection {
public:

	std::vector<std::string> levels;

	// #params
	//float buttons_plane_Yoffset = 0.2f; // relative to the screen
	std::pair<Eigen::Vector2f, Eigen::Vector2f> params;
	std::pair<Eigen::Vector2f, Eigen::Vector2f> buttons_plane;
	std::vector<std::pair<Eigen::Vector2f, Eigen::Vector2f>> buttons;

	Eigen::Vector2f offset = Eigen::Vector2f(0.f, 0.f); // offset
	Eigen::Vector2f dim = Eigen::Vector2f(0.6f, 0.9f); // dimensions, y - is not const, relative to the levels count

	/*..buttons plane params..*/
	Eigen::Vector2f plane_size = Eigen::Vector2f(1.0f, 1.0f); // y- is not const, relative to the levels count
	Eigen::Vector2f plane_pos = Eigen::Vector2f(0.f, 0.f); // relative to center

	/*..buttons params..*/
	int columns = 3;
	float border_y_offset = 0.018f;
	float border_x_offset = 0.018f;
	float buttons_offset = 0.018f;
	float buttons_ratio = 1.6f; 

private:


};

class StarObject {
public:

	//StarObject(Eigen::Vector2f pos, std::string id/*star name*/);
	//~StarObject();

	// :::::::::::::
	Eigen::Vector2f GetPosition();
	// :::::::::::::

	// ::#Levels#::
	LevelSelection selectionMenu;

	// ::#Params#::
	std::string texture;
	std::string name;
	Eigen::Vector2f position;
	float scale;

private:


};


#endif