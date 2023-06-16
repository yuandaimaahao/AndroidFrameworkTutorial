#include <jni.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//1 实现 java 层本地方法
JNIEXPORT void JNICALL
c_init1(JNIEnv *env, jobject thiz) {
     printf("c_init1\n");
}

JNIEXPORT void JNICALL
c_init2(JNIEnv *env, jobject thiz, jint age) {
    printf("c_init2\n");
}

JNIEXPORT jboolean JNICALL
c_init3(JNIEnv *env, jobject thiz, jstring name) {
    printf("c_init3\n");
}
 
JNIEXPORT void JNICALL
c_update(JNIEnv *env, jobject thiz) {
    printf("c_update\n");
}

#ifdef __cplusplus
}
#endif


// typedef struct {
// 	//Java层native方法名称
//    const char* name;
// 	//方法签名
//    const char* signature;
// 	//native层方法指针
//    void*       fnPtr;
// } JNINativeMethod;

//2 构建 JNINativeMethod 数组
//中间的方法签名看上去有点怪异，后面我们来讲它的命名规则
static JNINativeMethod methods[] = {
        {"init", "()V", (void *)c_init1},
        {"init", "(I)V", (void *)c_init2},
        {"init", "(Ljava/lang/String;)Z", (void *)c_init3},
        {"update", "()V", (void *)c_update},
};

/**
 * 3 完成动态注册的入口函数
 * 其内容基本固定
 */ 
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;
 
    // 获取JNI env变量
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        // 失败返回-1
        return result;
    }
 
    // 获取native方法所在类
    const char* className = "com/example/ndk/NativeTest";
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        return result;
    }
 
    // 动态注册native方法
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return result;
    }
 
    // 返回成功
    result = JNI_VERSION_1_6;
    return result;
}
