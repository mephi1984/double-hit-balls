#include "android_api.h"

#include "main_code.h"

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	JniInitApp<TMyApplication>(width, height, width, height);
}	


