#ifndef CREDITSCODE_H_INCLUDED
#define CREDITSCODE_H_INCLUDED

#include "game_area_interface.h"

using namespace SE;


class TGameCredits : public TGameAreaAncestor
{
    static const int CONST_STATE_APPEARING;
    static const int CONST_STATE_APPEARED;
    static const int CONST_STATE_DISAPPEARING;
    static const int CONST_STATE_DISAPPEARED;
    static const float CONST_APPEAR_TIME;


    int State;
    float StateTimer;

public:
    
    TGameCredits();
    
    virtual void Draw();
    virtual void Update(cardinal dt);
    virtual void OnTapDown(vec2 p);
    
    void StartAppear();
    void StartDisappear();
};


#endif
