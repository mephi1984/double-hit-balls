#ifndef GAME_AREA_INTERFACE_H_INCLUDED
#define GAME_AREA_INTERFACE_H_INCLUDED

#include "include/Engine.h"

using namespace SE;


class TGameAreaAncestor
{
public:
    virtual void Draw() { }
    virtual void Update(cardinal) { }
    virtual void OnTapUp(vec2) { }
    virtual void OnTapDown(vec2) { }
    virtual void OnFling(vec2) { }
	virtual void OnScroll(vec2) { }
	
	virtual ~TGameAreaAncestor() { } //By default destructor is not virtual
};


#endif
