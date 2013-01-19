#include "android_api.h"

#include "main_code.h"

boost::shared_ptr<TAndroidApplication> App(new TAndroidApplication);

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	try
	{
		App->OuterInit(width, height, 480.f, 320.f);
	}
	catch (ErrorCommon e)
	{
		throw;
	}
}	

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_StopSounds(JNIEnv * env, jobject obj)
{
    try
	{
		App->OuterDeinit(); //Clean up what is left at previous launch (if applicable)
	}
	catch (ErrorCommon e)
	{
		throw;
	}
}


JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_Update(JNIEnv * env, jobject obj, long dt)
{
	try
	{
    
		if (!App->IsInited())
		{
			return;
		}
		App->OuterDraw();
		App->OuterUpdate(dt);

	}
	catch (ErrorCommon e)
	{
		throw;
	}
}

JNIEXPORT int JNICALL Java_fishrungames_doublehitballs_JniWrapper_IsInited(JNIEnv * env, jobject obj)
{   
    try
	{
		if (App->IsInited())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	
	}
	catch (ErrorCommon e)
	{
		throw;
	}
}

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_Destroy(JNIEnv * env, jobject obj)
{
	try
	{
		*Console<<"Destroy!!!\n";
	}
	catch (ErrorCommon e)
	{
		throw;
	}
}

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_OnTapDown(JNIEnv * env, jobject obj, float x, float y, long time)
{
    try
	{
		if (!App->IsInited())
		{
			return;
		}
		
		x = x * 480.f / Renderer->GetScreenWidth();
		y = y * 320.f / Renderer->GetScreenHeight();
	 
		//*Console<<"Tap down "<<tostr(Renderer->GetScreenHeight())<<" "<<tostr(y)<<endl;
		OnTapDownSignal(vec2(x, y));
		
	}
	catch (ErrorCommon e)
	{
		throw;
	}
	
}


JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_OnTapUp(JNIEnv * env, jobject obj, float x, float y, long time)
{

	try
	{
    
		if (!App->IsInited())
		{
			return;
		}
		
		x = x * 480.f / Renderer->GetScreenWidth();
		y = y * 320.f / Renderer->GetScreenHeight();
		
		OnTapUpSignal(vec2(x, y));
	
	}
	catch (ErrorCommon e)
	{
		throw;
	}
}

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_OnTapMove(JNIEnv * env, jobject obj, float x, float y, long time)
{
}

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_OnFling(JNIEnv * env, jobject obj, jfloat velocityX, jfloat velocityY, long time)
{

	try
	{
    
		if (!App->IsInited())
		{
			return;
		}
		
		velocityX = velocityX * 480.f / Renderer->GetScreenWidth();
		velocityY = velocityY * 320.f / Renderer->GetScreenHeight();
		
		OnFlingSignal(vec2(velocityX, velocityY));
	
	}
	catch (ErrorCommon e)
	{
		throw;
	}
}

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_OnScroll(JNIEnv * env, jobject obj, jfloat distanceX, jfloat distanceY, long time)
{
    try
	{
		if (!App->IsInited())
		{
			return;
		}
		
		distanceX = distanceX * 480.f / Renderer->GetScreenWidth();
		distanceY = distanceY * 320.f / Renderer->GetScreenHeight();
		OnScrollSignal(vec2(distanceX, distanceY));
	
	}
	catch (ErrorCommon e)
	{
		throw;
	}
    
}

