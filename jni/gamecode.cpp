#include "gamecode.h"
#include "main_code.h"

const int CONST_LEVELSTATE_STANDBY = 0;
const int CONST_LEVELSTATE_PLAYING = 1;
const int CONST_LEVELSTATE_GO_TO_PAUSE = 2;
const int CONST_LEVELSTATE_PAUSE = 3;
const int CONST_LEVELSTATE_LOADING = 4;
const int CONST_LEVELSTATE_NODRAW = 5;
const int CONST_LEVELSTATE_FINISH_FREEZE = 6;
const int CONST_LEVELSTATE_FINISHED = 7;

const float CONST_TIMER_LOADING = 150.f;

const float CONST_PAUSE_APPEAR_TIME = 150.f;

const float CONST_FINISH_FREEZE_TIME = 1000.f;
const float CONST_FINISHING_TIME = 250.f;

const float CONST_BALL_VELOCITY = 200.f;

const vec2 CONST_SLIDE_UP_POS(240.f, 64.f);

const vec2 CONST_TAP_TO_CONTINUE_POS(240.f, 200.f);

bool operator<(const PairColorTexture& p1, const PairColorTexture& p2)
{
    if (p1.second == p2.second)
    {
		if (p1.first.v[0] == p2.first.v[0])
		{
			if (p1.first.v[1] == p2.first.v[1])
			{
				if (p1.first.v[2] == p2.first.v[2])
				{
					return p1.first.v[3] < p2.first.v[3];
				}
				else
				{
					return p1.first.v[2] < p2.first.v[2];
				}
			}
			else
			{
				return p1.first.v[1] < p2.first.v[1];
			}
		}
		else
		{
			return p1.first.v[0] < p2.first.v[0];
		}
    }
    
    return (p1.second < p2.second);
}


TBrick::TBrick()
    : State(CONST_BRICKSTATE_VISIBLE)
    , StateTimer(0.f)
    , Color(vec4(0.f, 0.f, 0.f, 1.f))
    , Locked(0)
    , InitialLocked(0)
{
}

void TBrick::SetVisible(vec4 color, int locked)
{
    State = CONST_BRICKSTATE_VISIBLE;
    Color = color;
    InitialLocked = locked;
    Locked = InitialLocked;
}

void TBrick::SetInvisible()
{
    State = CONST_BRICKSTATE_INVISIBLE;
}

void TBrick::TryDrawAppear(int ipos, int jpos)
{
    vec2 centerPos = GetPosFrom(ipos, jpos);
    
    vec2 blockHalfSize = vec2(0.5f*CONST_BRICK_WIDTH, 0.5f*CONST_BRICK_HEIGHT);
    
    std::string texName;
    if (Locked == 2)
    {
        texName = CONST_BLOCK_TEXTURE3;
    }
    else if (Locked == 1)
    {
        texName = CONST_BLOCK_TEXTURE2;
    }
    else
    {
        texName = CONST_BLOCK_TEXTURE1;
    }
                
    if (State == CONST_BRICKSTATE_DISAPPEAR)
    {
                
        RenderUniform1f("Transparency", StateTimer/CONST_BRICK_DISAPPEAR_TIME);
        RenderUniform4fv("BrickColor", Color.v);
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[texName]);
        Renderer->DrawRect(centerPos - blockHalfSize, centerPos + blockHalfSize);
    }
    else if (State == CONST_BRICKSTATE_APPEAR)
    {
                
        RenderUniform1f("Transparency", 1.f - StateTimer/CONST_BRICK_APPEAR_TIME);
        RenderUniform4fv("BrickColor", Color.v);
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[texName]);
        Renderer->DrawRect(centerPos - blockHalfSize, centerPos + blockHalfSize);
    }
}

void TBrick::Update(cardinal dt)
{
    if (State == CONST_BRICKSTATE_DISAPPEAR)
    {
        StateTimer -= dt;
        if (StateTimer < 0.f)
        {
            StateTimer = 0.f;
            State = CONST_BRICKSTATE_INVISIBLE;
        }
    }
    
    if (State == CONST_BRICKSTATE_APPEAR)
    {
        StateTimer -= dt;
        if (StateTimer < 0.f)
        {
            StateTimer = 0.f;
            State = CONST_BRICKSTATE_VISIBLE;
        }
    }
}

vec4 TBrick::GetColor()
{
    return Color;
}

vec2 TBrick::GetPosFrom(int ipos, int jpos)
{
    const vec2 BorderShift(CONST_BRICK_SHIFT_X, CONST_BRICK_SHIFT_Y);
    
    return BorderShift+vec2(CONST_BRICK_WIDTH*ipos + 0.5f*CONST_BRICK_WIDTH, 320.f - CONST_BRICK_HEIGHT*(jpos)-0.5f*CONST_BRICK_HEIGHT);

}

void TBrick::Disappear()
{
    StateTimer = CONST_BRICK_DISAPPEAR_TIME;
    State = CONST_BRICKSTATE_DISAPPEAR;
}

void TBrick::Hit()
{
    if (Locked == 0)
    {
        Disappear();
    }
    else
    {
        Locked--;
    }
}

void TBrick::Appear(vec4 color, int locked)
{
    StateTimer = CONST_BRICK_APPEAR_TIME;
    State = CONST_BRICKSTATE_APPEAR;
    Color = color;
    InitialLocked = locked;
    Locked = InitialLocked;
}

void TBrick::Appear()
{
    Appear(Color, InitialLocked);
}

int TBrick::GetLocked()
{
    return Locked;
}

bool TBrick::CanReact()
{
    return (State == CONST_BRICKSTATE_VISIBLE) || (State == CONST_BRICKSTATE_APPEAR);
}

//===========================================
//===========================================
//===========================================

TBonusFalling::TBonusFalling(vec2 pos)
    : BonusType(rand() % 3)
    , Pos(pos)
    , Lifetime(0.f)
{
    
    if (BonusType == 0)
    {
        TexName = CONST_BONUS_MULTIPLIER_TEXTURE;
    }
    
    if (BonusType == 1)
    {
        TexName = CONST_BONUS_GOTHROUGH_TEXTURE;
    }
    
    if (BonusType == 2)
    {
        TexName = CONST_BONUS_FLOOR_TEXTURE;
    }

}

vec2 TBonusFalling::GetPos()
{
    return Pos;
}

int TBonusFalling::GetType()
{
    return BonusType;
}


void TBonusFalling::Draw()
{
    vec2 BonusHalfSize = vec2(16.f, 16.f);
    
    float transparency = min(Lifetime/CONST_BONUS_APPEAR_TIME , 1.f);
    
    RenderUniform4fv("BrickColor", vec4(1.0f, 1.0f, 1.0f, 1.0f).v);
    RenderUniform1f("Transparency", transparency);
    
    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[TexName]);
	Renderer->DrawRect(Pos - BonusHalfSize, Pos + BonusHalfSize);

}


void TBonusFalling::Update(cardinal dt)
{
    Lifetime += dt;
    Pos.v[1] -= dt * CONST_BONUS_FALL_SPEED / 1000.f;
}

//===========================================
//===========================================
//===========================================


TBall::TBall(vec2 pos, vec2 velocity, vec4 color)
    : Pos(pos)
    , Velocity(velocity)
    , Color(color)
{
    //must be 5
    TalePos.push_back(Pos);
    TalePos.push_back(Pos);
    TalePos.push_back(Pos);
    TalePos.push_back(Pos);
    TalePos.push_back(Pos);
}

vec2 TBall::GetPos()
{
    return Pos;
}

vec2 TBall::GetVelocityVector()
{
    return Velocity;
}

void TBall::Go()
{
    Velocity = Normalize(vec2(2.f, 1.f));
}

void TBall::ReflectToLeft()
{
    if (Velocity.v[0] > 0.f)
    {
        Velocity.v[0] = - Velocity.v[0];
    }
}

void TBall::ReflectToRight()
{
    if (Velocity.v[0] < 0.f)
    {
        Velocity.v[0] = - Velocity.v[0];
    }
}

void TBall::ReflectToUp()
{
    if (Velocity.v[1] < 0.f)
    {
        Velocity.v[1] = - Velocity.v[1];
    }
}

void TBall::ReflectToDown()
{
    if (Velocity.v[1] > 0.f)
    {
        Velocity.v[1] = - Velocity.v[1];
    }
}

//function for reflector surface.
float ReflectorPlaneFunction(float shift)
{
    /*   _______
        /       \
        |       |
    Something like this
    
    */
    
    if (shift>=-70.f && shift < -40.f)
    {
        float t = (shift+70.f)/30.f; //0 to 1
        return 9.f + 21.f * t;
    }
    
    if (shift>=-40.f && shift < 40.f)
    {
        return 30.f;
    }
    
    
    if (shift >= 40.f && shift <= 70.f)
    {
        float t = (70.f - shift)/30.f; //1 to 0
        return 9.f + 21.f * t;
    }
    
    return 0.f;
}

void TBall::TryReflectOnReflector(vec2 refPos)
{
    const float reflectionShiftY = 13.f;
    const float reflectionMaxHeight = 30.f;
    
    
    
    if ((Pos.v[1] < reflectionMaxHeight + reflectionShiftY) && Pos.v[1] > 0.0f && Velocity.v[1] < 0)
	{
        float dy = ReflectorPlaneFunction(Pos.v[0] - refPos.v[0]);

		if (dy > 0 && (dy + reflectionShiftY > Pos.v[1]))  
		{
			float shift = (Pos.v[0] - refPos.v[0]) / 128.f;
			shift = min(shift, 0.5f);
			shift = max(shift, -0.5f);
			vec2 n = Normalize(vec2(shift, 1.0f));
		    
		    Velocity = Velocity - n * 2.f * (Velocity.v[0]*n.v[0] + Velocity.v[1]*n.v[1]);
            if ((Velocity.v[1] <= 0) || (fabs(Velocity.v[0]/Velocity.v[1]) > 4.f))
	        {
                Velocity.v[0] = 4.f*sign(Velocity.v[0]);
                Velocity.v[1] = 1.f;
                Velocity = Normalize(Velocity);
            }
		}
	}
}


void TBall::Update(cardinal dt)
{
    Pos += Velocity * (CONST_BALL_VELOCITY * dt / 1000.f);
    
    TalePos.push_back(Pos);
    
    if (TalePos.size() > 4)
    {
        TalePos.erase(TalePos.begin());
    }
}
    
//===========================================
//===========================================
//===========================================


TGameLevel::TGameLevel()
{
    
    BkgTexture = "bkg";
    
    RenderBufferReady = false;
    PrevLevelStateIsStandby = false;
    
    OutScale = 1.f;
    OutScaleVelocity = 0.f;
    
    StateTimer = 0.f;
    
    LevelState = CONST_LEVELSTATE_NODRAW;
    
    BallColor = vec4(0.2f, 0.8f, 1.0f, 1.0f);

	BonusFloorPosY = 0.f;

}

TGameLevel::~TGameLevel()
{
}



void TGameLevel::ReloadBlockInstansingList()
{
    std::map<int, std::string> ConstTextureBlockMap = boost::assign::map_list_of (0,CONST_BLOCK_TEXTURE1) (1,CONST_BLOCK_TEXTURE2) (2,CONST_BLOCK_TEXTURE3);
    
    std::pair<vec4, std::string> tempPair;
    BlockInstansingList.ColorBlockList.clear();
     
    for (int i=0; i<CONST_BRICKMATRIX_WIDTH; i++)
    {
        for (int j=0; j<CONST_BRICKMATRIX_HEIGHT; j++)
        {
            if (BlockMatrix[i][j].CanReact())
            {
                tempPair.first = BlockMatrix[i][j].GetColor();
                tempPair.second = ConstTextureBlockMap[BlockMatrix[i][j].GetLocked()];
               
                std::list<std::pair<PairColorTexture, TTriangleList>>::iterator itr = BlockInstansingList.ColorBlockList.end();
                
                for (auto b = BlockInstansingList.ColorBlockList.begin(); b != BlockInstansingList.ColorBlockList.end(); ++b)
                {
                    if (b->first == tempPair)
                    {
                        itr = b;
                    }
                }
                
                if (itr == BlockInstansingList.ColorBlockList.end())
                {
                    BlockInstansingList.ColorBlockList.push_back(std::pair<PairColorTexture, TTriangleList>(tempPair, TTriangleList()));
                    itr = BlockInstansingList.ColorBlockList.end();
                    
                    itr--;
                }
                
                vec2 posFrom = BlockMatrix[i][j].GetPosFrom(i,j) + vec2(-0.5f*CONST_BRICK_WIDTH, -0.5f*CONST_BRICK_HEIGHT);
                vec2 posTo = BlockMatrix[i][j].GetPosFrom(i,j) + vec2(+0.5f*CONST_BRICK_WIDTH, +0.5f*CONST_BRICK_HEIGHT);
                
                itr->second.Data += MakeDataTriangleList(posFrom, posTo);
                
            }
        }
    }

	for (auto it = BlockInstansingList.ColorBlockList.begin(); it != BlockInstansingList.ColorBlockList.end(); ++it)
	{
		it->second.RefreshBuffer();
	}
	
}

vec2 TGameLevel::GetBlock(const vec2& pos)
{    
    
	int x = static_cast<int>((pos.v[0] - CONST_BRICK_SHIFT_X) / CONST_BRICK_WIDTH);
	int y = static_cast<int>((320.0f + CONST_BRICK_SHIFT_Y - pos.v[1]) / CONST_BRICK_HEIGHT);
	
	if (x < 0)
		x = 0;
		
	if (x > CONST_BRICKMATRIX_WIDTH-1)
		x = CONST_BRICKMATRIX_WIDTH-1;
		
	if (y < 0)
		y = 0;
		
	if (y > CONST_BRICKMATRIX_HEIGHT-1)
		y = CONST_BRICKMATRIX_HEIGHT-1;
	
	return vec2(x, y);
}

bool TGameLevel::TapInBackBtnArea(const vec2& pos)
{
    return (pos.v[1] > 320.f - 64.f) && (pos.v[0]>=240.f-75.f) && (pos.v[0]<=240.f+75.f);
}

void TGameLevel::SetFinishFreeze()
{
    StateTimer = CONST_FINISH_FREEZE_TIME;
    
    LevelState = CONST_LEVELSTATE_FINISH_FREEZE;
    
}

void TGameLevel::SetFinished()
{
    StateTimer = CONST_FINISHING_TIME;
    
    LevelState = CONST_LEVELSTATE_FINISHED;
    
    OutScale = 1.f;
}

vec4 TGameLevel::ParseColor(const std::string& s)
{
    vec4 r;
    std::string ss(s);
    
    int i = ss.find(", ");
    int c = toint(ss.substr(0, i));
    ss.erase(0, i+2);
    r.v[0] = c / 255.f;

    i = ss.find(", ");
    c = toint(ss.substr(0, i));
    ss.erase(0, i+2);
    r.v[1] = c / 255.f;
    
    i = ss.find(", ");
    c = toint(ss.substr(0, i));
    ss.erase(0, i+2);
    r.v[2] = c / 255.f;
    
    c = toint(ss);
    r.v[3] = c / 255.f;
    
    return r;
}

void TGameLevel::ReloadLevel()
{
    cardinal byteCount;
    boost::shared_array<char> file = CreateMemFromFile<char>(LevelFileName, byteCount);
    std::string fileString(&file[0]);
    char c;
    //int n = 0;
 
    
    std::vector<std::string> rows;
    int rowLen;
    while (fileString.size() > 0)
    {
		rowLen = fileString.find(fendl);
        rows.push_back(fileString.substr(0, rowLen));
        fileString.erase(0, rowLen+2);
    }
    
    std::vector<std::string>::iterator rowIterator = rows.begin();
    BallColor = ParseColor(*rowIterator);
    ++rowIterator;
    
    std::vector<vec4> colors;
    
    vec4 tc;
    
    while (*rowIterator != "Colormap")
    {
        tc = ParseColor(*rowIterator);
        colors.push_back(tc);
        ++rowIterator;
    }
    
    std::vector<std::string>::iterator rowColorIterator;
    std::vector<std::string>::iterator rowLockIterator;
    
    rowColorIterator = rowIterator + 1;

    rowLockIterator = rowColorIterator + 14;
    

    
    for (int j=0; j<CONST_BRICKMATRIX_HEIGHT; j++)
    {
        for (int i=0; i<CONST_BRICKMATRIX_WIDTH; i++)
        {
            c = (*rowLockIterator)[i];
            if (c == 0x30)
            {
                BlockMatrix[i][j].SetInvisible();
            }
            else
            {
                BlockMatrix[i][j].Appear(colors[(*rowColorIterator)[i] - 0x30], (c-0x30)-1);
            }
        }
        rowLockIterator++;
        rowColorIterator++;
    }

    ReloadBlockInstansingList();

}

void TGameLevel::FillWithFile(const std::string& filename)
{
    LevelFileName = filename;
    ReloadLevel();
}

void TGameLevel::SetStandBy()
{
    ReloadLevel();
    InitLevel();
    LevelState = CONST_LEVELSTATE_STANDBY;
}

void TGameLevel::SetLoading(const std::string& bkg, const std::string& levelscreen)    
{
    BkgTexture = bkg;
    LevelScreenTexture = levelscreen;
    InitLevel();
    StateTimer = CONST_TIMER_LOADING;
    LevelState = CONST_LEVELSTATE_LOADING;
}

void TGameLevel::InitLevel()
{
    ReflectorPos = vec2(240, 16 + 13);
    vec2 ballPos = vec2(240, 80);
    
    BallList.clear();
    BallList.push_back(TBall(ballPos, vec2(0, 0), BallColor));
    
    BallList.begin()->BallInBlock = GetBlock(ballPos);
	BallList.begin()->PrevBallInBlock = BallList.begin()->BallInBlock;
	
	BonusGothroughTimer = 0.f;
    BonusFloorTimer = 0.f;
    
    BonusFallingList.clear();
    
    RenderBufferReady = false;
    
    ReloadBallInstancingList();

	BonusFloorPosY = 0.f;

}

bool TGameLevel::IsLoaded()
{
    return (LevelState == CONST_LEVELSTATE_STANDBY);
}

void TGameLevel::Draw()
{
    
    if (LevelState == CONST_LEVELSTATE_NODRAW)
    {
        CheckGlError();
        return;
    }
    
    if (LevelState == CONST_LEVELSTATE_LOADING)
    {
        Renderer->PushMatrix();
        float scale = 1.f - 0.5f*StateTimer/CONST_TIMER_LOADING;
        if (scale < 0.5f)
            scale = 0.5f;
        
        if (scale > 1.f)
            scale = 1.f;

		Renderer->TranslateMatrix(vec3(Renderer->GetMatrixWidth() * 0.5f, Renderer->GetMatrixHeight() * 0.5f, 0));
        Renderer->ScaleMatrix(scale);
		Renderer->TranslateMatrix(-vec3(Renderer->GetMatrixWidth() * 0.5f, Renderer->GetMatrixHeight() * 0.5f, 0));
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[LevelScreenTexture]);
        Renderer->DrawRect(vec2(0, 0), vec2(480.f, 320.f));
        Renderer->PopMatrix();
        CheckGlError();
        return;
    }
    
    
    bool mustShowButtons = ((LevelState == CONST_LEVELSTATE_PAUSE) || (LevelState == CONST_LEVELSTATE_GO_TO_PAUSE));
    
    bool pause = (mustShowButtons || (LevelState == CONST_LEVELSTATE_FINISHED));
    
    
    bool renderBufferReady = RenderBufferReady;
    
    if (pause && renderBufferReady)
    {
        
		//See also below (same method)
        Renderer->PushMatrix();
		Renderer->TranslateMatrix(vec3(Renderer->GetMatrixWidth() * 0.5f, Renderer->GetMatrixHeight() * 0.5f, 0));
        Renderer->ScaleMatrix(OutScale);
		Renderer->TranslateMatrix(-vec3(Renderer->GetMatrixWidth() * 0.5f, Renderer->GetMatrixHeight() * 0.5f, 0));
        DrawBuffer();
        
        if (mustShowButtons)
        {
            DrawPauseButtons();
        }
        Renderer->PopMatrix();
        CheckGlError();
    	return;
    }
    
    if (pause && !renderBufferReady)
    {
       
        Renderer->SwitchToFrameBuffer("LevelBuffer");
        Renderer->SetProjectionMatrix(480.f, 320.f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        CheckGlError();
    }
    
    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[BkgTexture]);
	Renderer->DrawRect(vec2(0, 0), vec2(480.f, 320.f));
	
	std::list<TBall>::iterator iBall;
    
	Renderer->PushShader("BrickShader");
	
	
	for (int i=0; i<CONST_BRICKMATRIX_WIDTH; i++)
    {
        for (int j=0; j<CONST_BRICKMATRIX_HEIGHT; j++)
        {
            BlockMatrix[i][j].TryDrawAppear(i,j); //Draws only appearing/disappearing blocks
        }
    }
    
    RenderUniform1f("Transparency", 1.f);

    std::list<std::pair<PairColorTexture, TTriangleList>>::iterator colorBlockIterator;
    for (colorBlockIterator = BlockInstansingList.ColorBlockList.begin(); colorBlockIterator != BlockInstansingList.ColorBlockList.end(); ++colorBlockIterator)
    {
        RenderUniform4fv("BrickColor", colorBlockIterator->first.first.v);
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[colorBlockIterator->first.second]);
        
        Renderer->DrawTriangleList(colorBlockIterator->second);
    }
    
    std::list<TBonusFalling>::iterator iBonus;
    
    for (iBonus = BonusFallingList.begin(); iBonus != BonusFallingList.end(); ++iBonus)
    {
        iBonus->Draw();
    }
    
    DrawBallInstancingList();
    
    Renderer->PopShader();
    
	
    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_REFLECTOR_TEXTURE]);
	Renderer->DrawRect(vec2(-128.f, -16.f)+ReflectorPos, vec2(128.f, 16.f)+ReflectorPos);
	
    
    const vec2 wallUpPos1(240.f-256.f, 320.f-64.f);
    const vec2 wallUpPos2(240.f+256.f, 320.f);
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_WALL_UP_TEXTURE]);
	Renderer->DrawRect(wallUpPos1, wallUpPos2);
	
	const vec2 wallLeftPos1(0.f, 320.f - 512.f);
	const vec2 wallLeftPos2(32.f, 320.f);
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_WALL_LEFT_TEXTURE]);
	Renderer->DrawRect(wallLeftPos1, wallLeftPos2);
	
	const vec2 wallRightPos1(480.f-32.f, 320.f - 512.f);
	const vec2 wallRightPos2(480.f, 320.f);
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_WALL_RIGHT_TEXTURE]);
	Renderer->DrawRect(wallRightPos1, wallRightPos2);
	

	if (BonusFloorTimer>0.f)
	{
        
        const vec2 wallDownPos(240.f, BonusFloorPosY);
        
	    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_WALL_BONUS_TEXTURE]);
	    
	    Renderer->DrawRect(vec2(-256.f, -16.f)+wallDownPos, vec2(256.f, 16.f)+wallDownPos);
    }
	
    
    if (!pause)
    {
        RenderUniform1f("Transparency", 1.f);
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_BACK_BTN_TEXTURE]);
        const vec2 BackBtnPos(240.f, 320.f - 32.f - 20.f);
        Renderer->DrawRect(vec2(-128.f, -32.f)+BackBtnPos, vec2(128.f, 32.f)+BackBtnPos);
    }
	
	if (pause && !renderBufferReady)
    {
        
         //ololo
		//See also above (same method)
        Renderer->SwitchToScreen();
		Renderer->SetFullScreenViewport();
        
        Renderer->PushMatrix();
		//Renderer->LoadIdentity();
		Renderer->TranslateMatrix(vec3(Renderer->GetMatrixWidth() * 0.5f, Renderer->GetMatrixHeight() * 0.5f, 0));
        Renderer->ScaleMatrix(OutScale);
		Renderer->TranslateMatrix(-vec3(Renderer->GetMatrixWidth() * 0.5f, Renderer->GetMatrixHeight() * 0.5f, 0));
        DrawBuffer();
        if (mustShowButtons)
        {
            DrawPauseButtons();
        }
        Renderer->PopMatrix();
        RenderBufferReady = true;
        CheckGlError();
    }
    CheckGlError();
}

void TGameLevel::DrawPauseButtons()
{
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_SLIDE_UP_BTN_TEXTURE]);
        Renderer->DrawRect(vec2(-128.f, -64.f)+CONST_SLIDE_UP_POS, vec2(128.f, 64.f)+CONST_SLIDE_UP_POS);
        
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_TAP_TO_CONTINUE_BTN_TEXTURE]);
        Renderer->DrawRect(vec2(-128.f, -128.f)+CONST_TAP_TO_CONTINUE_POS, vec2(128.f, 128.f)+CONST_TAP_TO_CONTINUE_POS);

}

void TGameLevel::DrawBallInstancingList()
{
    RenderUniform1f("Transparency", 1.f);
    RenderUniform4fv("BrickColor", BallColor.v);
    
    if (BonusGothroughTimer > 0.f)
    {
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_BALLGLOW_TEXTURE]);
        
        Renderer->DrawTriangleList(BallInstancingList.BallAndGlowList[1]);
    }
    
    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_BALL_TEXTURE]);
        
    Renderer->DrawTriangleList(BallInstancingList.BallAndGlowList[0]);
    
}

void TGameLevel::DrawBuffer()
{
  
    Renderer->PushShader("FrameShader");
    float brightness;
    if (CONST_LEVELSTATE_GO_TO_PAUSE)
    {
        brightness = 0.3f + 0.7f * StateTimer / CONST_PAUSE_APPEAR_TIME;
    }
    else
    {
        brightness = 0.3f;
    }
    
    RenderUniform1f("Brightness", brightness);
    glBindTexture(GL_TEXTURE_2D,ResourceManager->FrameManager.GetFrameTexture("LevelBuffer"));
    
	//Matrix switched to identity
    //vec2 RectPos = vec2(-1, -1);
    //vec2 RectSize = vec2(2, 2);
	vec2 RectPos = vec2(240.f, 160.f);
	vec2 RectSize = vec2(240.f, 160.f);

    Renderer->DrawRect(RectPos-RectSize, RectPos+RectSize);

    Renderer->PopShader(); 
    
    CheckGlError();
}

void TGameLevel::SetPause()
{
    OutScaleVelocity = 0.f;
    OutScale = 1.f;
    RenderBufferReady = false;
    LevelState = CONST_LEVELSTATE_GO_TO_PAUSE;
    StateTimer = CONST_PAUSE_APPEAR_TIME;
    App->MarkSetGameLevelPause();
}

void TGameLevel::ReleasePause()
{
    RenderBufferReady = false;
    
    if (PrevLevelStateIsStandby)
    {
        LevelState = CONST_LEVELSTATE_STANDBY;
        PrevLevelStateIsStandby = false;
    }
    else
    {
        LevelState = CONST_LEVELSTATE_PLAYING;
    }
    App->MarkReleaseGameLevelPause();
}

bool TGameLevel::IsPaused()
{
    return ((LevelState == CONST_LEVELSTATE_PAUSE) || (LevelState == CONST_LEVELSTATE_GO_TO_PAUSE) || (LevelState == CONST_LEVELSTATE_FINISHED));
}

void TGameLevel::Update(cardinal dt)
{
    
    if (LevelState == CONST_LEVELSTATE_NODRAW)
    {
        return;
    }
    
    if (LevelState == CONST_LEVELSTATE_GO_TO_PAUSE)
    {
        StateTimer -= dt;
        if (StateTimer <= 0.f)
        {
            StateTimer = 0.f;
            LevelState = CONST_LEVELSTATE_PAUSE;
        }
        
        return;
    }

    if (LevelState == CONST_LEVELSTATE_PAUSE)
    {
        OutScale += OutScaleVelocity * dt;
        TryGoToMenu();
        CheckGlError();
        return;
    }
    
    if (LevelState == CONST_LEVELSTATE_FINISHED)
    {
        StateTimer -= dt;
        
        OutScale = StateTimer/CONST_FINISHING_TIME;
        
        if (StateTimer <= 0.f)
        {
            TryGoToMenu();
        }
        CheckGlError();
        return;
    }
    

    if (LevelState == CONST_LEVELSTATE_LOADING)
    {
        StateTimer -= dt;
        if (StateTimer <= 0.f)
        {
            StateTimer = 0.f;
            RenderBufferReady = false;
            LevelState = CONST_LEVELSTATE_STANDBY;
        }
    }
    
    if (LevelState == CONST_LEVELSTATE_FINISH_FREEZE)
    {
        StateTimer -= dt;
        
        if (StateTimer <= 0.f)
        {
            SetFinished();
            CheckGlError();
            return;
        }
        
        //To make the whole scene like freeze
        dt = static_cast<cardinal>(dt / max((CONST_FINISH_FREEZE_TIME-StateTimer)/100.f, 1.f));
        
    }
    
    if (BonusGothroughTimer > 0.f)
    {
        BonusGothroughTimer -= dt;
        BonusGothroughTimer = max(BonusGothroughTimer, 0.f);
    }
    
    if (BonusFloorTimer > 0.f)
    {
        BonusFloorTimer -= dt;
        BonusFloorTimer = max(BonusFloorTimer, 0.f);
    }
    
    UpdateBallList(dt);
    
	
	for (int i=0; i<CONST_BRICKMATRIX_WIDTH; i++)
    {
        for (int j=0; j<CONST_BRICKMATRIX_HEIGHT; j++)
        {
            BlockMatrix[i][j].Update(dt);
        }
    }
    
    std::list<TBonusFalling>::iterator iBonus = BonusFallingList.begin();

	while (iBonus != BonusFallingList.end())
    {
        iBonus->Update(dt);
        
        if ((fabs(ReflectorPos.v[0] - iBonus->GetPos().v[0])<CONST_BONUS_CATCH_DISTANCE_X) &&
            (fabs(ReflectorPos.v[1] - iBonus->GetPos().v[1])<CONST_BONUS_CATCH_DISTANCE_Y))
        {
            int bonusType = iBonus->GetType();
            iBonus = BonusFallingList.erase(iBonus);

		
            if (bonusType == CONST_BONUS_TYPE_GOTHROUGH)
            {
                BonusGothroughTimer = CONST_BONUS_GOTHROUGH_TIME;
            }
            else if (bonusType == CONST_BONUS_TYPE_MULTIPLIER)
            {
                vec2 pos = BallList.begin()->GetPos();
                vec2 velocity = BallList.begin()->GetVelocityVector();
                MultiplyBalls(pos, velocity);
            }
            else if (bonusType == CONST_BONUS_TYPE_FLOOR)
            {
                BonusFloorTimer = CONST_BONUS_FLOOR_TIME;
            }
            
        }
        else if (iBonus->GetPos().v[1] < -15.f)
        {
            iBonus = BonusFallingList.erase(iBonus);
        }

		if (iBonus != BonusFallingList.end())
		{
			++iBonus;
		}
    }
    
    //Bonus floor pos
    if (BonusFloorTimer/CONST_BONUS_GOTHROUGH_TIME < 0.2f)
    {
        //Bonus must go down until 0.f
        if (BonusFloorPosY > 0.f)
        {
            BonusFloorPosY -= CONST_BONUS_FLOOR_APPEAR_SPEED * dt / 1000.f;
            BonusFloorPosY = max(BonusFloorPosY, 0.f);
        }
        
    }
    else
    {
       //Bonus must go up until 16.f
        if (BonusFloorPosY < 16.f)
        {
            BonusFloorPosY += CONST_BONUS_FLOOR_APPEAR_SPEED * dt / 1000.f;
            BonusFloorPosY = min(BonusFloorPosY, 16.f);
        }
    }
    
    
    bool noMoreBlocks = true;
    
    for (int i=0; i<CONST_BRICKMATRIX_WIDTH; i++)
    {
        for (int j=0; j<CONST_BRICKMATRIX_HEIGHT; j++)
        {
            if (BlockMatrix[i][j].CanReact())
            {
                noMoreBlocks = false;
            }
        }
    }
    
    
    if (noMoreBlocks && LevelState != CONST_LEVELSTATE_FINISH_FREEZE)
    {
        App->OpenNextLevel();
        App->MarkSetGameLevelPause();
        SetFinishFreeze();
        CheckGlError();
        
    }
    
    if (BallList.size() == 0 && LevelState != CONST_LEVELSTATE_FINISH_FREEZE)
    {
        SetStandBy();
    }
    
}



void TGameLevel::ReloadBallInstancingList()
{
    //Changing this function? Don't forget to change next one!
    
    BallInstancingList.BallAndGlowList.clear();
    
    std::list<TBall>::iterator i;
    /*
    vec3 p1;
    vec3 p2;
    vec3 p3;
    vec3 p4;
    
    vec2 t1 = vec2(0.0f, 0.0f);
    vec2 t2 = vec2(0.0f, 1.0f);
    vec2 t3 = vec2(1.0f, 1.0f);
    vec2 t4 = vec2(1.0f, 0.0f);
    */
    
    
    for (i = BallList.begin(); i != BallList.end(); ++i)
    {
        /*
        p1 = vec3(i->Pos, 0.f) + vec3(-8.f, -8.f, 0.f);
        p2 = vec3(i->Pos, 0.f) + vec3(-8.f, +8.f, 0.f);
        p3 = vec3(i->Pos, 0.f) + vec3(+8.f, +8.f, 0.f);
        p4 = vec3(i->Pos, 0.f) + vec3(+8.f, -8.f, 0.f);
        
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p1);
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p2);
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p3);
        
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p3);
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p4);
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p1);
        
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t1);
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t2);
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t3);
        
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t3);
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t4);
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t1);
         */
        
        BallInstancingList.BallAndGlowList[0].Data += MakeDataTriangleList(i->Pos + vec2(-8.f, -8.f), i->Pos + vec2(8.f, 8.f));
        
        //Replace6PointsInTriangleList(BallInstancingList.BallAndGlowList[0].Data, n, i->Pos + vec2(-8.f, -8.f), i->Pos + vec2(8.f, 8.f));
        
        //n += 6;
    }
    
    std::list<vec2>::iterator j;
    
    for (i = BallList.begin(); i != BallList.end(); ++i)
    {
        for (j = i->TalePos.begin(); j != i->TalePos.end(); ++j)
        {
            /*
            p1 = vec3(*j, 0.f) + vec3(-16.f, -16.f, 0.f);
            p2 = vec3(*j, 0.f) + vec3(-16.f, +16.f, 0.f);
            p3 = vec3(*j, 0.f) + vec3(+16.f, +16.f, 0.f);
            p4 = vec3(*j, 0.f) + vec3(+16.f, -16.f, 0.f);
        
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p1);
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p2);
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p3);
        
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p3);
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p4);
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(p1);
        
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t1);
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t2);
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t3);
        
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t3);
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t4);
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].push_back(t1);
            */
            BallInstancingList.BallAndGlowList[1].Data += MakeDataTriangleList(*j + vec2(-16.f, -16.f), *j + vec2(16.f, 16.f));
            //Replace6PointsInTriangleList(BallInstancingList.BallAndGlowList[1].Data, n, *j + vec2(-16.f, -16.f), *j + vec2(16.f, 16.f));
            
            //n += 6;
        }
    }

	//std::map<int, TTriangleList>::iterator it;
	
	auto it = BallInstancingList.BallAndGlowList.begin();

	for (; it !=  BallInstancingList.BallAndGlowList.end(); ++it)
	//for (auto it =  BallInstancingList.BallAndGlowList.begin(); it !=  BallInstancingList.BallAndGlowList.end(); ++it)
	
	{
		it->second.RefreshBuffer();
	}
	
}

void TGameLevel::RefreshBallInstancingList()
{
    //Changing this function? Don't forget to change previous one!
    /*
    vec3 p1;
    vec3 p2;
    vec3 p3;
    vec3 p4;
    
    vec2 t1 = vec2(0.0f, 0.0f);
    vec2 t2 = vec2(0.0f, 1.0f);
    vec2 t3 = vec2(1.0f, 1.0f);
    vec2 t4 = vec2(1.0f, 0.0f);
    */
    int n = 0;
    int m = 0;
    
    std::list<TBall>::iterator i;
    
    for (i = BallList.begin(); i != BallList.end(); ++i)
    {
        /*
        p1 = vec3(i->Pos, 0.f) + vec3(-8.f, -8.f, 0.f);
        p2 = vec3(i->Pos, 0.f) + vec3(-8.f, +8.f, 0.f);
        p3 = vec3(i->Pos, 0.f) + vec3(+8.f, +8.f, 0.f);
        p4 = vec3(i->Pos, 0.f) + vec3(+8.f, -8.f, 0.f);
        
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p1;
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p2;
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p3;
        
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p3;
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p4;
        BallInstancingList.BallAndGlowList[0].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p1;
        
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t1;
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t2;
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t3;
        
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t3;
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t4;
        BallInstancingList.BallAndGlowList[0].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t1;
        */
        
        Replace6PointsInTriangleList(BallInstancingList.BallAndGlowList[0].Data, n, i->Pos + vec2(-8.f, -8.f), i->Pos + vec2(8.f, 8.f));
        
        n += 6;
    }
    
    std::list<vec2>::iterator j;
    
    n = 0;
    m = 0;
    
    for (i = BallList.begin(); i != BallList.end(); ++i)
    {
        for (j = i->TalePos.begin(); j != i->TalePos.end(); ++j)
        {
            /*
            p1 = vec3(*j, 0.f) + vec3(-16.f, -16.f, 0.f);
            p2 = vec3(*j, 0.f) + vec3(-16.f, +16.f, 0.f);
            p3 = vec3(*j, 0.f) + vec3(+16.f, +16.f, 0.f);
            p4 = vec3(*j, 0.f) + vec3(+16.f, -16.f, 0.f);
        
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p1;
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p2;
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p3;
        
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p3;
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p4;
            BallInstancingList.BallAndGlowList[1].Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][n++] = p1;
        
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t1;
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t2;
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t3;
        
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t3;
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t4;
            BallInstancingList.BallAndGlowList[1].Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][m++] = t1;*/
            //BallInstancingList.BallAndGlowList[1].Data += MakeDataTriangleList();
            
            Replace6PointsInTriangleList(BallInstancingList.BallAndGlowList[1].Data, n, *j + vec2(-16.f, -16.f), *j + vec2(16.f, 16.f));
            
            n += 6;
            
        }
    }

	std::map<int, TTriangleList>::iterator it;

	for (it =  BallInstancingList.BallAndGlowList.begin(); it !=  BallInstancingList.BallAndGlowList.end(); ++it)
	{
		it->second.RefreshBuffer();
	}
	
}


void TGameLevel::UpdateBallList(cardinal dt)
{
    std::list<TBall>::iterator iBall;
    
    bool mustReloadBalls = false;
    
    //If not loaded - force load
    if (BallInstancingList.BallAndGlowList.size() == 0)
    {
        mustReloadBalls = true;
    }
    
    if (BonusFloorTimer == 0.f)
    {
        vec2 ballPos;
        
		iBall = BallList.begin();

        while (iBall != BallList.end())
        {
            ballPos = iBall->GetPos();
        
            if (ballPos.v[1]<0.f)
            {
                iBall = BallList.erase(iBall);
                mustReloadBalls = true;
            }

			if (iBall != BallList.end())
			{
				++iBall;
			}
        }
    
    }
    
    bool blockListChanged = false;
    
    for (iBall = BallList.begin(); iBall != BallList.end(); ++iBall)
    {
        
    
    iBall->Update(dt);
    
    vec2 ballPos = iBall->GetPos();

    
    if (ballPos.v[0] > 480.f-15.f)
    {
        iBall->ReflectToLeft();
    }
	
	if (ballPos.v[0] < 15.f)
	{
        iBall->ReflectToRight();
    }
	
	if (ballPos.v[1] > 320.0f-16.f)
	{
        iBall->ReflectToDown();
    }
    
    if (BonusFloorTimer > 0.f)
    {
        if (ballPos.v[1] < 13.0f)
        {
            iBall->ReflectToUp();
        }
    }
	
	iBall->TryReflectOnReflector(ReflectorPos);
    
	vec2 ipos = GetBlock(ballPos);
	if (!(ipos == iBall->BallInBlock))
	{
		iBall->PrevBallInBlock = iBall->BallInBlock;
		iBall->BallInBlock = ipos;
		
		int i = static_cast<int>(iBall->BallInBlock.v[0]);
		int j = static_cast<int>(iBall->BallInBlock.v[1]);
		
		int iprev = static_cast<int>(iBall->PrevBallInBlock.v[0]);
		int jprev = static_cast<int>(iBall->PrevBallInBlock.v[1]);
		
		
		if (BlockMatrix[i][j].CanReact())
		{
            bool canThrowBonus;
            
            blockListChanged = true;
            
            if (BonusGothroughTimer > 0.f)
            {
                BlockMatrix[i][j].Disappear();
                canThrowBonus = true;
                
            }
            else
            {
                canThrowBonus = (BlockMatrix[i][j].GetLocked() == 0);
                
                BlockMatrix[i][j].Hit();
                
            }
            
			vec2 blockPos = BlockMatrix[i][j].GetPosFrom(i, j);
		
			
			if (canThrowBonus && rand() % 20 == 0)
			{
			     BonusFallingList.push_back(TBonusFalling(blockPos));
            }
            
            
            if (BonusGothroughTimer == 0.f)
            {
                if (j < jprev)
		        {
				    iBall->ReflectToDown();
			    }
			    else
			    if (j > jprev)
			    {
				    iBall->ReflectToUp();
			    }
                else
			    if (i < iprev)
			    {
				    iBall->ReflectToRight();
			    }
                else
			    if (i > iprev)
			    {
				    iBall->ReflectToLeft();
			    }
            }
            
            
		}
		
	}
	
    }
    
    if (blockListChanged)
    {
        ReloadBlockInstansingList();
    }
    
    if (mustReloadBalls)
    {
        ReloadBallInstancingList();
    }
    else
    {
        RefreshBallInstancingList();
    }
}

void TGameLevel::MultiplyBalls(vec2 pos, vec2 velocity)
{
    mat2 r;
    vec2 v;
    
    for (int i = -2; i<=2; i++)
    {
        r = mat2(i*pi/4.f);
        v = r*velocity;
        
        v.v[1] = max(static_cast<float>(fabs(v.v[1])), 0.2f) * sign(v.v[1]); //Prevent velocityY from being ~= 0
        
        BallList.push_back(TBall(pos, v, BallColor));
    }
    
    ReloadBallInstancingList();
}


void TGameLevel::OnTapDown(vec2 pos)
{
    if (LevelState == CONST_LEVELSTATE_STANDBY)
    {
        if (TapInBackBtnArea(pos))
        {
            SetPause();
            PrevLevelStateIsStandby = true;
        }
        else
        {
        
            LevelState = CONST_LEVELSTATE_PLAYING;
            BallList.begin()->Go();
        }
    }
    else if (LevelState == CONST_LEVELSTATE_PLAYING)
    {
        
        
        if (TapInBackBtnArea(pos))
        {
            SetPause();
        }
        else if (fabs(ReflectorPos.v[0] - pos.v[0])>64.f)
        {
            ReflectorPos.v[0] = pos.v[0];
        }
        
    }
    else if (LevelState == CONST_LEVELSTATE_PAUSE)
    {
        if (pos.v[1] > 128.f)
        {
            ReleasePause();
        }
    }
}

void TGameLevel::OnTapUp(vec2 pos)
{
}

void TGameLevel::OnFling(vec2 slideSpeed)
{
    if (LevelState == CONST_LEVELSTATE_PAUSE)
    {
        OutScaleVelocity = slideSpeed.v[1]/320.f;
    }
}

void TGameLevel::OnScroll(vec2 shift)
{
    const float CONST_SCROLL_SCALE = 1.1f;
    if (LevelState == CONST_LEVELSTATE_PLAYING || LevelState == CONST_LEVELSTATE_STANDBY)
    {
        ReflectorPos.v[0] -= CONST_SCROLL_SCALE*shift.v[0];
    }
    else if (LevelState == CONST_LEVELSTATE_PAUSE)
    {
        
#ifdef TARGET_WIN32
		OutScale += shift.v[1]/320.f;
#else
		OutScale -= shift.v[1]/320.f;
#endif
        TryGoToMenu();
    }
}

void TGameLevel::TryGoToMenu()
{
    if (OutScale < 0.5f)
    {
        OutScale = 0.5f;
        LevelState = CONST_LEVELSTATE_NODRAW;
        App->GoFromGameToMenu();
    }
    if (OutScale > 1.f)
    {
        OutScale = 1.f;
    }
}
