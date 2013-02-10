#ifndef MAIN_CODE_H_INCLUDED
#define MAIN_CODE_H_INCLUDED

#ifdef TARGET_ANDROID
#include "android_api.h"
#endif

#ifdef TARGET_WIN32
#define NOMINMAX 1
#include <Windows.h>
#undef NOMINMAX
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "boost/shared_ptr.hpp"
#include "boost/thread/thread.hpp"
#include "boost/assign.hpp"
#include "boost/bind.hpp"

#include "include/Engine.h"

#include "boost/signal.hpp"//Signal must be included after asio

#include "gamecode.h"
#include "menucode.h"
#include "creditscode.h"
#include "loadingcode.h"

using namespace SE;


extern boost::signal<void (vec2)> OnTapUpSignal;
extern boost::signal<void (vec2)> OnTapDownSignal;
extern boost::signal<void (vec2)> OnFlingSignal;
extern boost::signal<void (vec2)> OnScrollSignal;

extern boost::signal<void ()> OnDrawSignal;

extern const std::string CONST_BLOCK_TEXTURE1;
extern const std::string CONST_BLOCK_TEXTURE2;
extern const std::string CONST_BLOCK_TEXTURE3;

extern const std::string CONST_BONUS_GOTHROUGH_TEXTURE;
extern const std::string CONST_BONUS_MULTIPLIER_TEXTURE;
extern const std::string CONST_BONUS_FLOOR_TEXTURE;

extern const std::string CONST_BALL_TEXTURE;
extern const std::string CONST_BALLGLOW_TEXTURE;

extern const std::string CONST_WALL_LEFT_TEXTURE;
extern const std::string CONST_WALL_RIGHT_TEXTURE;
extern const std::string CONST_WALL_UP_TEXTURE;
extern const std::string CONST_WALL_BONUS_TEXTURE;


extern const std::string CONST_REFLECTOR_TEXTURE;

extern const std::string CONST_BACK_BTN_TEXTURE;

extern const std::string CONST_SLIDE_UP_BTN_TEXTURE;
extern const std::string CONST_TAP_TO_CONTINUE_BTN_TEXTURE;

extern const std::string CONST_BACK_BTN_TEXTURE;
extern const std::string CONST_SLIDE_UP_BTN_TEXTURE;
extern const std::string CONST_TAP_TO_CONTINUE_BTN_TEXTURE;
extern const std::string CONST_LOADING_TEXTURE;
extern const std::string CONST_LOGO_SMALL_TEXTURE;
extern const std::string CONST_CREDITS_TEXTURE;



const int CONST_GAMESTATE_LEVEL = 1;
const int CONST_GAMESTATE_MENU = 0;
const int CONST_GAMESTATE_FROM_MENU_TO_LEVEL = 2;
const int CONST_GAMESTATE_FROM_MENU_TO_CREDITS = 3;
const int CONST_GAMESTATE_CREDITS = 4;
const int CONST_GAMESTATE_FROM_CREDITS_TO_MENU = 5;
const int CONST_GAMESTATE_LOADING = 6;
const int CONST_GAMESTATE_PRELOADING = 7;


class TAndroidApplication : public TApplication
{
protected:
	TGameMenu Menu;
	TGameLevel GameLevel;
	TGameCredits GameCredits;
	TGameLoading GameLoading;
	
	int GameState; 
	float StateTimer;
    
    void TryLoadSavedGame();
    void TrySaveGame();
    
    bool Inited;
    
    std::vector<std::pair<std::string, std::string> > TextureNamesToLoad;
    
    void ApplySignalsToMenu();
    void DisapplySignalsToMenu();
    void ApplySignalsToGame();
    void DisapplySignalsToGame();
    void ApplySignalsToCredits();
    void DisapplySignalsToCredits();

public:
    bool Loaded;
    
	TAndroidApplication() : TApplication(), Loaded(false), Inited(false) { }

    virtual void InnerInit();
    
    virtual void InnerDeinit();
	
	virtual void InnerDraw();
	
	virtual void InnerUpdate(cardinal dt);
	
	void GoFromMenuToGame(int level);
	void GoFromGameToMenu();
	void GoFromMenuToCredits();
	void GoFromCreditsToMenu();
	void MarkSetGameLevelPause();
	void MarkReleaseGameLevelPause();
	
	void OpenNextLevel();
	
	bool IsLoaded();
	
	bool IsInited();
	
	void LoadResources();
	
	virtual void InnerOnTapDown(vec2 p);
	
	virtual void InnerOnTapUp(vec2 p);
	
	virtual void InnerOnMove(vec2 shift);
	
	virtual void OnFling(vec2 v);

};


extern TAndroidApplication* Application;



#endif
