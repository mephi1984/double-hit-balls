#ifndef GALAXY_H
#define GALAXY_H

#include "galaxy_objects.h"

#include "include/Engine.h"
#include <vector>


class GalaxyMenu {
public:

	// ======== All objects =========
	std::vector<StarObject> Stars;
	// ======== All objects =========


	// ======== Main Methods ========
	void InitGalaxy();
	void DrawGalaxy();
	void UpdateGalaxy();
	// ======== Main Methods ========

	void InteractWithGalaxy(); // Prototype for mouse/tap events


	// :::::::::::
	float GetGalaxyPosition();
	// :::::::::::

private:

	// Params
	//Vector2f selfPosition;

};

#endif
