#include "loadingcode.h"
#include "include/Engine.h"
#include "main_code.h"

void TGameLoading::Draw()
{
	//*SE::Console << "TGameLoading::Draw";
	
	glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList[CONST_LOADING_BACKGROUND_BLACK]);
	Renderer->DrawRect(Vector2f(0.f, 0.f), Vector2f(Renderer->GetMatrixWidth(), Renderer->GetMatrixHeight()));

    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_LOADING_TEXTURE]);
	Renderer->DrawRect(Vector2f(240.f - 128.f, 10.f), Vector2f(240.f + 128.f, 10.f + 64.f));
	
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_LOGO_SMALL_TEXTURE]);
	Renderer->DrawRect(Vector2f(480.f - 256.f+50.f, 320.f - 64.f), Vector2f(480.f+50.f, 320.f));
    
    CheckGlError();

}
