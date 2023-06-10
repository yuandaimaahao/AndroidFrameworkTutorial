#include "HelloJNI.h"
#include <stdio.h>
#include <jni.h>

JNIEXPORT jstring JNICALL Java_HelloJNI_sayHello(JNIEnv *env, jobject obj)
{
    return (*env)->NewStringUTF(env,"Hello from JNI !");
}