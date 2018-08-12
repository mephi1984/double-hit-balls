#include "Sounds.h"
#include <jni.h>

static JNIEnv* env = NULL;
static jclass jSounds = NULL;

/** BackgroundSound */

void playBackgroundSound() {
    jmethodID method = env->GetStaticMethodID(jSounds, "JniPlayBackgroundSound", "()V");
    env->CallStaticVoidMethod(jSounds, method);
}

void stopBackgroundSound () {
    jmethodID method = env->GetStaticMethodID(jSounds, "JniStopBackgroundSound", "()V");
    env->CallStaticVoidMethod(jSounds, method);
}

/** GameSound - Gunshot */

void playGameSoundGunshot() {
    jmethodID method = env->GetStaticMethodID(jSounds, "JniPlayGunshotSound", "()V");
    env->CallStaticVoidMethod(jSounds, method);
}

void stopGameSoundGunshot() {
    jmethodID method = env->GetStaticMethodID(jSounds, "JniStopGunshotSound", "()V");
    env->CallStaticVoidMethod(jSounds, method);
}

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_sounds_JniSoundCalls_initJniSounds(JNIEnv *pEnv, jobject pThis) {
    env = pEnv;
    jSounds = env->FindClass("fishrungames/doublehitballs/sounds/JniSoundCalls");
}