#ifndef GALAXY_OBJECTS_H
#define GALAXY_OBJECTS_H

#include "include/Engine.h"
#include <string>

class StarObject {
public:

	//StarObject(Eigen::Vector2f pos, std::string id/*star name*/);
	//~StarObject();

	// :::::::::::::
	Eigen::Vector2f GetPosition();
	// :::::::::::::

	// ::#Levels#::
	std::vector<std::string> levels;

	// ::#Params#::
	std::string selfTexture;
	std::string name;
	Eigen::Vector2f selfPosition;
	float selfScale;

private:


};


#endif