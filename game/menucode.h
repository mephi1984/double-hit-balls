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
	virtual void Update(size_t dt);
	virtual void OnTapDown(Vector2f pos);
	virtual void OnTapUp(Vector2f pos);
    virtual void OnTapUpAfterMove(Vector2f pos);
	virtual void OnFling(Vector2f slideSpeed);
	virtual void OnScroll(Vector2f shift);
	
	void OpenNextLevel();
	int GetMenuItemCount();
	void SetMenuItemCount(int menuItemCount);

};



#endif
