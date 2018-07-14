#include "creditscode.h"
#include "include/Engine.h"

#include "main_code.h"

const int TGameCredits::CONST_STATE_APPEARING = 0;
const int TGameCredits::CONST_STATE_APPEARED = 1;
const int TGameCredits::CONST_STATE_DISAPPEARING = 2;
const int TGameCredits::CONST_STATE_DISAPPEARED = 3;

const float TGameCredits::CONST_APPEAR_TIME = 150.f;

TGameCredits::TGameCredits()
    : TGameAreaAncestor()
    , StateTimer(0)
    , State(CONST_STATE_DISAPPEARED)
{
}

void TGameCredits::Draw()
{
	//*SE::Console << "TGameCredits::Draw";
    float transparency = StateTimer/CONST_APPEAR_TIME;

    
    RenderUniform1f("Transparency", transparency);
    
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList["main_menu_bkg_left"]);
	Renderer->DrawRect(Vector2f(0, 0.f), Vector2f(Renderer->GetScreenWidth(), Renderer->GetScreenHeight()));
	
	const Vector2f creditsPos(140.f, 130.f);
	
	const Vector2f creditsHalfSize(128.f, 128.f);
	
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_CREDITS_TEXTURE]);
	Renderer->DrawRect(creditsPos-creditsHalfSize, creditsPos+creditsHalfSize);
	
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_LOGO_SMALL_TEXTURE]);
	Renderer->DrawRect(Vector2f(240.f-128.f+15.f, 320.f-64.f), Vector2f(240.f+128.f+15.f, 320.f));
    
    CheckGlError("Draw TGameCredits");
}

void TGameCredits::Update(size_t dt)
{
    if (State == CONST_STATE_APPEARING)
	{
        StateTimer += dt;
        if (StateTimer >= CONST_APPEAR_TIME)
        {
            StateTimer = CONST_APPEAR_TIME;
            State = CONST_STATE_APPEARED;
        }
    }
    else if (State == CONST_STATE_DISAPPEARING)
	{
        StateTimer -= dt;
        if (StateTimer <= 0.f)
        {
            StateTimer = 0.f;
            State = CONST_STATE_DISAPPEARED;
        }
    }
}

void TGameCredits::OnTapDown(Vector2f p)
{
    Application->GoFromCreditsToMenu();
}

void TGameCredits::StartAppear()
{
    State = CONST_STATE_APPEARING;
}

void TGameCredits::StartDisappear()
{
    State = CONST_STATE_DISAPPEARING;
}
