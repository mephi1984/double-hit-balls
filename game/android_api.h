#ifndef ANDROID_API_H_INCLUDED
#define ANDROID_API_H_INCLUDED

#include <jni.h>
#include <android/log.h>

#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <string>
#include <sstream>

#include "boost/shared_ptr.hpp"

#include "main_code.h"

#include "include/Engine.h"

using namespace SE;

extern "C"
{
    JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_SetJavaVM(JNIEnv * env, jobject obj);

    JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height);
};

std::stringstream LoadFileFromAndroid(const std::string& fileName);

void SaveFileToAndroid(const std::string& fileName, const std::string& fileContent);

#endif
