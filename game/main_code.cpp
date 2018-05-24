#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>

#include "include/Engine.h"

#include "menucode.h"

#include "main_code.h"
#include <boost/property_tree/json_parser.hpp>

boost::signals2::signal<void (Vector2f)> OnTapUpSignal;
boost::signals2::signal<void (Vector2f)> OnTapUpAfterMoveSignal;
boost::signals2::signal<void (Vector2f)> OnTapDownSignal;
boost::signals2::signal<void (Vector2f)> OnFlingSignal;
boost::signals2::signal<void (Vector2f)> OnScrollSignal;

boost::signals2::signal<void ()> OnDrawSignal;


const std::string CONST_BLOCK_TEXTURE1 = "block1_mod1";
const std::string CONST_BLOCK_TEXTURE2 = "block2_mod1";
const std::string CONST_BLOCK_TEXTURE3 = "block3_mod1";

const std::string CONST_BONUS_GOTHROUGH_TEXTURE = "bonus_gothrough_mod1";
const std::string CONST_BONUS_MULTIPLIER_TEXTURE = "bonus_multiplier_mod1";
const std::string CONST_BONUS_FLOOR_TEXTURE = "bonus_floor_mod1";

const std::string CONST_BALL_TEXTURE = "ball_mod1";

const std::string CONST_BALLGLOW_TEXTURE = "ball_glow";

const std::string CONST_REFLECTOR_TEXTURE = "reflector_mod1";

const std::string CONST_WALL_LEFT_TEXTURE = "wall_left_mod2";
const std::string CONST_WALL_RIGHT_TEXTURE = "wall_right_mod2";
const std::string CONST_WALL_UP_TEXTURE = "wall_up_mod2";

const std::string CONST_WALL_BONUS_TEXTURE = "wall_bonus_mod2";

const std::string CONST_BACK_BTN_TEXTURE = "back_btn";
const std::string CONST_SLIDE_UP_BTN_TEXTURE = "slide_up_btn";
const std::string CONST_TAP_TO_CONTINUE_BTN_TEXTURE = "tap_to_continue_btn";
const std::string CONST_LOADING_TEXTURE = "loading";
const std::string CONST_LOGO_SMALL_TEXTURE = "logo_small";
const std::string CONST_LOADING_BACKGROUND_BLACK = "loading_background_black";
const std::string CONST_CREDITS_TEXTURE = "credits";

const float CONST_CREDITS_SHOW_TIME = 150.f;


TMyApplication* Application;


void TMyApplication::InnerInit()
{

    Application = this;
    
#ifdef TARGET_WIN32
#ifdef NDEBUG
	//ST::PathToResources = "resources/";
	ST::PathToResources = "../../../assets/";
#else
	ST::PathToResources = "../../../assets/";
#endif
#endif
    
#ifdef TARGET_IOS
    ST::PathToResources = "assets/";
#endif

    if (Console != NULL)
    {
        *Console<<"APP INIT\n";
    }
    srand (static_cast<size_t>(time(NULL)));
	GameState = CONST_GAMESTATE_PRELOADING;
	StateTimer = 0.f;

	/*
	ResourceManager->ShaderManager.AddShader("DefaultShader", "shader1vertex.txt", "shader1fragment.txt");
	ResourceManager->ShaderManager.AddShader("FrameShader", "frameshader_vertex.txt", "frameshader_fragment.txt");
	ResourceManager->ShaderManager.AddShader("BrickShader", "brickshader_vertex.txt", "brickshader_fragment.txt");
	Renderer->PushShader("DefaultShader");
	*/

	ResourceManager->ShaderManager.AddShader("DefaultShader", "shaders/texture-shader.vertex", "shaders/texture-shader.fragment");
	ResourceManager->ShaderManager.AddShader("ColorShader", "shaders/color-shader.vertex", "shaders/color-shader.fragment");
	ResourceManager->ShaderManager.AddShader("FrameShader", "shaders/frameshader_vertex.txt", "shaders/frameshader_fragment.txt");
	ResourceManager->ShaderManager.AddShader("BrickShader", "shaders/brickshader_vertex.txt", "shaders/brickshader_fragment.txt");
	ResourceManager->ShaderManager.AddShader(ParticleEffect::PARTICLE_SHADER, "shaders/particle-shader.vertex", "shaders/particle-shader.fragment");
	Renderer->PushShader("DefaultShader");

	ResourceManager->TexList.AddTexture(CONST_LOADING_BACKGROUND_BLACK + ".png", CONST_LOADING_BACKGROUND_BLACK);
	ResourceManager->TexList.AddTexture(CONST_LOADING_TEXTURE + ".png", CONST_LOADING_TEXTURE);
	ResourceManager->TexList.AddTexture(CONST_LOGO_SMALL_TEXTURE + ".png", CONST_LOGO_SMALL_TEXTURE);

	ResourceManager->TexList.AddTexture("console_bkg.bmp");


	ResourceManager->FrameManager.AddFrameRenderBuffer("LevelBuffer", 512, 512);
	
	OnDrawSignal.connect(boost::bind(&TGameLoading::Draw, boost::ref(GameLoading)));
	Inited = true;

	Renderer->SetOrthoProjection();

	Renderer->SetFullScreenViewport();
	Application->SetGameLevelScreenScale();
	//GameLevel.SetLevelScale();
	EffectsInit();
	
	// ------- UI -------
	ResourceManager->FontManager.AddFont("arial32", "arial32.png", "arial32.txt");
	ResourceManager->FontManager.AddFont("lucon12", "lucon12.png", "lucon12.txt");
	ResourceManager->FontManager.PushFont("lucon12");
	//ResourceManager->newGuiManager.LoadFromConfig("gui_main_menu.json");
	//SetButtonsAction();
	// ------- UI -------

	// TESTS of menu
	if (Menu.GalaxMenu.InitGalaxyMenu("galaxy_ptree.json")) {
		std::cout << "ok" << std::endl;
	}
	else {
		std::cout << "menu error" << std::endl;
	}


}

void TMyApplication::InnerDeinit()
{
    Inited = false;
    Loaded = false;
    if (Console != NULL)
    {
        *Console<<"APP DEINIT\n";
    }

    OnTapUpSignal.disconnect(boost::bind(&TGameLevel::OnTapUp, boost::ref(GameLevel), _1));
    OnTapUpSignal.disconnect(boost::bind(&TGameMenu::OnTapUp, boost::ref(Menu), _1));
    
    OnTapUpAfterMoveSignal.disconnect(boost::bind(&TGameMenu::OnTapUpAfterMove, boost::ref(Menu), _1));
    
    
    OnFlingSignal.disconnect(boost::bind(&TGameLevel::OnFling, boost::ref(GameLevel), _1));
    OnFlingSignal.disconnect(boost::bind(&TGameMenu::OnFling, boost::ref(Menu), _1));
    
    OnScrollSignal.disconnect(boost::bind(&TGameLevel::OnScroll, boost::ref(GameLevel), _1));
    OnScrollSignal.disconnect(boost::bind(&TGameMenu::OnScroll, boost::ref(Menu), _1));
    
    OnTapDownSignal.disconnect(boost::bind(&TGameLevel::OnTapDown, boost::ref(GameLevel), _1));
    OnTapDownSignal.disconnect(boost::bind(&TGameMenu::OnTapDown, boost::ref(Menu), _1));
    OnTapDownSignal.disconnect(boost::bind(&TGameCredits::OnTapDown, boost::ref(GameCredits), _1));
    
    
    OnDrawSignal.disconnect(boost::bind(&TGameLoading::Draw, boost::ref(GameLoading)));
    OnDrawSignal.disconnect(boost::bind(&TGameMenu::Draw, boost::ref(Menu)));
    OnDrawSignal.disconnect(boost::bind(&TGameLevel::Draw, boost::ref(GameLevel)));
    OnDrawSignal.disconnect(boost::bind(&TGameCredits::Draw, boost::ref(GameCredits)));

}

void TMyApplication::InnerOnTapUp(Vector2f p)
{
	OnTapUpSignal(Vector2f(p(0), p(1)));
}

void TMyApplication::InnerOnTapUpAfterMove(Vector2f p)
{
	OnTapUpAfterMoveSignal(Vector2f(p(0), p(1)));
}

void TMyApplication::InnerOnMove(Vector2f p, Vector2f shift)
{
	OnScrollSignal(Vector2f(shift(0), shift(1)));
}
	
void TMyApplication::OnFling(Vector2f v) 
{
}


void TMyApplication::ApplySignalsToMenu()
{

    OnTapUpSignal.connect(boost::bind(&TGameMenu::OnTapUp, boost::ref(Menu), _1));
    OnTapUpAfterMoveSignal.connect(boost::bind(&TGameMenu::OnTapUpAfterMove, boost::ref(Menu), _1));
    OnFlingSignal.connect(boost::bind(&TGameMenu::OnFling, boost::ref(Menu), _1));
    OnScrollSignal.connect(boost::bind(&TGameMenu::OnScroll, boost::ref(Menu), _1));
    OnTapDownSignal.connect(boost::bind(&TGameMenu::OnTapDown, boost::ref(Menu), _1));
    
}


void TMyApplication::DisapplySignalsToMenu()
{
    OnTapUpSignal.disconnect(boost::bind(&TGameMenu::OnTapUp, boost::ref(Menu), _1));
    OnTapUpAfterMoveSignal.disconnect(boost::bind(&TGameMenu::OnTapUpAfterMove, boost::ref(Menu), _1));
    OnFlingSignal.disconnect(boost::bind(&TGameMenu::OnFling, boost::ref(Menu), _1));
    OnScrollSignal.disconnect(boost::bind(&TGameMenu::OnScroll, boost::ref(Menu), _1));
    OnTapDownSignal.disconnect(boost::bind(&TGameMenu::OnTapDown, boost::ref(Menu), _1));
}

void TMyApplication::ApplySignalsToGame()
{
    OnTapUpSignal.connect(boost::bind(&TGameLevel::OnTapUp, boost::ref(GameLevel), _1));
    OnFlingSignal.connect(boost::bind(&TGameLevel::OnFling, boost::ref(GameLevel), _1));
	OnScrollSignal.connect(boost::bind(&TGameLevel::OnScroll, boost::ref(GameLevel), _1));
    //OnTapDownSignal.connect(boost::bind(&TGameLevel::OnTapDown, boost::ref(GameLevel), _1));
    
}

void TMyApplication::DisapplySignalsToGame()
{
    OnTapUpSignal.disconnect(boost::bind(&TGameLevel::OnTapUp, boost::ref(GameLevel), _1));
    OnFlingSignal.disconnect(boost::bind(&TGameLevel::OnFling, boost::ref(GameLevel), _1));
    OnScrollSignal.disconnect(boost::bind(&TGameLevel::OnScroll, boost::ref(GameLevel), _1));
    //OnTapDownSignal.disconnect(boost::bind(&TGameLevel::OnTapDown, boost::ref(GameLevel), _1));
    
}

void TMyApplication::ApplySignalsToCredits()
{
    OnTapDownSignal.connect(boost::bind(&TGameCredits::OnTapDown, boost::ref(GameCredits), _1));
}


void TMyApplication::DisapplySignalsToCredits()
{
    OnTapDownSignal.disconnect(boost::bind(&TGameCredits::OnTapDown, boost::ref(GameCredits), _1));
}

void TMyApplication::LoadResources()
{

    TextureNamesToLoad.clear();

	// :::::::::::::::::::::::::::::::::::::
	// :::::::::::::PTREE LOAD::::::::::::::
    
	boost::property_tree::ptree Textures_pt;
	boost::property_tree::json_parser::read_json(ST::PathToResources + "bg_textures_config.json", Textures_pt);
	std::string bg_ext = ".jpeg";

	// :::::::::::::::::::::::::::::::::::::

    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("main_menu_bkg_left.png", "main_menu_bkg_left"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("main_menu_bkg_right.png", "main_menu_bkg_right"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("select_level.png", "select_level"));
	/*
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock1.png", "shutterstock1"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock2.png", "shutterstock2"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock3.png", "shutterstock3"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock4.png", "shutterstock4"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock5.png", "shutterstock5"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock6.png", "shutterstock6"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock7.png", "shutterstock7"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock8.png", "shutterstock8"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock9.png", "shutterstock9"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock10.png", "shutterstock10"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock11.png", "shutterstock11"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("shutterstock12.png", "shutterstock12"));
	*/
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_1") + bg_ext, "shutterstock1"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_2") + bg_ext, "shutterstock2"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_11") + bg_ext, "shutterstock3"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_13") + bg_ext, "shutterstock4"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_1") + bg_ext, "shutterstock5"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_1") + bg_ext, "shutterstock6"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_1") + bg_ext, "shutterstock7"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_1") + bg_ext, "shutterstock8"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_1") + bg_ext, "shutterstock9"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_1") + bg_ext, "shutterstock10"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_1") + bg_ext, "shutterstock11"));
	TextureNamesToLoad.push_back(std::pair<std::string, std::string>("level_background/" + Textures_pt.get<std::string>("TextureList.bg_1") + bg_ext, "shutterstock12"));

	/*..galaxies and stars/planets Init..*/ // tmp
	std::vector<int> galaxies;
	galaxies.resize(1);
	galaxies[0] = 2;
	for (int i = 0; i < galaxies.size(); i++) {
		TextureNamesToLoad.push_back(std::pair<std::string, std::string>("/galax_menu/galaxies/galaxy_" + std::to_string(i) + ".png", "galaxy_" + std::to_string(i)));
		for (int j = 0; j < galaxies[i]; j++) {
			TextureNamesToLoad.push_back(std::pair<std::string, std::string>("/galax_menu/planets/star_" + std::to_string(i) + "_" + std::to_string(j) + ".png", "star_" + std::to_string(i) + "_" + std::to_string(j)));
		}
	}

    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot1.png", "levelshot1"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot2.png", "levelshot2"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot3.png", "levelshot3"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot4.png", "levelshot4"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot5.png", "levelshot5"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot6.png", "levelshot6"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot7.png", "levelshot7"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot8.png", "levelshot8"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot9.png", "levelshot9"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot10.png", "levelshot10"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot11.png", "levelshot11"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("levelshot12.png", "levelshot12"));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>("game_end.png", "game_end"));
    
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_BLOCK_TEXTURE1 + ".png", CONST_BLOCK_TEXTURE1));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_BLOCK_TEXTURE2 + ".png", CONST_BLOCK_TEXTURE2));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_BLOCK_TEXTURE3 + ".png", CONST_BLOCK_TEXTURE3));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_BONUS_GOTHROUGH_TEXTURE + ".png", CONST_BONUS_GOTHROUGH_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_BONUS_MULTIPLIER_TEXTURE + ".png", CONST_BONUS_MULTIPLIER_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_BONUS_FLOOR_TEXTURE + ".png", CONST_BONUS_FLOOR_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_BALL_TEXTURE + ".png", CONST_BALL_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_BALLGLOW_TEXTURE + ".png", CONST_BALLGLOW_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_WALL_LEFT_TEXTURE + ".png", CONST_WALL_LEFT_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_WALL_RIGHT_TEXTURE + ".png", CONST_WALL_RIGHT_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_WALL_UP_TEXTURE + ".png", CONST_WALL_UP_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_WALL_BONUS_TEXTURE + ".png", CONST_WALL_BONUS_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_REFLECTOR_TEXTURE + ".png", CONST_REFLECTOR_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_BACK_BTN_TEXTURE + ".png", CONST_BACK_BTN_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_SLIDE_UP_BTN_TEXTURE + ".png", CONST_SLIDE_UP_BTN_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_TAP_TO_CONTINUE_BTN_TEXTURE + ".png", CONST_TAP_TO_CONTINUE_BTN_TEXTURE));
    TextureNamesToLoad.push_back(std::pair<std::string, std::string>(CONST_CREDITS_TEXTURE + ".png", CONST_CREDITS_TEXTURE));
    
#ifdef TARGET_IOS
    ResourceManager->SoundManager.LoadMusicLooped("level1ogg.ogg");
#else
	//ResourceManager->SoundManager.LoadMusic("level1ogg.ogg");
#endif
    
	ResourceManager->FontManager.AddFont("droid_sans14", "droid_sans14_font_bitmap.png", "droid_sans14_font_charmap.txt");
	ResourceManager->FontManager.PushFont("droid_sans14");
   
	TryLoadSavedGame();

}

void TMyApplication::TryLoadSavedGame()
{

    std::string fileName = GetFilePathUserData("progress.txt");

    FILE* file = fopen(fileName.c_str(), "r");
	if (file != NULL)
	{
        char line[100];
        char* r;
        r = fgets(line, 100, file);
        if (r != NULL)
        {
            Menu.SetMenuItemCount(toint(r));
        }
		fclose(file);
	}
}

void TMyApplication::TrySaveGame()
{

    std::string fileName = GetFilePathUserData("progress.txt");
    
    std::string s = tostr(Menu.GetMenuItemCount());
    FILE* file = fopen(fileName.c_str(), "w");
	if (file != NULL)
	{
        fputs(s.c_str(), file);
		fflush(file);
		fclose(file);
	}

}


	
void TMyApplication::InnerDraw()
{
	glDisable(GL_DEPTH_TEST);

    OnDrawSignal();


}


void TMyApplication::InnerUpdate(size_t dt)
{

    if (GameState == CONST_GAMESTATE_PRELOADING)
    {
		*SE::Console << "1CONST_GAMESTATE_PRELOADING";
        StateTimer += dt/1000.f;
        if (StateTimer >= 1.f)
        {
            LoadResources();
            GameState = CONST_GAMESTATE_LOADING;
            StateTimer = 0.f;
        }
    }
    else if (GameState == CONST_GAMESTATE_LOADING)
	{
		*SE::Console << "2CONST_GAMESTATE_LOADING";
        StateTimer += dt/1000.f;
        if (StateTimer >= 1.f)
        {
            StateTimer -= 1.f;
        }
        
        if (TextureNamesToLoad.size() != 0)
        {
            ResourceManager->TexList.AddTexture(TextureNamesToLoad.begin()->first, TextureNamesToLoad.begin()->second);
            TextureNamesToLoad.erase(TextureNamesToLoad.begin());
        }
        else
        {
            GameState = CONST_GAMESTATE_MENU;
            ApplySignalsToMenu();
            OnDrawSignal.disconnect(boost::bind(&TGameLoading::Draw, boost::ref(GameLoading)));
            OnDrawSignal.connect(0, boost::bind(&TGameMenu::Draw, boost::ref(Menu)));
            StateTimer = 0.f;
            Loaded = true;
        }
    }
    else if (GameState == CONST_GAMESTATE_LEVEL)
	{
		*SE::Console << "3CONST_GAMESTATE_LEVEL";
        GameLevel.Update(dt);
		EffectsUpdate(dt);
    }
    else if (GameState == CONST_GAMESTATE_MENU)
	{
		//*SE::Console << "4CONST_GAMESTATE_MENU";
        Menu.Update(dt);
		
    }
    else if (GameState == CONST_GAMESTATE_FROM_MENU_TO_LEVEL)
	{
		*SE::Console << "5CONST_GAMESTATE_FROM_MENU_TO_LEVEL";
        GameLevel.Update(dt);
        if (GameLevel.IsLoaded())
        {
			//*SE::Console << "5CONST_GAMESTATE_FROM_MENU_TO_LEVEL";
            GameState = CONST_GAMESTATE_LEVEL;
            OnDrawSignal.disconnect(boost::bind(&TGameMenu::Draw, boost::ref(Menu)));
            //CONNECT SLOT
            DisapplySignalsToMenu();
            ApplySignalsToGame();
        }
    }
    else if (GameState == CONST_GAMESTATE_FROM_MENU_TO_CREDITS)
	{
		*SE::Console << "6CONST_GAMESTATE_FROM_MENU_TO_CREDITS";
        Menu.Update(dt);
        GameCredits.Update(dt);
        StateTimer -= dt;
        if (StateTimer <= 0.f)
        {
            GameState = CONST_GAMESTATE_CREDITS;
            OnDrawSignal.disconnect(boost::bind(&TGameMenu::Draw, boost::ref(Menu)));
            ApplySignalsToCredits();
            StateTimer = 0.f;
        }
    }
    else if (GameState == CONST_GAMESTATE_CREDITS)
	{
		//*SE::Console << "7CONST_GAMESTATE_CREDITS";
        GameCredits.Update(dt);
    }
    else if (GameState == CONST_GAMESTATE_FROM_CREDITS_TO_MENU)
    {
        Menu.Update(dt);
        GameCredits.Update(dt);
        StateTimer -= dt;
        if (StateTimer <= 0.f)
        {
            GameState = CONST_GAMESTATE_MENU;
            StateTimer = 0.f;
            ApplySignalsToMenu();
            OnDrawSignal.disconnect(boost::bind(&TGameCredits::Draw, boost::ref(GameCredits)));
        }
    }
}


void TMyApplication::GoFromMenuToGame(int level)
{
//#ifndef TARGET_IOS
//	ResourceManager->SoundManager.PlayMusicLooped("level1ogg.ogg");
//#endif

    GameLevel.FillWithFile(ST::PathToResources + "level"+tostr(level+1)+".txt");
    GameLevel.SetLoading("shutterstock" + tostr(level+1), "levelshot"+tostr(level+1));    
    GameState = CONST_GAMESTATE_FROM_MENU_TO_LEVEL;
    OnDrawSignal.connect(1, boost::bind(&TGameLevel::Draw, boost::ref(GameLevel)));
    
    DisapplySignalsToMenu();
 
}

void TMyApplication::GoFromGameToMenu()
{
    //#ifndef TARGET_IOS
//	ResourceManager->SoundManager.StopMusic("level1ogg.ogg");
//#endif
    TrySaveGame();
    DisapplySignalsToGame();
    ApplySignalsToMenu();
    GameState = CONST_GAMESTATE_MENU;
    OnDrawSignal.disconnect(boost::bind(&TGameLevel::Draw, boost::ref(GameLevel)));
}

void TMyApplication::GoFromMenuToCredits()
{
    GameState = CONST_GAMESTATE_FROM_MENU_TO_CREDITS;
    StateTimer = CONST_CREDITS_SHOW_TIME;
    GameCredits.StartAppear();
    OnDrawSignal.connect(1, boost::bind(&TGameCredits::Draw, boost::ref(GameCredits)));
    DisapplySignalsToMenu();

}

void TMyApplication::GoFromCreditsToMenu()
{
    
    GameState = CONST_GAMESTATE_FROM_CREDITS_TO_MENU;
    StateTimer = CONST_CREDITS_SHOW_TIME;
    GameCredits.StartDisappear();
    OnDrawSignal.connect(0, boost::bind(&TGameMenu::Draw, boost::ref(Menu)));
    
    DisapplySignalsToCredits();
}

void TMyApplication::MarkSetGameLevelPause()
{
    OnDrawSignal.connect(0, boost::bind(&TGameMenu::Draw, boost::ref(Menu)));
}

void TMyApplication::MarkReleaseGameLevelPause()
{
    OnDrawSignal.disconnect(boost::bind(&TGameMenu::Draw, boost::ref(Menu)));
}
	

void TMyApplication::OpenNextLevel()
{
    Menu.OpenNextLevel();
}

bool TMyApplication::IsLoaded()
{
    return Loaded;
}

bool TMyApplication::IsInited()
{
    return Inited;
}

void TMyApplication::SetGameLevelScreenScale()
{
	if ((((float)Renderer->GetScreenWidth())/((float)Renderer->GetScreenHeight())) >= levelScreenRatio)
	{
		levelScreenHeight = (float)Renderer->GetScreenHeight();
		levelScreenWidth = levelScreenHeight * levelScreenRatio;
	} else 
	{
		levelScreenWidth = (float)Renderer->GetScreenWidth();
		levelScreenHeight = levelScreenWidth / levelScreenRatio;
	}
}

float TMyApplication::GetGameLevelScreenWidth()
{
	return levelScreenWidth;
}

float TMyApplication::GetGameLevelScreenHeight()
{
	return levelScreenHeight;
}

void TMyApplication::InnerOnMouseDown(TMouseState& mouseState) {
	/*
	if (mouseState.LeftButtonPressed) {
		if ((float)mouseState.X >= (Renderer->GetScreenWidth())*0.25f && (float)mouseState.X <= (Renderer->GetScreenWidth())*0.75f && (float)mouseState.Y >= (Renderer->GetScreenHeight())*0.25f && (float)mouseState.Y <= (Renderer->GetScreenHeight())*0.75f) {
			// some tmp code
			Application->GoFromMenuToGame(1);
		}
	}
	*/
	OnTapDownSignal(Vector2f(mouseState.X, ((Renderer->GetScreenHeight()) - mouseState.Y))); // Temp mouse down action for WIN32
}

void TMyApplication::InnerOnMouseMove(TMouseState& mouseState) {
	
}

void TMyApplication::EffectsInit() {

	boost::property_tree::ptree JSONsource;
	boost::property_tree::ptree JSONconfig;
	std::string effectJSON;
	boost::property_tree::json_parser::read_json(ST::PathToResources + "config.json", JSONconfig);

	// LEFT
	effectJSON = JSONconfig.get<std::string>("lefteffect");
	boost::property_tree::json_parser::read_json(ST::PathToResources + effectJSON, JSONsource);
	lsparkler.parse(JSONsource); // parse JSON
	lsparkler.load(); // load textures
	// RIGHT
	effectJSON = JSONconfig.get<std::string>("righteffect");
	boost::property_tree::json_parser::read_json(ST::PathToResources + effectJSON, JSONsource);
	rsparkler.parse(JSONsource);
	rsparkler.load();
	// TOP
	effectJSON = JSONconfig.get<std::string>("topeffect");
	boost::property_tree::json_parser::read_json(ST::PathToResources + effectJSON, JSONsource);
	tsparkler.parse(JSONsource);
	tsparkler.load();
	// BOTTOM
	effectJSON = JSONconfig.get<std::string>("boteffect");
	boost::property_tree::json_parser::read_json(ST::PathToResources + effectJSON, JSONsource);
	bsparkler.parse(JSONsource);
	bsparkler.load();
	// Level finish
	effectJSON = JSONconfig.get<std::string>("lvlFinish");
	boost::property_tree::json_parser::read_json(ST::PathToResources + effectJSON, JSONsource);
	lvlFirework.parse(JSONsource);
	lvlFirework.load();

	float width = Renderer->GetScreenWidth();
	float height = Renderer->GetScreenHeight();

	lsparkler.setCoords({ width / 2, height / 2, 0 });
	rsparkler.setCoords({ width / 2, height / 2, 0 });
	tsparkler.setCoords({ width / 2, height / 2, 0 });
	bsparkler.setCoords({ width / 2, height / 2, 0 });
	lvlFirework.setCoords({ width / 2, 0, 0 });
}

void TMyApplication::EffectsUpdate(size_t dt) {
	lsparkler.update(dt / 1000.f);
	rsparkler.update(dt / 1000.f);
	tsparkler.update(dt / 1000.f);
	bsparkler.update(dt / 1000.f);
	lvlFirework.update(dt / 1000.f);
}
void TMyApplication::EffectsDraw() {
	lsparkler.draw();
	rsparkler.draw();
	tsparkler.draw();
	bsparkler.draw();
	lvlFirework.draw();
}

void TMyApplication::hitSpark(std::string direct,Vector2f Pos) {
	if (direct == "left") {
		lsparkler.setCoords({ Pos(0),Pos(1),0 });
		lsparkler.stopSpawn();
		lsparkler.startSpawn();

	}
	if (direct == "right") {
		rsparkler.setCoords({ Pos(0),Pos(1),0 });
		rsparkler.stopSpawn();
		rsparkler.startSpawn();
	}
	if (direct == "up") {
		tsparkler.setCoords({ Pos(0),Pos(1),0 });
		tsparkler.stopSpawn();
		tsparkler.startSpawn();
	}
	if (direct == "down") {
		bsparkler.setCoords({ Pos(0),Pos(1),0 });
		bsparkler.stopSpawn();
		bsparkler.startSpawn();
	}
}

void TMyApplication::fireworkEffect() {
	lvlFirework.stopSpawn();
	lvlFirework.startSpawn();
}

void TMyApplication::SetButtonsAction () {
	auto backBtn = ResourceManager->newGuiManager.findWidgetByName("backButton");
	if (backBtn) {
		backBtn->onMouseDownSignal.connect([this, backBtn](Vector2f pos, int touchNumber) {
			this->GameLevel.SetPause();
			this->GameLevel.PrevLevelStateIsStandby = true;
		});
	}
}