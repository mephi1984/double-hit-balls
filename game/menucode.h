#ifndef MENUCODE_H_INCLUDED
#define MENUCODE_H_INCLUDED

#include "include/Engine.h"
#include "game_area_interface.h"

using namespace SE;


class TGameMenu : public TGameAreaAncestor
{
protected:
	float MenuSpeed;
	float MenuPos;
	
	bool HoldToTap;
	
	int MenuItemCount;
	
	int SelectedGame;
	
	
public:
	TGameMenu();
	virtual void Draw();
	virtual void Update(cardinal dt);
	virtual void OnTapDown(vec2 pos);
	virtual void OnTapUp(vec2 pos);
    virtual void OnTapUpAfterMove(vec2 pos);
	virtual void OnFling(vec2 slideSpeed);
	virtual void OnScroll(vec2 shift);
	
	void OpenNextLevel();
	int GetMenuItemCount();
	void SetMenuItemCount(int menuItemCount);

};



#endif
