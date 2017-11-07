#include <jni.h>
#include "ShopLayer.h"
extern "C"
{
	JNIEXPORT void JNICALL Java_com_kuxx_jh_JNI_sendMessage(JNIEnv *env, jobject obj,jint msg);
}

JNIEXPORT void JNICALL Java_com_kuxx_jh_JNI_sendMessage(JNIEnv *env, jobject obj,jint msg)
{
	ShopLayer::setMessage((PYARET)msg);
}
