#include "android_api.h"

#include "main_code.h"

JavaVM* localJavaVirtualMachine = 0;

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_SetJavaVM(JNIEnv * env, jobject obj)
{
    env->GetJavaVM(&localJavaVirtualMachine);
}

JNIEXPORT void JNICALL Java_fishrungames_doublehitballs_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	JniInitApp<TMyApplication>(width, height, width, height);
}

std::stringstream LoadFileFromAndroid(const std::string& fileName)
{
    *Console << "LoadFileFromAndroid called \n";

    JNIEnv* jEnv;
    localJavaVirtualMachine->GetEnv((void**)&jEnv, JNI_VERSION_1_4);

    jclass jniWrapper = jEnv->FindClass("fishrungames/doublehitballs/JniWrapper");

    jstring jFileName = jEnv->NewStringUTF(fileName.c_str());

    jmethodID jLoadFile = jEnv->GetStaticMethodID(jniWrapper, "LoadFile", "(Ljava/lang/String;)Ljava/lang/String;");

    jstring jFileContent = (jstring)jEnv->CallStaticObjectMethod(jniWrapper, jLoadFile, jFileName);

    jboolean jCopy = false;
    std::stringstream ss;
    ss << jEnv->GetStringUTFChars(jFileContent, &jCopy);

    *Console << ss.str();

    return ss;
}

void SaveFileToAndroid(const std::string& fileName, const std::string& fileContent)
{
	*Console << "SaveFileToAndroid called \n";

	JNIEnv* jEnv;
	localJavaVirtualMachine->GetEnv((void**)&jEnv, JNI_VERSION_1_4);

    jclass jniWrapper = jEnv->FindClass("fishrungames/doublehitballs/JniWrapper");

    jstring jFileName = jEnv->NewStringUTF(fileName.c_str());
    jstring jFileContent = jEnv->NewStringUTF(fileContent.c_str());

    jmethodID jSaveFile = jEnv->GetStaticMethodID(jniWrapper, "SaveFile", "(Ljava/lang/String;Ljava/lang/String;)V");

	jEnv->CallStaticVoidMethod(jniWrapper, jSaveFile, jFileName, jFileContent);
}