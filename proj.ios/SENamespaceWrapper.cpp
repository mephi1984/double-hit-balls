//
//  SENamespaceWrapper.hpp
//  salmontemplate
//
//  Created by Роберт Хайреев on 16/01/2017.
//
//

#include "include/Utils/Utils.h"

//#import "include/Utils/IosApi/ObjC/ViewControllerTemplate.h"
#include "include/Utils/IosApi/IosWrapper.h"
#include "SENamespaceWrapper.h"


namespace SE {
    void SetKeyboardText(const char* newText);
    void OnKeyboardHide();
}

    //TApplication *app = NULL;
void SE_AppDeinit() {
    SE::AppDeinit();
}

void SE_AppUpdate(int dt) {
    SE::AppUpdate(dt);
}

void SE_AppDraw() {
    SE::AppDraw();
}

void SE_AppOnTapDown(int posx, int posy, int touchNumber) {
    SE::AppOnTapDown(posx, posy, touchNumber);
}

void SE_AppOnTapUp(int posx, int posy, int touchNumber) {
    SE::AppOnTapUp(posx, posy, touchNumber);
}

void SE_AppOnTapUpAfterMove(int posx, int posy, int touchNumber) {
    SE::AppOnTapUpAfterMove(posx, posy, touchNumber);
}

void SE_AppOnScroll(int shiftx, int shifty, int touchNumber) {
    SE::AppOnScroll(shiftx, shifty, touchNumber);
}

void SE_AppOnScale(float scale) {
    SE::AppOnScale(scale);
}

void SE_SetKeyboardText(const char* newText) {
    SE::SetKeyboardText(newText);
}

void SE_OnKeyboardHide() {
    SE::OnKeyboardHide();
}
