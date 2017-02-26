//
//  SENamespaceWrapper.hpp
//  salmontemplate
//
//  Created by Роберт Хайреев on 16/01/2017.
//
//

#ifndef SENamespace_h
#define SENamespace_h


#ifdef __cplusplus
extern "C" {
#endif
    //void SE_CreateEngine();
    //void SE_DestroyEngine();
    //TApplication *app = NULL;
    void SE_SetBindDrawableFunc(void(*funcPtr)());
    void SE_AppDeinit();
    void SE_AppUpdate(int dt);
    void SE_AppDraw();
    void SE_AppOnTapDown(int posx, int posy, int touchNumber);
    void SE_AppOnTapUp(int posx, int posy, int touchNumber);
    void SE_AppOnTapUpAfterMove(int posx, int posy, int touchNumber);
    void SE_AppOnScroll(int shiftx, int shifty, int touchNumber);
    void SE_AppOnScale(float scale);
    void SE_SetKeyboardText(const char* newText);
    void SE_OnKeyboardHide();
#ifdef __cplusplus
} //end extern "C"
#endif

#endif /* SENamespace_h */

