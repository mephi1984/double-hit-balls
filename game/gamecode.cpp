#include "gamecode.h"
#include "main_code.h"

using namespace SE;
//const std::string fendl = "\x0D\x0A"; //Windows-style, for files

const int CONST_REFLECTOR_WIDTH = 210;
const int CONST_REFLECTOR_HEIGHT = 45;

const int CONST_LEVELSTATE_STANDBY = 0;
const int CONST_LEVELSTATE_PLAYING = 1;
const int CONST_LEVELSTATE_GO_TO_PAUSE = 2;
const int CONST_LEVELSTATE_PAUSE = 3;
const int CONST_LEVELSTATE_LOADING = 4;
const int CONST_LEVELSTATE_NODRAW = 5;
const int CONST_LEVELSTATE_FINISH_FREEZE = 6;
const int CONST_LEVELSTATE_FINISHED = 7;
const int CONST_LEVELSTATE_SNAPSHOTTING = 8;

const float CONST_TIMER_LOADING = 150.f;

const float CONST_PAUSE_APPEAR_TIME = 150.f;

const float CONST_FINISH_FREEZE_TIME = 4000.f; // 1000.f
const float CONST_FINISHING_TIME = 250.f;

const float CONST_BALL_VELOCITY = 200.f;

const Vector2f CONST_SLIDE_UP_POS(240.f, 64.f);

const Vector2f CONST_TAP_TO_CONTINUE_POS(240.f, 200.f);

const float CONST_BACK_BTN_WIDTH = 256.f;
const float CONST_BACK_BTN_HEIGHT = 64.f;

bool operator<(const PairColorTexture& p1, const PairColorTexture& p2)
{
    if (p1.second == p2.second)
    {
		if (p1.first(0) == p2.first(0))
		{
			if (p1.first(1) == p2.first(1))
			{
				if (p1.first(2) == p2.first(2))
				{
					return p1.first(3) < p2.first(3);
				}
				else
				{
					return p1.first(2) < p2.first(2);
				}
			}
			else
			{
				return p1.first(1) < p2.first(1);
			}
		}
		else
		{
			return p1.first(0) < p2.first(0);
		}
    }
    
    return (p1.second < p2.second);
}


TBrick::TBrick()
    : State(CONST_BRICKSTATE_VISIBLE)
    , StateTimer(0.f)
    , Color(Vector4f(0.f, 0.f, 0.f, 1.f))
    , Locked(0)
    , InitialLocked(0)
{
}

void TBrick::SetVisible(Vector4f color, int locked)
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

void TBrick::TryDrawAppear(int ipos, int jpos, int screenWidth)
{

    Vector2f centerPos = GetPosFrom(ipos, jpos, screenWidth);
    float LEVEL_VIEWPORT_WIDTH = Application->GetGameLevelScreenWidth();
	float LEVEL_VIEWPORT_HEIGHT = Application->GetGameLevelScreenHeight();

    Vector2f blockHalfSize = Vector2f(0.5f*CONST_BRICK_WIDTH * (LEVEL_VIEWPORT_WIDTH/480.f), 0.5f*CONST_BRICK_HEIGHT * (LEVEL_VIEWPORT_HEIGHT/320.f));
    
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
        RenderUniform4fv("BrickColor", (Color.data()));
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[texName]);
        Renderer->DrawRect(centerPos - blockHalfSize, centerPos + blockHalfSize);
    }
    else if (State == CONST_BRICKSTATE_APPEAR)
    {
                
        RenderUniform1f("Transparency", 1.f - StateTimer/CONST_BRICK_APPEAR_TIME);
        RenderUniform4fv("BrickColor", Color.data());
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[texName]);
        Renderer->DrawRect(centerPos - blockHalfSize, centerPos + blockHalfSize);
    }
}

void TBrick::Update(size_t dt)
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

Vector4f TBrick::GetColor()
{

    return Color;
}

Vector2f TBrick::GetPosFrom(int ipos, int jpos, int screenWidth)
{
	float LEVEL_VIEWPORT_WIDTH  = Application->GetGameLevelScreenWidth();
	float LEVEL_VIEWPORT_HEIGHT = Application->GetGameLevelScreenHeight();

    const Vector2f BorderShift(CONST_BRICK_SHIFT_X * (LEVEL_VIEWPORT_WIDTH/480.f), CONST_BRICK_SHIFT_Y * (LEVEL_VIEWPORT_HEIGHT/320.f));
    
	Vector2f outlineShift = Vector2f(((screenWidth - LEVEL_VIEWPORT_WIDTH)*0.5f),0.f);

    return outlineShift+BorderShift+Vector2f(CONST_BRICK_WIDTH*(LEVEL_VIEWPORT_WIDTH/480.f)*ipos + 0.5f*CONST_BRICK_WIDTH*(LEVEL_VIEWPORT_WIDTH/480.f), LEVEL_VIEWPORT_HEIGHT - CONST_BRICK_HEIGHT*(LEVEL_VIEWPORT_HEIGHT/320.f)*(jpos)-0.5f*CONST_BRICK_HEIGHT*(LEVEL_VIEWPORT_HEIGHT/320.f));
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

void TBrick::Appear(Vector4f color, int locked)
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

TBonusFalling::TBonusFalling(Vector2f pos)
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

Vector2f TBonusFalling::GetPos()
{

    return Pos;
}

int TBonusFalling::GetType()
{

    return BonusType;
}


void TBonusFalling::Draw()
{

    Vector2f BonusHalfSize = Vector2f(16.f, 16.f);
    
    float transparency = min_t(Lifetime/CONST_BONUS_APPEAR_TIME , 1.f);
    
    RenderUniform4fv("BrickColor", Vector4f(1.0f, 1.0f, 1.0f, 1.0f).data());
    RenderUniform1f("Transparency", transparency);
    
    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[TexName]);
	Renderer->DrawRect(Pos - BonusHalfSize, Pos + BonusHalfSize);

}


void TBonusFalling::Update(size_t dt)
{

    Lifetime += dt;
    Pos(1) -= dt * CONST_BONUS_FALL_SPEED / 1000.f;
}

//===========================================
//===========================================
//===========================================


TBall::TBall(Vector2f pos, Vector2f velocity, Vector4f color)
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

Vector2f TBall::GetPos()
{
    return Pos;
}

Vector2f TBall::GetVelocityVector()
{
    return Velocity;
}

void TBall::Go()
{

//    Velocity = Normalize(Vector2f(2.f, 1.f));
	Velocity = Vector2f(2.f, 1.f).normalized();
}

void TBall::ReflectToLeft()
{

	Application->hitSpark("left",Pos);

    if (Velocity(0) > 0.f)
    {
        Velocity(0) = - Velocity(0);
    }
}

void TBall::ReflectToRight()
{

	Application->hitSpark("right", Pos);

    if (Velocity(0) < 0.f)
    {
        Velocity(0) = - Velocity(0);
    }
}

void TBall::ReflectToUp()
{

	Application->hitSpark("up", Pos);

    if (Velocity(1) < 0.f)
    {
        Velocity(1) = - Velocity(1);
    }
}

void TBall::ReflectToDown()
{

	Application->hitSpark("down", Pos);

    if (Velocity(1) > 0.f)
    {
        Velocity(1) = - Velocity(1);
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
	float LEVEL_VIEWPORT_WIDTH = Application->GetGameLevelScreenWidth();
	float LEVEL_VIEWPORT_HEIGHT = Application->GetGameLevelScreenHeight();
	float hRW = (CONST_REFLECTOR_WIDTH*LEVEL_VIEWPORT_WIDTH / 700.f)*0.5f; // Half Reflector width
	float hRPW = hRW*0.594f; // Half Reflector plane width
	float cRH = CONST_REFLECTOR_HEIGHT*LEVEL_VIEWPORT_HEIGHT / 480.f; // Current Reflector Height

	if (shift >= -(hRW) && shift < -(hRPW)) {
		float t = (shift+ hRW)/cRH;
		return cRH*0.29f + (cRH*0.71f) * t;
	}
	if (shift >= -(hRPW) && shift < hRPW) {
		return cRH;
	}
	if (shift >= hRPW && shift <= hRW) {
		float t = (hRW - shift) / cRH;
		return cRH*0.29f + (cRH*0.71) * t;
	}
    return 0.f;
}

void TBall::TryReflectOnReflector(Vector2f refPos)
{

	float LEVEL_VIEWPORT_HEIGHT = Application->GetGameLevelScreenHeight();
    const float reflectionShiftY = 13.f;
    const float reflectionMaxHeight = CONST_REFLECTOR_HEIGHT*LEVEL_VIEWPORT_HEIGHT/480.f;
    
    
    
    if ((Pos(1) < reflectionMaxHeight + reflectionShiftY) && Pos(1) > 0.0f && Velocity(1) < 0)
	{
        float dy = ReflectorPlaneFunction(Pos(0) - refPos(0));

		if (dy > 0 && (dy + reflectionShiftY > Pos(1)))  
		{
			float shift = (Pos(0) - refPos(0)) / 128.f;
			shift = min_t(shift, 0.5f);
			shift = max_t(shift, -0.5f);
			//Vector2f n = Normalize(Vector2f(shift, 1.0f));
			Vector2f n = Vector2f(shift, 1.0f).normalized();
		    
		    Velocity = Velocity - n * 2.f * (Velocity(0)*n(0) + Velocity(1)*n(1));
            if ((Velocity(1) <= 0) || (fabs(Velocity(0)/Velocity(1)) > 4.f))
	        {
                Velocity(0) = 4.f*sign(Velocity(0));
                Velocity(1) = 1.f;
                //Velocity = Normalize(Velocity);
				Velocity = Velocity.normalized();
            }
		}
	}
}


void TBall::Update(size_t dt)
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
    
    BallColor = Vector4f(0.2f, 0.8f, 1.0f, 1.0f);

	BonusFloorPosY = 0.f;
}

TGameLevel::~TGameLevel()
{
}



void TGameLevel::ReloadBlockInstansingList(int screenWidth)
{

	float LEVEL_VIEWPORT_WIDTH = Application->GetGameLevelScreenWidth();
	float LEVEL_VIEWPORT_HEIGHT = Application->GetGameLevelScreenHeight();

    std::map<int, std::string> ConstTextureBlockMap = boost::assign::map_list_of (0,CONST_BLOCK_TEXTURE1) (1,CONST_BLOCK_TEXTURE2) (2,CONST_BLOCK_TEXTURE3);
    
    std::pair<Vector4f, std::string> tempPair;
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
                
                Vector2f posFrom = BlockMatrix[i][j].GetPosFrom(i,j, screenWidth) + Vector2f(-0.5f*CONST_BRICK_WIDTH*(LEVEL_VIEWPORT_WIDTH/480.f), -0.5f*CONST_BRICK_HEIGHT*(LEVEL_VIEWPORT_HEIGHT/320.f));
                Vector2f posTo = BlockMatrix[i][j].GetPosFrom(i,j, screenWidth) + Vector2f(+0.5f*CONST_BRICK_WIDTH*(LEVEL_VIEWPORT_WIDTH/480.f), +0.5f*CONST_BRICK_HEIGHT*(LEVEL_VIEWPORT_HEIGHT/320.f));
                
                itr->second.Data += MakeDataTriangleList(posFrom, posTo);
                
            }
        }
    }

	for (auto it = BlockInstansingList.ColorBlockList.begin(); it != BlockInstansingList.ColorBlockList.end(); ++it)
	{
		it->second.RefreshBuffer();
	}
	
}

Vector2f TGameLevel::GetBlock(const Vector2f& pos)
{    

	float LEVEL_VIEWPORT_WIDTH = Application->GetGameLevelScreenWidth();
	float LEVEL_VIEWPORT_HEIGHT = Application->GetGameLevelScreenHeight();
	int x = static_cast<int>((pos(0) - CONST_BRICK_SHIFT_X*(LEVEL_VIEWPORT_WIDTH /480.f) - ((Renderer->GetScreenWidth() - LEVEL_VIEWPORT_WIDTH)*0.5f)) / (CONST_BRICK_WIDTH*(LEVEL_VIEWPORT_WIDTH /480.f)));
	int y = static_cast<int>((LEVEL_VIEWPORT_HEIGHT + CONST_BRICK_SHIFT_Y*(LEVEL_VIEWPORT_HEIGHT/320.f) - pos(1)) / (CONST_BRICK_HEIGHT*(LEVEL_VIEWPORT_HEIGHT/320.f)));
	
	
	if (x < 0)
		x = 0;
		
	if (x > CONST_BRICKMATRIX_WIDTH-1)
		x = CONST_BRICKMATRIX_WIDTH-1;
		
	if (y < 0)
		y = 0;
		
	if (y > CONST_BRICKMATRIX_HEIGHT-1)
		y = CONST_BRICKMATRIX_HEIGHT-1;
	

	return Vector2f(x, y);
}

bool TGameLevel::TapInBackBtnArea(const Vector2f& pos)
{

	const float yRatio = Application->GetGameLevelScreenHeight() / 320.f;
	const float backBtnWidth = CONST_BACK_BTN_WIDTH*Application->GetGameLevelScreenWidth()/480.f;
	const float backBtnHeight = CONST_BACK_BTN_HEIGHT*yRatio;
    return (pos(1) > Application->GetGameLevelScreenHeight() - 52.f*yRatio - backBtnHeight) && (pos(0)>=Renderer->GetScreenWidth()*0.5f-backBtnWidth*0.5f) && (pos(0)<=Renderer->GetScreenWidth()*0.5f+backBtnWidth*0.5f);
}

void TGameLevel::SetFinishFreeze()
{

    StateTimer = CONST_FINISH_FREEZE_TIME; // Firework timeline timer
    
    LevelState = CONST_LEVELSTATE_FINISH_FREEZE;
    
}

void TGameLevel::SetFinished()
{

    StateTimer = CONST_FINISHING_TIME;
    
    LevelState = CONST_LEVELSTATE_FINISHED;
    
    OutScale = 1.f;
}

Vector4f TGameLevel::ParseColor(const std::string& s)
{

    Vector4f r;
    std::string ss(s);
    
    int i = ss.find(", ");
    int c = toint(ss.substr(0, i));
    ss.erase(0, i+2);
    r(0) = c / 255.f;

    i = ss.find(", ");
    c = toint(ss.substr(0, i));
    ss.erase(0, i+2);
    r(1) = c / 255.f;
    
    i = ss.find(", ");
    c = toint(ss.substr(0, i));
    ss.erase(0, i+2);
    r(2) = c / 255.f;
    
    //c = toint(ss);
    //r(3) = c / 255.f;
	i = ss.find("\r");
	c = toint(ss.substr(0, i));
	ss.erase(0, i + 2);
	r(3) = c / 255.f;
	
    return r;
}

void TGameLevel::ReloadLevel()
{

    size_t byteCount;
    boost::shared_array<char> file = CreateMemFromFile<char>(LevelFileName, byteCount);
    std::string fileString(&file[0]);
    char c;
    //int n = 0;
 
    
    std::vector<std::string> rows;
    int rowLen;
    while (fileString.size() > 0)
    {
		rowLen = fileString.find(SE::fendl);
        rows.push_back(fileString.substr(0, rowLen));
        fileString.erase(0, rowLen+2);
    }
    
    std::vector<std::string>::iterator rowIterator = rows.begin();
    BallColor = ParseColor(*rowIterator);
    ++rowIterator;
    
    std::vector<Vector4f> colors;
    
    Vector4f tc;
    
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

    ReloadBlockInstansingList(Renderer->GetScreenWidth());

}

void TGameLevel::setBackground(const std::string& textureName)
{
	BkgTexture = textureName;
	LevelScreenTexture = textureName;
}

void TGameLevel::FillWithFile(const std::string& filename)
{

	levelName = GetFileNameWithoutExt(filename);

    LevelFileName = filename;
    ReloadLevel();
}

void TGameLevel::SetStandBy()
{

    ReloadLevel();
    InitLevel(Renderer->GetScreenWidth(), Renderer->GetScreenHeight());
    LevelState = CONST_LEVELSTATE_STANDBY;
}

void TGameLevel::SetLoading()    
{

    InitLevel(Renderer->GetScreenWidth(), Renderer->GetScreenHeight());
    StateTimer = CONST_TIMER_LOADING;
    LevelState = CONST_LEVELSTATE_LOADING;
}

void TGameLevel::InitLevel(int screenWidth, int screenHeight)
{

	float LEVEL_VIEWPORT_WIDTH = Application->GetGameLevelScreenWidth();
	float LEVEL_VIEWPORT_HEIGHT = Application->GetGameLevelScreenHeight();
    ReflectorPos = Vector2f(screenWidth*0.5f, 16* LEVEL_VIEWPORT_HEIGHT/320.f + 13 * LEVEL_VIEWPORT_HEIGHT / 320.f);
    Vector2f ballPos = Vector2f(screenWidth*0.5f, 80* LEVEL_VIEWPORT_HEIGHT/320.f);
    
    BallList.clear();
    BallList.push_back(TBall(ballPos, Vector2f(0, 0), BallColor));
    
    BallList.begin()->BallInBlock = GetBlock(ballPos);
	BallList.begin()->PrevBallInBlock = BallList.begin()->BallInBlock;
	
	BonusGothroughTimer = 0.f;
    BonusFloorTimer = 0.f;
    
    BonusFallingList.clear();
    
    RenderBufferReady = false;
    
    ReloadBallInstancingList();

	BonusFloorPosY = 0.f;

	ReloadBlockInstansingList(screenWidth);

}

bool TGameLevel::IsLoaded()
{
    return (LevelState == CONST_LEVELSTATE_STANDBY);
}

void TGameLevel::drawOutline(int screenWidth, int screenHeight) {

	glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList["black_square"]);
	Renderer->DrawRect(
		Vector2f(
			0.0f,
			0.0f
		),
		Vector2f(
			(screenWidth - Application->GetGameLevelScreenWidth())*0.5f,
			screenHeight
		)
	);

	Renderer->DrawRect(
		Vector2f(
			screenWidth - (screenWidth - Application->GetGameLevelScreenWidth())*0.5f,
			0.f
		),
		Vector2f(
			screenWidth,
			screenHeight
		)
	);
}

void TGameLevel::DrawSnapshot(const std::string& assignedSnapshotFrameBuffer)
{
	InitLevel(768, 480);

	int prevState = LevelState;
	LevelState = CONST_LEVELSTATE_SNAPSHOTTING;
	InnerDraw(768, 480, 768, 480);
	LevelState = prevState;

	prerenderedImage =
		ResourceManager->FrameManager.GetFrameTextureCopy(assignedSnapshotFrameBuffer, levelName + "_prerender");
}


void TGameLevel::Draw()
{
	InnerDraw(Renderer->GetScreenWidth(), Renderer->GetScreenHeight(), Renderer->GetMatrixWidth(), Renderer->GetMatrixHeight());
}


void TGameLevel::InnerDraw(int screenWidth, int screenHeight, int matrixWidth, int matrixHeight)
{

	// Scaling math
	float tSW = Application->GetGameLevelScreenWidth(); // Screen Width
	float tSH = Application->GetGameLevelScreenHeight(); // Screen Height
	float xlOffset = (screenWidth - tSW)*0.5f; // Level Screen x-offset

	float ylOffset = 0.f;
	float lrFBO = 4 * tSH / 320.f; // Left/Right Wall Textures offset from bottom
	float uWTW = tSW * (static_cast<float>(ResourceManager->TexList.GetTextureOriginalHeight(CONST_WALL_UP_TEXTURE)) / static_cast<float>(ResourceManager->TexList.GetTextureOriginalWidth(CONST_WALL_UP_TEXTURE))); // up Wall Texture Width
	float lWTW = (static_cast<float>(ResourceManager->TexList.GetTextureOriginalWidth(CONST_WALL_LEFT_TEXTURE)) / static_cast<float>(ResourceManager->TexList.GetTextureOriginalHeight(CONST_WALL_LEFT_TEXTURE))) * (tSH - uWTW - lrFBO); // left Wall Texture Width
	float rWTW = lWTW; // right Wall Texture Width
	float bWTO = (lWTW + rWTW) * 0.5f; // bonus Wall x-Offset
	float bWTW = (tSW - bWTO) * (static_cast<float>(ResourceManager->TexList.GetTextureOriginalHeight(CONST_WALL_BONUS_TEXTURE)) / static_cast<float>(ResourceManager->TexList.GetTextureOriginalWidth(CONST_WALL_BONUS_TEXTURE))); // bonus Wall Texture Width

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

		Renderer->TranslateMatrix(Vector3f(matrixWidth * 0.5f, matrixHeight * 0.5f, 0));
        Renderer->ScaleMatrix(scale);
		Renderer->TranslateMatrix(-Vector3f(matrixWidth * 0.5f, matrixHeight * 0.5f, 0));
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[LevelScreenTexture]);
        Renderer->DrawRect(Vector2f(xlOffset, ylOffset), Vector2f(xlOffset+tSW, ylOffset+tSH), Vector2f (0.f,0.f), Vector2f(1.f,1.f)); // Game Screen Scale
        Renderer->PopMatrix();
        CheckGlError();
        return;
    }
    
    
    bool mustShowButtons = ((LevelState == CONST_LEVELSTATE_PAUSE) || (LevelState == CONST_LEVELSTATE_GO_TO_PAUSE));
    
    bool pause = (mustShowButtons || (LevelState == CONST_LEVELSTATE_FINISHED));
    
    
    bool renderBufferReady = RenderBufferReady;
    
	/*
    if (pause && renderBufferReady)
    {
        
		//See also below (same method)
        Renderer->PushMatrix();
		Renderer->TranslateMatrix(Vector3f(matrixWidth * 0.5f, matrixHeight * 0.5f, 0));
        Renderer->ScaleMatrix(OutScale);
		Renderer->TranslateMatrix(-Vector3f(matrixWidth * 0.5f, matrixHeight * 0.5f, 0));
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
        Renderer->SetProjectionMatrix(screenWidth, screenHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        CheckGlError();
    }*/
    
	// Level background
	// :::::::::::
	float bkgTW = (float)ResourceManager->TexList.GetTextureOriginalWidth(BkgTexture);
	float bkgTH = (float)ResourceManager->TexList.GetTextureOriginalHeight(BkgTexture);
	float bkgSW; // Background Secreen Width
	float bkgSH; // Background Secreen Height
	float bkgSWO; // Background Secreen Width Offset
	float bkgSHO; // Background Secreen Height Offset
	
	if ((bkgTW/bkgTH) >= 1.6f/*screen ratio*/) {
		bkgSW = (tSH/bkgTH) * bkgTW;
		bkgSH = tSH;
		bkgSWO = (((float)screenWidth) - bkgSW) * 0.5f;
		bkgSHO = ylOffset;
	} else {
		bkgSH = (tSW / bkgTW) * bkgTH;
		bkgSW = tSW;
		bkgSWO = xlOffset;
		bkgSHO = ylOffset;
	}

	// :::::::::::
    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[BkgTexture]);
	//Renderer->DrawRect(Vector2f(xlOffset, ylOffset), Vector2f(xlOffset+tSW, ylOffset+tSH),Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
	Renderer->DrawRect(Vector2f(bkgSWO, bkgSHO), Vector2f(bkgSWO + bkgSW, bkgSHO + bkgSH), Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
	//Renderer->DrawRect(Vector2f(0,0), Vector2f(100,100), Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));


	
	std::list<TBall>::iterator iBall;
    
	Renderer->PushShader("BrickShader");
	
	
	for (int i=0; i<CONST_BRICKMATRIX_WIDTH; i++)
    {
        for (int j=0; j<CONST_BRICKMATRIX_HEIGHT; j++)
        {
            BlockMatrix[i][j].TryDrawAppear(i,j, screenWidth); //Draws only appearing/disappearing blocks
        }
    }
    
    RenderUniform1f("Transparency", 1.f);

    std::list<std::pair<PairColorTexture, TTriangleList>>::iterator colorBlockIterator;
    for (colorBlockIterator = BlockInstansingList.ColorBlockList.begin(); colorBlockIterator != BlockInstansingList.ColorBlockList.end(); ++colorBlockIterator)
    {
        RenderUniform4fv("BrickColor", colorBlockIterator->first.first.data());
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[colorBlockIterator->first.second]);
        
        Renderer->DrawTriangleList(colorBlockIterator->second);
    }
	
    
    std::list<TBonusFalling>::iterator iBonus;
    
    for (iBonus = BonusFallingList.begin(); iBonus != BonusFallingList.end(); ++iBonus)
    {
        iBonus->Draw();
    }
    
	Application->EffectsDraw();

    DrawBallInstancingList();
    
    Renderer->PopShader();
    
	
    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_REFLECTOR_TEXTURE]);
	float xRW = 210.f * tSW/700.f; // x Reflector Width
	float yRH = 45.f * tSH/480.f; // y Reflector Height
	Renderer->DrawRect(Vector2f(-xRW*0.5f, -yRH*0.5f)+ReflectorPos, Vector2f(xRW*0.5f, yRH*0.5f)+ReflectorPos, Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));

	const Vector2f wallUpPos1(xlOffset, tSH-ylOffset-uWTW);
	const Vector2f wallUpPos2(tSW + xlOffset, tSH-ylOffset);
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_WALL_UP_TEXTURE]);
	Renderer->DrawRect(wallUpPos1, wallUpPos2, Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
	
	const Vector2f wallLeftPos1(xlOffset, lrFBO+ylOffset);
	const Vector2f wallLeftPos2(lWTW + xlOffset, tSH-ylOffset-(uWTW*0.68f));
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_WALL_LEFT_TEXTURE]);
	Renderer->DrawRect(wallLeftPos1, wallLeftPos2, Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
	
	const Vector2f wallRightPos1(tSW+xlOffset-rWTW, lrFBO+ylOffset);
	const Vector2f wallRightPos2(tSW + xlOffset, tSH-ylOffset-(uWTW*0.68f));
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_WALL_RIGHT_TEXTURE]);
	Renderer->DrawRect(wallRightPos1, wallRightPos2, Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
	

	if (BonusFloorTimer>0.f)
	{
        
        const Vector2f wallDownPos(240.f, BonusFloorPosY);
        
	    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_WALL_BONUS_TEXTURE]);
	    
		const Vector2f wallBonusPos1(bWTO + xlOffset, ylOffset + wallDownPos(1) - bWTW*0.5f);
		const Vector2f wallBonusPos2(tSW + xlOffset - bWTO, ylOffset + wallDownPos(1) + bWTW*0.5f);
		Renderer->DrawRect(wallBonusPos1, wallBonusPos2, Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
    }
	
    
	/*/
    if (!pause && LevelState != CONST_LEVELSTATE_SNAPSHOTTING)
    {
        RenderUniform1f("Transparency", 1.f);
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_BACK_BTN_TEXTURE]);
        const Vector2f BackBtnPos(screenWidth*0.5f, Application->GetGameLevelScreenHeight() - 52.f*(Application->GetGameLevelScreenHeight()/320.f));
		const float const_backBtnWidth = CONST_BACK_BTN_WIDTH * Application->GetGameLevelScreenWidth()/480.f;
		const float const_backBtnHeight  = CONST_BACK_BTN_HEIGHT * Application->GetGameLevelScreenHeight()/320.f;
        Renderer->DrawRect(Vector2f(-const_backBtnWidth*0.5f, -const_backBtnHeight*0.5f)+BackBtnPos, Vector2f(const_backBtnWidth*0.5f, const_backBtnHeight*0.5f)+BackBtnPos);
    }
	
	if (pause && !renderBufferReady)
    {
        
         //ololo
		//See also above (same method)
        Renderer->SwitchToScreen();
		Renderer->SetFullScreenViewport();
        
        Renderer->PushMatrix();
		//Renderer->LoadIdentity();
		Renderer->TranslateMatrix(Vector3f(matrixWidth * 0.5f, matrixHeight * 0.5f, 0));
		//Renderer->TranslateMatrix(Vector3f(Application->GetGameLevelScreenWidth(), Application->GetGameLevelScreenHeight(), 0));
        Renderer->ScaleMatrix(OutScale);
		Renderer->TranslateMatrix(-Vector3f(matrixWidth * 0.5f, matrixHeight * 0.5f, 0));
        DrawBuffer();
        if (mustShowButtons)
        {
            DrawPauseButtons();
        }
        Renderer->PopMatrix();
        RenderBufferReady = true;
        CheckGlError();
    }*/



	drawOutline(screenWidth, screenHeight);


    CheckGlError();
}

void TGameLevel::DrawPauseButtons() //continue from
{

        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_SLIDE_UP_BTN_TEXTURE]);
        Renderer->DrawRect(Vector2f(-128.f, -64.f)+CONST_SLIDE_UP_POS, Vector2f(128.f, 64.f)+CONST_SLIDE_UP_POS);
        
        glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_TAP_TO_CONTINUE_BTN_TEXTURE]);
        Renderer->DrawRect(Vector2f(-128.f, -128.f)+CONST_TAP_TO_CONTINUE_POS, Vector2f(128.f, 128.f)+CONST_TAP_TO_CONTINUE_POS);

}

void TGameLevel::DrawBallInstancingList()
{

    RenderUniform1f("Transparency", 1.f);
    RenderUniform4fv("BrickColor", BallColor.data());
    
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
    //Vector2f RectPos = Vector2f(-1, -1);
    //Vector2f RectSize = Vector2f(2, 2);
	float x_levelScreenCenter = Renderer->GetScreenWidth()*0.5f;
	float y_levelScreenCenter = Renderer->GetScreenHeight()*0.5f;
	Vector2f RectPos = Vector2f(x_levelScreenCenter, y_levelScreenCenter);
	Vector2f RectSize = Vector2f(x_levelScreenCenter, y_levelScreenCenter);

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
    Application->MarkSetGameLevelPause();
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
    Application->MarkReleaseGameLevelPause();
}

bool TGameLevel::IsPaused()
{

    return ((LevelState == CONST_LEVELSTATE_PAUSE) || (LevelState == CONST_LEVELSTATE_GO_TO_PAUSE) || (LevelState == CONST_LEVELSTATE_FINISHED));
}

void TGameLevel::Update(size_t dt)
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
        dt = static_cast<size_t>(dt / max_t((CONST_FINISH_FREEZE_TIME-StateTimer)/100.f, 1.f));
        
    }
    
    if (BonusGothroughTimer > 0.f)
    {
        BonusGothroughTimer -= dt;
        BonusGothroughTimer = max_t(BonusGothroughTimer, 0.f);
    }
    
    if (BonusFloorTimer > 0.f)
    {
        BonusFloorTimer -= dt;
        BonusFloorTimer = max_t(BonusFloorTimer, 0.f);
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
        
        if ((fabs(ReflectorPos(0) - iBonus->GetPos()(0))<CONST_BONUS_CATCH_DISTANCE_X) &&
            (fabs(ReflectorPos(1) - iBonus->GetPos()(1))<CONST_BONUS_CATCH_DISTANCE_Y))
        {
            int bonusType = iBonus->GetType();
            iBonus = BonusFallingList.erase(iBonus);

		
            if (bonusType == CONST_BONUS_TYPE_GOTHROUGH)
            {
                BonusGothroughTimer = CONST_BONUS_GOTHROUGH_TIME;
            }
            else if (bonusType == CONST_BONUS_TYPE_MULTIPLIER)
            {
                Vector2f pos = BallList.begin()->GetPos();
                Vector2f velocity = BallList.begin()->GetVelocityVector();
                MultiplyBalls(pos, velocity);
            }
            else if (bonusType == CONST_BONUS_TYPE_FLOOR)
            {
                BonusFloorTimer = CONST_BONUS_FLOOR_TIME;
            }
            
        }
        else if (iBonus->GetPos()(1) < -15.f)
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
            BonusFloorPosY = max_t(BonusFloorPosY, 0.f);
        }
        
    }
    else
    {
       //Bonus must go up until 16.f
        if (BonusFloorPosY < 16.f)
        {
            BonusFloorPosY += CONST_BONUS_FLOOR_APPEAR_SPEED * dt / 1000.f;
            BonusFloorPosY = min_t(BonusFloorPosY, 16.f);
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
		Application->fireworkEffect(); // Firework start

        Application->OpenNextLevel();
        Application->MarkSetGameLevelPause();
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
    Vector3f p1;
    Vector3f p2;
    Vector3f p3;
    Vector3f p4;
    
    Vector2f t1 = Vector2f(0.0f, 0.0f);
    Vector2f t2 = Vector2f(0.0f, 1.0f);
    Vector2f t3 = Vector2f(1.0f, 1.0f);
    Vector2f t4 = Vector2f(1.0f, 0.0f);
    */
    
    
    for (i = BallList.begin(); i != BallList.end(); ++i)
    {
        /*
        p1 = Vector3f(i->Pos, 0.f) + Vector3f(-8.f, -8.f, 0.f);
        p2 = Vector3f(i->Pos, 0.f) + Vector3f(-8.f, +8.f, 0.f);
        p3 = Vector3f(i->Pos, 0.f) + Vector3f(+8.f, +8.f, 0.f);
        p4 = Vector3f(i->Pos, 0.f) + Vector3f(+8.f, -8.f, 0.f);
        
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
        
        BallInstancingList.BallAndGlowList[0].Data += MakeDataTriangleList(i->Pos + Vector2f(-8.f, -8.f), i->Pos + Vector2f(8.f, 8.f));
        
        //Replace6PointsInTriangleList(BallInstancingList.BallAndGlowList[0].Data, n, i->Pos + Vector2f(-8.f, -8.f), i->Pos + Vector2f(8.f, 8.f));
        
        //n += 6;
    }
    
    std::list<Vector2f>::iterator j;
    
    for (i = BallList.begin(); i != BallList.end(); ++i)
    {
        for (j = i->TalePos.begin(); j != i->TalePos.end(); ++j)
        {
            /*
            p1 = Vector3f(*j, 0.f) + Vector3f(-16.f, -16.f, 0.f);
            p2 = Vector3f(*j, 0.f) + Vector3f(-16.f, +16.f, 0.f);
            p3 = Vector3f(*j, 0.f) + Vector3f(+16.f, +16.f, 0.f);
            p4 = Vector3f(*j, 0.f) + Vector3f(+16.f, -16.f, 0.f);
        
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
            BallInstancingList.BallAndGlowList[1].Data += MakeDataTriangleList(*j + Vector2f(-16.f, -16.f), *j + Vector2f(16.f, 16.f));
            //Replace6PointsInTriangleList(BallInstancingList.BallAndGlowList[1].Data, n, *j + Vector2f(-16.f, -16.f), *j + Vector2f(16.f, 16.f));
            
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
    Vector3f p1;
    Vector3f p2;
    Vector3f p3;
    Vector3f p4;
    
    Vector2f t1 = Vector2f(0.0f, 0.0f);
    Vector2f t2 = Vector2f(0.0f, 1.0f);
    Vector2f t3 = Vector2f(1.0f, 1.0f);
    Vector2f t4 = Vector2f(1.0f, 0.0f);
    */
    int n = 0;
    int m = 0;
    
    std::list<TBall>::iterator i;
    
    for (i = BallList.begin(); i != BallList.end(); ++i)
    {
        /*
        p1 = Vector3f(i->Pos, 0.f) + Vector3f(-8.f, -8.f, 0.f);
        p2 = Vector3f(i->Pos, 0.f) + Vector3f(-8.f, +8.f, 0.f);
        p3 = Vector3f(i->Pos, 0.f) + Vector3f(+8.f, +8.f, 0.f);
        p4 = Vector3f(i->Pos, 0.f) + Vector3f(+8.f, -8.f, 0.f);
        
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
        
        Replace6PointsInTriangleList(BallInstancingList.BallAndGlowList[0].Data, n, i->Pos + Vector2f(-8.f, -8.f), i->Pos + Vector2f(8.f, 8.f));
        
        n += 6;
    }
    
    std::list<Vector2f>::iterator j;
    
    n = 0;
    m = 0;
    
    for (i = BallList.begin(); i != BallList.end(); ++i)
    {
        for (j = i->TalePos.begin(); j != i->TalePos.end(); ++j)
        {
            /*
            p1 = Vector3f(*j, 0.f) + Vector3f(-16.f, -16.f, 0.f);
            p2 = Vector3f(*j, 0.f) + Vector3f(-16.f, +16.f, 0.f);
            p3 = Vector3f(*j, 0.f) + Vector3f(+16.f, +16.f, 0.f);
            p4 = Vector3f(*j, 0.f) + Vector3f(+16.f, -16.f, 0.f);
        
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
            
            Replace6PointsInTriangleList(BallInstancingList.BallAndGlowList[1].Data, n, *j + Vector2f(-16.f, -16.f), *j + Vector2f(16.f, 16.f));
            
            n += 6;
            
        }
    }

	std::map<int, TTriangleList>::iterator it;

	for (it =  BallInstancingList.BallAndGlowList.begin(); it !=  BallInstancingList.BallAndGlowList.end(); ++it)
	{
		it->second.RefreshBuffer();
	}
	
}


void TGameLevel::UpdateBallList(size_t dt)
{

    std::list<TBall>::iterator iBall;

	float LEVEL_VIEWOPRT_WIDTH = Application->GetGameLevelScreenWidth();
	float LEVEL_VIEWPORT_HEIGHT = Application->GetGameLevelScreenHeight();
    
    bool mustReloadBalls = false;
    
    //If not loaded - force load
    if (BallInstancingList.BallAndGlowList.size() == 0)
    {
        mustReloadBalls = true;
    }

    if (BonusFloorTimer == 0.f)
    {
        Vector2f ballPos;
        
		iBall = BallList.begin();

        while (iBall != BallList.end())
        {
            ballPos = iBall->GetPos();
        
			

            if (ballPos(1)<0.f)
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
    
    Vector2f ballPos = iBall->GetPos();

    float xWO = (Renderer->GetScreenWidth()-LEVEL_VIEWOPRT_WIDTH)*0.5f; // x Wall Offest
	float yWO = (Renderer->GetScreenHeight()-LEVEL_VIEWPORT_HEIGHT)*0.5f; // y Wall Offset

    if (ballPos(0) > (xWO + LEVEL_VIEWOPRT_WIDTH)-15.f*(LEVEL_VIEWOPRT_WIDTH/480.f))
    {
        iBall->ReflectToLeft();
    }
	
	if (ballPos(0) < xWO + 15.f)
	{
        iBall->ReflectToRight();
    }
	
	if (ballPos(1) > (yWO+LEVEL_VIEWPORT_HEIGHT)-16.f * (LEVEL_VIEWPORT_HEIGHT/320.f))
	{
        iBall->ReflectToDown();
    }
    
    if (BonusFloorTimer > 0.f)
    {
        if (ballPos(1) < yWO+13.0f*(LEVEL_VIEWPORT_HEIGHT/320.f))
        {
            iBall->ReflectToUp();
        }
    }
	
	iBall->TryReflectOnReflector(ReflectorPos);
    
	Vector2f ipos = GetBlock(ballPos);
	if (!(ipos == iBall->BallInBlock))
	{
		iBall->PrevBallInBlock = iBall->BallInBlock;
		iBall->BallInBlock = ipos;
		
		int i = static_cast<int>(iBall->BallInBlock(0));
		int j = static_cast<int>(iBall->BallInBlock(1));
		
		int iprev = static_cast<int>(iBall->PrevBallInBlock(0));
		int jprev = static_cast<int>(iBall->PrevBallInBlock(1));
		
		
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
            
			Vector2f blockPos = BlockMatrix[i][j].GetPosFrom(i, j, Renderer->GetScreenWidth());
		
			
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
        ReloadBlockInstansingList(Renderer->GetScreenWidth());
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

void TGameLevel::MultiplyBalls(Vector2f pos, Vector2f velocity)
{

    //mat2 r;
	Matrix2f r;
    Vector2f v;
    
    for (int i = -2; i<=2; i++)
    {

		float alpha = i*pi / 4.f;
		r(0, 0) = cosf(alpha);
		r(1, 0) = sinf(alpha);
		r(0, 1) = -sinf(alpha);
		r(1, 1) = cosf(alpha);

        v = r*velocity;
        
        v(1) = max_t(static_cast<float>(fabs(v(1))), 0.2f) * sign(v(1)); //Prevent velocityY from being ~= 0
        
        BallList.push_back(TBall(pos, v, BallColor));
    }
    
    ReloadBallInstancingList();
}


void TGameLevel::OnTapDown(Vector2f pos)
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
        else if (fabs(ReflectorPos(0) - pos(0))>64.f)
        {
            ReflectorPos(0) = pos(0);
        }
        
    }
    else if (LevelState == CONST_LEVELSTATE_PAUSE)
    {
        if (pos(1) > 128.f)
        {
            ReleasePause();
        }
    }
}

void TGameLevel::OnTapUp(Vector2f pos)
{

}

void TGameLevel::OnFling(Vector2f slideSpeed)
{

    if (LevelState == CONST_LEVELSTATE_PAUSE)
    {
        OutScaleVelocity = slideSpeed(1)/320.f;
    }
}

void TGameLevel::OnScroll(Vector2f shift)
{

    const float CONST_SCROLL_SCALE = 1.1f;
    if (LevelState == CONST_LEVELSTATE_PLAYING || LevelState == CONST_LEVELSTATE_STANDBY)
    {
        ReflectorPos(0) -= CONST_SCROLL_SCALE*shift(0);
    }
    else if (LevelState == CONST_LEVELSTATE_PAUSE)
    {
        

		OutScale += shift(1)/320.f;

        TryGoToMenu();
    }
}

void TGameLevel::TryGoToMenu()
{

    if (OutScale < 0.5f)
    {
        OutScale = 0.5f;
        LevelState = CONST_LEVELSTATE_NODRAW;
        Application->GoFromGameToMenu();
    }
    if (OutScale > 1.f)
    {
        OutScale = 1.f;
    }
}

bool TBrick::IsAppear() {
	if (State == CONST_BRICKSTATE_DISAPPEAR)
	{
		return true;
	}
	else
	{
		return false;
	}
}