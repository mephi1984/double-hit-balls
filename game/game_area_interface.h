#ifndef GAME_AREA_INTERFACE_H_INCLUDED
#define GAME_AREA_INTERFACE_H_INCLUDED

#include "include/Engine.h"

using namespace SE;


class TGameAreaAncestor
{
public:
    virtual void Draw() { }
    virtual void Update(size_t) { }
    virtual void OnTapUp(Vector2f) { }
    virtual void OnTapUpAfterMove(Vector2f) { }
    
    virtual void OnTapDown(Vector2f) { }
    virtual void OnFling(Vector2f) { }
	virtual void OnScroll(Vector2f) { }
	
	virtual ~TGameAreaAncestor() { } //By default destructor is not virtual
};


#endif
