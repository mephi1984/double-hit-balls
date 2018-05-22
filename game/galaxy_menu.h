#ifndef GALAXY_MENU_H
#define GALAXY_MENU_H

#include <vector>
#include <string>

#include "galaxy.h"

class GalaxyMenu {
public:

	GalaxyMenu();
	~GalaxyMenu();

	// ======== All objects =========
	std::vector<Galaxy> galaxies;
	// ======== All objects =========


	// ======== Main Methods ========
	void InitGalaxyMenu(std::string config);
	void DrawGalaxyMenu();
	void UpdateGalaxyMenu();
	// ======== Main Methods ========

	void InteractWithGalaxy(/*..Vector/Vector/Int..*/); // Prototype for mouse/tap events

	// Params
	Eigen::Vector2f menuPosition;
	float menuScale;

private:

	void readSaveData(); // inner init method


};


#endif