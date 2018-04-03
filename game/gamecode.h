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

const int CONST_BRICKMATRIX_WIDTH = 12; // 12
const int CONST_BRICKMATRIX_HEIGHT = 13; // 13
const float CONST_BRICK_WIDTH = 38.65f;
const float CONST_BRICK_HEIGHT = 0.5f*CONST_BRICK_WIDTH;
const float CONST_BRICK_SHIFT_X = 8.2f;
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
    Vector4f Color;
public:
    TBrick();
    void SetVisible(Vector4f color, int locked);
    void SetInvisible();
    void TryDrawAppear(int ipos, int jpos);
    void Update(size_t dt);
    Vector2f GetPosFrom(int ipos, int jpos);
    Vector4f GetColor();
    void Appear(Vector4f color, int locked);
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
   Vector2f Pos;
   std::string TexName;
   int BonusType; //0 - multiplier,  1 - Go-through, 2 - floor
public:
    TBonusFalling(Vector2f pos);
    Vector2f GetPos();
    int GetType();
    void Draw();
    void Update(size_t dt);           
};

struct TBall
{
    Vector2f Pos;
    Vector2f Velocity;
    Vector4f Color;
    std::list<Vector2f> TalePos;
    
    Vector2f BallInBlock;
    Vector2f PrevBallInBlock;
    
    TBall(Vector2f pos, Vector2f velocity, Vector4f color);
    Vector2f GetPos();
    Vector2f GetVelocityVector();
    
    void Go();
    
    void ReflectToLeft();
    void ReflectToRight();
    void ReflectToUp();
    void ReflectToDown();
    
    void TryReflectOnReflector(Vector2f refPos);
   
    void Update(size_t dt);    
};

typedef std::pair<Vector4f, std::string> PairColorTexture;

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
	float lvlWidth;
	float lvlHeight;
    std::string BkgTexture;
    std::string LevelScreenTexture;
    std::string LevelFileName;
    
    Vector2f ReflectorPos;
    
    int LevelState;
    bool PrevLevelStateIsStandby;
    float StateTimer;
    
    TBrick BlockMatrix[CONST_BRICKMATRIX_WIDTH][CONST_BRICKMATRIX_HEIGHT];
    TBlockInstansingList BlockInstansingList;
    
    bool TapInBackBtnArea(const Vector2f& pos);
    
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
    
    Vector4f BallColor;
    
    void ReloadBallInstancingList();
    void RefreshBallInstancingList();
    
    
    void UpdateBallList(size_t dt);
    void MultiplyBalls(Vector2f pos, Vector2f velocity);
    
    Vector2f GetBlock(const Vector2f& pos);
    void InitLevel();
    
    float BonusGothroughTimer;
    float BonusFloorTimer;
    float BonusFloorPosY;
    
    Vector4f ParseColor(const std::string& s);
    void ReloadLevel();
    
public:
    TGameLevel();
    ~TGameLevel();
    
    void FillWithFile(const std::string& filename);
    
	//void SetLevelScale();
	//Vector2f GetLevelScale();

    void SetStandBy();
    void SetLoading(const std::string& bkg, const std::string& levelscreen);
    bool IsLoaded();
    
	virtual void Draw();
	
	void SetPause();
	bool IsPaused();
	void ReleasePause();
	
	virtual void Update(size_t dt);
	virtual void OnTapDown(Vector2f pos);
	virtual void OnTapUp(Vector2f pos);
	virtual void OnFling(Vector2f slideSpeed);
	virtual void OnScroll(Vector2f shift);
};



#endif
