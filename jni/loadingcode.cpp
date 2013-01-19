#include "loadingcode.h"
#include "include/Engine.h"
#include "main_code.h"

void TGameLoading::Draw()
{
    glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_LOADING_TEXTURE]);
	Renderer->DrawRect(vec2(240.f - 128.f, 10.f), vec2(240.f + 128.f, 10.f + 64.f));
	
	glBindTexture(GL_TEXTURE_2D,ResourceManager->TexList[CONST_LOGO_SMALL_TEXTURE]);
	Renderer->DrawRect(vec2(480.f - 256.f+50.f, 320.f - 64.f), vec2(480.f+50.f, 320.f));
    
    CheckGlError();
}
