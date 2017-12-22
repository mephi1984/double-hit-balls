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


extern boost::signals2::signal<void(Vector2f)> OnTapUpSignal;
extern boost::signals2::signal<void(Vector2f)> OnTapUpAfterMoveSignal;

extern boost::signals2::signal<void(Vector2f)> OnTapDownSignal;
extern boost::signals2::signal<void(Vector2f)> OnFlingSignal;
extern boost::signals2::signal<void(Vector2f)> OnScrollSignal;

extern boost::signals2::signal<void()> OnDrawSignal;

class TMyApplication : public TApplication
{
protected:
	ParticleEffect sparkler;

public:
	TMyApplication() : TApplication() { }

	virtual void InnerInit();

	virtual void InnerDeinit();

	virtual void InnerDraw();

	virtual void InnerUpdate(size_t dt);

	virtual void InnerOnMouseDown(TMouseState& mouseState);

	virtual void InnerOnMove(Vector2f p, Vector2f shift);

};


extern TMyApplication* Application;


#endif
