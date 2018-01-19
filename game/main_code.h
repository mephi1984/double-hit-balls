#ifndef MAIN_CODE_H_INCLUDED
#define MAIN_CODE_H_INCLUDED

#ifdef TARGET_ANDROID
#include "android_api.h"
#endif

#ifdef TARGET_WIN32
#define NOMINMAX 1
#include <Windows.h>
#undef NOMINMAX
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "boost/shared_ptr.hpp"
#include "boost/thread/thread.hpp"
#include "boost/assign.hpp"
#include "boost/bind.hpp"

#include "include/Engine.h"

#include "boost/signals2.hpp"//Signal must be included after asio

using namespace SE;

class TMyApplication : public TApplication
{
protected:

	bool Inited;

public:
	TRenderPair fabricRender;
	TRenderPair background;

	bool Loaded;

	TMyApplication() : TApplication(), Loaded(false), Inited(false) { }

	virtual void InnerInit();

	virtual void InnerDeinit();

	virtual void InnerDraw();

	virtual void InnerUpdate(size_t dt);

	bool IsLoaded();

	bool IsInited();


	virtual void InnerOnTapDown(Vector2f p);

	virtual void InnerOnTapUp(Vector2f p);

	virtual void InnerOnTapUpAfterMove(Vector2f p);

	virtual void InnerOnMove(Vector2f p, Vector2f shift);

	virtual void OnFling(Vector2f v);

	virtual void OnMouseWheel(short int delta);


	float distance = 2000;

	float alpha = 0;

	float phi = pi / 6;
};


extern TMyApplication* Application;



#endif
