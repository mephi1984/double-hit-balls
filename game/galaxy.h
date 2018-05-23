#ifndef GALAXY_H
#define GALAXY_H

#include "include/Engine.h"

#include "galaxy_objects.h"

class Galaxy {
public:

	// ====== All objects =======
	std::vector<StarObject> Stars;
	// ====== All objects =======

	// ::#Params#::
	std::string name;
	float scale;
	Eigen::Vector2f position;
	std::string texture;

private:



};


#endif
