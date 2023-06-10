# Binder Java 层整体框架

## 1. Java 层 Binder 框架的初始化

当通过 Zygote 启动进程时，Zygote 进程会调用 AndroidRuntime::startReg 函数注册一系列的JNI函数，其中就包括我们的 Java 层 Binder相关的 JNI，此时就标志着 Framework 层 Binder 框架的启动。

```c++

static const RegJNIRec gRegJNI[] = {
    //......
    REG_JNI(register_android_os_Binder), //register_android_os_Binder 是一个函数指针
    //......
};

int AndroidRuntime::startReg(JNIEnv* env)
{
    
    //......
    if (register_jni_procs(gRegJNI, NELEM(gRegJNI), env) < 0) {
        env->PopLocalFrame(NULL);
        return -1;
    }
   
    //......

    return 0;
}

```

register_android_os_Binder 是一个函数指针，我们来看一下他的实现：

```c++
int register_android_os_Binder(JNIEnv* env)
{
    //注册Binder类的JNI方法
    if (int_register_android_os_Binder(env) < 0)
        return -1;
    //注册BinderInteral类的JNI方法
    if (int_register_android_os_BinderInternal(env) < 0)
        return -1;
    //注册BinderProxy类的JNI方法
    if (int_register_android_os_BinderProxy(env) < 0)
        return -1;

    jclass clazz = FindClassOrDie(env, "android/util/Log");
    gLogOffsets.mClass = MakeGlobalRefOrDie(env, clazz);
    gLogOffsets.mLogE = GetStaticMethodIDOrDie(env, clazz, "e",
            "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Throwable;)I");

    clazz = FindClassOrDie(env, "android/os/ParcelFileDescriptor");
    gParcelFileDescriptorOffsets.mClass = MakeGlobalRefOrDie(env, clazz);
    gParcelFileDescriptorOffsets.mConstructor = GetMethodIDOrDie(env, clazz, "<init>",
                                                                 "(Ljava/io/FileDescriptor;)V");

    clazz = FindClassOrDie(env, "android/os/StrictMode");
    gStrictModeCallbackOffsets.mClass = MakeGlobalRefOrDie(env, clazz);
    gStrictModeCallbackOffsets.mCallback = GetStaticMethodIDOrDie(env, clazz,
            "onBinderStrictModePolicyChange", "(I)V");

    clazz = FindClassOrDie(env, "java/lang/Thread");
    gThreadDispatchOffsets.mClass = MakeGlobalRefOrDie(env, clazz);
    gThreadDispatchOffsets.mDispatchUncaughtException = GetMethodIDOrDie(env, clazz,
            "dispatchUncaughtException", "(Ljava/lang/Throwable;)V");
    gThreadDispatchOffsets.mCurrentThread = GetStaticMethodIDOrDie(env, clazz, "currentThread",
            "()Ljava/lang/Thread;");

    return 0;
}

```
