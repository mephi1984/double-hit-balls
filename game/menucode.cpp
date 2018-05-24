#include "menucode.h"
#include "main_code.h"


const float CONST_SPEED_NEG_ACCELERATION_K = 0.03f;

const float CONST_MENU_POS_K = 0.25f;

const float CONST_MENU_WINDOW_DISTANCE = 240.f + 40.f;

const float CONST_SHIFT_ON_SIDE = 0.f;

TGameMenu::TGameMenu()
	: MenuSpeed(0)
	, MenuPos(0)
	, HoldToTap(false)
	, MenuItemCount(1)
	, SelectedGame(-1)
{
}


void TGameMenu::Draw()
{
#define NEW_MENU_DRAW
//	*SE::Console << "TGameMenu::Draw";
    CheckGlError("Draw TGameMenu");
    RenderUniform1i("sel", 0);
    RenderUniform1f("Transparency", 1.f);
    float bkgShift = MenuPos*0.1f - 100.f;
#ifndef NEW_MENU_DRAW
	glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList["main_menu_bkg_left"]);	
	Renderer->DrawRect(Vector2f(bkgShift,0.f), Vector2f(Renderer->GetScreenWidth()+bkgShift,Renderer->GetScreenHeight()));
	glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList["main_menu_bkg_right"]);
	Renderer->DrawRect(Vector2f(Renderer->GetScreenWidth()+bkgShift,0.f), Vector2f(960.f+bkgShift,Renderer->GetScreenHeight()));
#else

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	/*..GalaxMenu draw..*/
	GalaxMenu.DrawGalaxyMenu();
#endif

#ifndef NEW_MENU_DRAW
	if (SelectedGame == 0)
	{
        RenderUniform1i("sel", 1);
    }
    else
    {
        RenderUniform1i("sel", 0);
    }
    
    for (int i=0; i<MenuItemCount; i++)
    {
        glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList["levelshot"+tostr(i+1)]);
        Renderer->DrawRect(Vector2f(160.f+MenuPos+CONST_MENU_WINDOW_DISTANCE*i,80.f), Vector2f(160.f+240.f+MenuPos+CONST_MENU_WINDOW_DISTANCE*i, 240.f));
    }
    
    if (MenuItemCount == 12)
    {
        glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList["game_end"]);
        Renderer->DrawRect(Vector2f(160.f+MenuPos+CONST_MENU_WINDOW_DISTANCE*12,160.f - 64.f), Vector2f(160.f+256.f+MenuPos+CONST_MENU_WINDOW_DISTANCE*12, 160.f + 64.f));
    
    }
    
    
    
    glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList["select_level"]);
    Renderer->DrawRect(Vector2f(240.f-128.f, 241.f), Vector2f(240.f+128.f, 305.f));
    
	glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList["logo_small"]);
    Renderer->DrawRect(Vector2f(240.f-128.f+15.f, 0.f), Vector2f(240.f+128.f+15.f, 64.f));
#endif

    CheckGlError("Draw TGameMenu 2");

	
}

void TGameMenu::Update(size_t dt)
{
	/*..Galaxy Menu..*/
	GalaxMenu.UpdateGalaxyMenu((float)SE::Renderer->GetScreenWidth(), (float)SE::Renderer->GetScreenHeight());

	if (HoldToTap)
		return;
	// если кнопка не нажата
	float k;
	
	MenuPos = MenuPos + MenuSpeed*dt/1000.f;

    int menuItemToShowCount;
    
    if (MenuItemCount == 12)
    {
        menuItemToShowCount = 13;
    }
    else
    {
        menuItemToShowCount = MenuItemCount;
    }
	
	float acc;
	
	if (MenuItemCount == 1)
	{
	   k = 3;
    }
	else
	{
        k = fabs( - MenuPos - ((menuItemToShowCount-1)*CONST_MENU_WINDOW_DISTANCE*0.5f)) / ((MenuItemCount-1)*CONST_MENU_WINDOW_DISTANCE*0.5f);  
    }
    
	acc = -MenuSpeed * CONST_SPEED_NEG_ACCELERATION_K * k;
	
    
	float xleft = (- MenuPos + CONST_SHIFT_ON_SIDE - (menuItemToShowCount-1)*CONST_MENU_WINDOW_DISTANCE);
	
	if (xleft < 0)
	{
        xleft = 0;
    }
    
    acc += xleft*CONST_MENU_POS_K;
    
    float xright = (MenuPos - CONST_SHIFT_ON_SIDE);
	
	if (xright < 0)
	{
        xright = 0;
    }
    
    acc -= xright*CONST_MENU_POS_K;
    
	if ((MenuSpeed + acc*dt) * MenuSpeed < 0) //Check if MenuSpeed changes direction
	{
		MenuSpeed = 0;
	}
	else
	{
		MenuSpeed += acc*dt;
	}
    
    
}

void TGameMenu::OnTapDown(Vector2f pos)
{
	*SE::Console << "TGameMenu::OnTapDown";
    HoldToTap = true;

    if (pos(1)<64.f && pos(0)>=265.f-128.f && pos(0)<=265.f+128.f)
    {
        Application->GoFromMenuToCredits();
        return;
    }
	
    Vector2f realPos = pos - Vector2f(MenuPos, 0);
    
    if (realPos(1) >= 80.f && realPos(1) <= 240.f)
    {
        float x = realPos(0) - 160.f;
        
        int p = 0;
        while (x >280.f)
        {
            x -= 280.f;
            p++;
        }
        
        if (x <= 240.f && x >= 0.f && (p<MenuItemCount))
        {
            SelectedGame = p;
            return;
        }
    }
    

    SelectedGame = -1;

}

void TGameMenu::OnTapUp(Vector2f pos)
{
	*SE::Console << "TGameMenu::OnTapUp";
	HoldToTap = false;

	if (SelectedGame != -1)
	{
        MenuPos = -CONST_MENU_WINDOW_DISTANCE*SelectedGame-40.f;
        Application->GoFromMenuToGame(SelectedGame);
        SelectedGame = -1;
    }
}

void TGameMenu::OnTapUpAfterMove(Vector2f pos)
{
    HoldToTap = false;
}


void TGameMenu::OnFling(Vector2f slideSpeed)
{
	*SE::Console << "TGameMenu::OnFling";
	HoldToTap = false;
	MenuSpeed = slideSpeed(0);
}

void TGameMenu::OnScroll(Vector2f shift)
{
	MenuPos = MenuPos - shift(0);
}

void TGameMenu::OpenNextLevel()
{
	*SE::Console << "TGameMenu::OpenNextLevel";
    if (MenuItemCount < 12)
    {
        MenuItemCount++;
    }
}

int TGameMenu::GetMenuItemCount()
{
	*SE::Console << "TGameMenu::GetMenuItemCount";
    return MenuItemCount;
}

void TGameMenu::SetMenuItemCount(int menuItemCount)
{
	*SE::Console << "TGameMenu::SetMenuItemCount";
    MenuItemCount = menuItemCount;
}
