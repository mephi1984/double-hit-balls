#ifndef GAMECODE_H_INCLUDED
#define GAMECODE_H_INCLUDED

#include "include/Engine.h"
#include "game_area_interface.h"

#include <list>

using namespace SE;


const int CONST_BRICKSTATE_VISIBLE = 3;
const int CONST_BRICKSTATE_DISAPPEAR = 2;
const int CONST_BRICKSTATE_INVISIBLE = 1;
const int CONST_BRICKSTATE_APPEAR = 0;

const float CONST_BRICK_DISAPPEAR_TIME = 200.f;
const float CONST_BRICK_APPEAR_TIME = 200.f;

const int CONST_BRICKMATRIX_WIDTH = 12;
const int CONST_BRICKMATRIX_HEIGHT = 13;
const float CONST_BRICK_WIDTH = 37.5f;
const float CONST_BRICK_HEIGHT = 0.5f*CONST_BRICK_WIDTH;
const float CONST_BRICK_SHIFT_X = 15.f;
const float CONST_BRICK_SHIFT_Y = -16.f;

const float CONST_BONUS_APPEAR_TIME = 400.f;
const float CONST_BONUS_FALL_SPEED = 100.f;
const float CONST_BONUS_GOTHROUGH_TIME = 8000.f;
const float CONST_BONUS_CATCH_DISTANCE_X = 65.f;
const float CONST_BONUS_CATCH_DISTANCE_Y = 20.f;

const float CONST_BONUS_FLOOR_TIME = 8000.f;
const float CONST_BONUS_FLOOR_APPEAR_SPEED = 50.f;

const int CONST_BONUS_TYPE_MULTIPLIER = 0;
const int CONST_BONUS_TYPE_GOTHROUGH = 1;
const int CONST_BONUS_TYPE_FLOOR = 2;


class TBrick
{
protected:
    int InitialLocked;
    int Locked; //0, 1 or 2
    int State;
    float StateTimer;
    vec4 Color;
public:
    TBrick();
    void SetVisible(vec4 color, int locked);
    void SetInvisible();
    void TryDrawAppear(int ipos, int jpos);
    void Update(cardinal dt);
    vec2 GetPosFrom(int ipos, int jpos);
    vec4 GetColor();
    void Appear(vec4 color, int locked);
    void Appear();
    void Hit();
    void Disappear();
    int GetLocked();
    bool CanReact();
};

class TBonusFalling
{
protected:
   float Lifetime;
   vec2 Pos;
   std::string TexName;
   int BonusType; //0 - multiplier,  1 - Go-through, 2 - floor
public:
    TBonusFalling(vec2 pos);
    vec2 GetPos();
    int GetType();
    void Draw();
    void Update(cardinal dt);           
};

struct TBall
{
    vec2 Pos;
    vec2 Velocity;
    vec4 Color;
    std::list<vec2> TalePos;
    
    vec2 BallInBlock;
    vec2 PrevBallInBlock;
    
    TBall(vec2 pos, vec2 velocity, vec4 color);
    vec2 GetPos();
    vec2 GetVelocityVector();
    
    void Go();
    
    void ReflectToLeft();
    void ReflectToRight();
    void ReflectToUp();
    void ReflectToDown();
    
    void TryReflectOnReflector(vec2 refPos);
   
    void Update(cardinal dt);    
};

typedef std::pair<vec4, std::string> PairColorTexture;

struct TBlockInstansingList
{
    //color-texture  ---> and list of triangles for this color
    std::list<std::pair<PairColorTexture, TTriangleList>> ColorBlockList;
};


struct TBallInstancingList
{
    std::map<int, TTriangleList> BallAndGlowList;
};

class TGameLevel : public TGameAreaAncestor
{
protected:
    std::string BkgTexture;
    std::string LevelScreenTexture;
    std::string LevelFileName;
    
    vec2 ReflectorPos;
    
    int LevelState;
    bool PrevLevelStateIsStandby;
    float StateTimer;
    
    TBrick BlockMatrix[CONST_BRICKMATRIX_WIDTH][CONST_BRICKMATRIX_HEIGHT];
    TBlockInstansingList BlockInstansingList;
    
    bool TapInBackBtnArea(const vec2& pos);
    
    void ReloadBlockInstansingList();
    void SetFinished();
    void SetFinishFreeze();
    
    void DrawBuffer();
    void DrawPauseButtons();
    
    void DrawBallInstancingList();
 
    bool RenderBufferReady;
    
    float OutScale;
    
    float OutScaleVelocity;
    
    void TryGoToMenu();
    
    std::list<TBonusFalling> BonusFallingList;
    
    std::list<TBall> BallList;
    
    TBallInstancingList BallInstancingList;
    
    vec4 BallColor;
    
    void ReloadBallInstancingList();
    void RefreshBallInstancingList();
    
    
    void UpdateBallList(cardinal dt);
    void MultiplyBalls(vec2 pos, vec2 velocity);
    
    vec2 GetBlock(const vec2& pos);
    void InitLevel();
    
    float BonusGothroughTimer;
    float BonusFloorTimer;
    float BonusFloorPosY;
    
    vec4 ParseColor(const std::string& s);
    void ReloadLevel();
    
public:
    TGameLevel();
    ~TGameLevel();
    
    void FillWithFile(const std::string& filename);
    
    void SetStandBy();
    void SetLoading(const std::string& bkg, const std::string& levelscreen);
    bool IsLoaded();
    
	virtual void Draw();
	
	void SetPause();
	bool IsPaused();
	void ReleasePause();
	
	virtual void Update(cardinal dt);
	virtual void OnTapDown(vec2 pos);
	virtual void OnTapUp(vec2 pos);
	virtual void OnFling(vec2 slideSpeed);
	virtual void OnScroll(vec2 shift);
};



#endif
