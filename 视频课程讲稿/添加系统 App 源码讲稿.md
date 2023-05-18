# 添加系统 App 源码讲稿

## 1. Android Studio 版本选择与下载

## 2. 如何新建一个系统 App 项目

* 使用 Android Studio 新建一个空项目 FirstSystemApp，包名设置为 `com.yuandaima.firstsystemapp`，语言选择 Java。

* 在 `jelly/rice14` 目录下创建如下的目录和文件：

    ![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230227131236.png)

* 将 as 项目中的 res 文件下的资源文件拷贝到 `Jelly/Rice14/FirstSystemApp/res` 中
* 把 as 项目中的 MainActivity.java 拷贝到 `Jelly/Rice14/FirstSystemApp/src/com/yuandaima/firstsystemapp` 中。
* 拷贝 AndroidManifest.xml 文件
* 编写已添加的 Android.bp 文件：

```json
android_app {
    name: "FirstSystemApp",

    srcs: ["src/**/*.java"],

    resource_dirs: ["res"],

    manifest: "AndroidManifest.xml",

    platform_apis: true,
    
    sdk_version: "",

    certificate: "platform",

    product_specific: true,

    //依赖
    static_libs: ["androidx.appcompat_appcompat",
                 "com.google.android.material_material",
                 "androidx-constraintlayout_constraintlayout"],

}
```

至此我们的系统 App 就创建好了。



接着在我们的 Product 中添加这个App，修改 `device/Jelly/Rice14/Rice14.mk`：

```Makefile
# 添加以下内容
PRODUCT_PACKAGES += FirstSystemApp
```

接着编译系统，启动虚拟机，打开 app：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
emulator 
```

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230227135955.png)


## 3. 系统 App 添加库依赖

* AOSP 已有的库
* 自己添加的库
  * 打包好的软件包
  * 源码形式

### 3.1 AOSP 已有的库

 * prebuilts/tools/common/m2/
 * prebuilts/sdk/current/androidx/
  
### 3.2 自己添加库

打包好的软件包

假设我们的 FirstSystemApp 需要引入 lottie 这个动画库。

首先我们[这里](https://repo1.maven.org/maven2/com/airbnb/android/lottie/5.2.0/)下载好 lottie 库的 aar 打包文件。

在 `device/Jelly/Rice14` 目录下创建如下的目录结构：

```bash
liblottie/
├── Android.bp
└── lottie-5.2.0.aar
```

其中 Android.bp 的内容如下：

```bash
android_library_import {
    name: "lib-lottie",
    aars: ["lottie-5.2.0.aar"],
    sdk_version: "current",
}
```

然后我们修改 FirstSystemApp 中的 Android.bp 引入这个库：

```json

    static_libs: ["androidx.appcompat_appcompat",
                 "com.google.android.material_material",
                 "androidx-constraintlayout_constraintlayout",
                 "FirstSystemAndroidLibrary",
                  "lib-lottie"],
```

这样就可以在 App 中使用 lottie 库了


源码形式

在 `device/Jelly/Rice14` 目录下创建如下的文件和文件夹

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230228155617.png)

其中 `MyCustomView.java` 是一个用于演示的没有具体功能的自定义 View：

```java
package com.yuandaima.firstsystemandroidlibrary;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.Nullable;

public class MyCustomView extends View {
    public MyCustomView(Context context) {
        super(context);
    }

    public MyCustomView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public MyCustomView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public MyCustomView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }
}
```

`AndroidManifest.xml` 的内容如下：

```xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
 package="com.yuandaima.firstsystemandroidlibrary">

</manifest>
```

`Android.bp` 的内容如下：

```json
android_library  {
    name: "FirstSystemAndroidLibrary",

    srcs: ["src/**/*.java"],

    resource_dirs: ["res"],

    manifest: "AndroidManifest.xml",

    sdk_version: "current",

    product_specific: true,
    
    //依赖
    static_libs: ["androidx.appcompat_appcompat",],

    java_version: "1.7",

    installable: true,

}
```

接着修改我们的 FirstSystemApp 项目

`Android.bp` 添加依赖如下：

```json
android_library  {

    //......
    
    //依赖
    static_libs: ["androidx.appcompat_appcompat",
                 "com.google.android.material_material",
                 "androidx-constraintlayout_constraintlayout",
                 "FirstSystemAndroidLibrary"],

}
```

修改一下 `MainActivity`，在 App 里使用我们的自定义 View：

```java
package com.yuandaima.firstsystemapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.yuandaima.firstsystemandroidlibrary.MyCustomView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        MyCustomView myView = new MyCustomView(this);
    }
}
```

接着编译系统，启动虚拟机，打开 app：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
emulator 
```

这样我们的库就算引入完毕了。


## 4. JNI 项目

Android 10 下，Android.bp(soong) 方式对 JNI 的支持有点问题，所以我们只有用 Android.mk 来演示了。Android 13 下 Android.bp (soong) 是完美支持 JNI 的。

在 `device/Jelly/Rice14` 目录下添加如下的文件与文件夹：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230306191505.png)

jni/Android.mk 内容如下：

```Makefile
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

# This is the target being built.
LOCAL_MODULE:= myjnilib


# All of the source files that we will compile.
LOCAL_SRC_FILES:= \
    native.cpp

# All of the shared libraries we link against.
LOCAL_LDLIBS := -llog

# No static libraries.
LOCAL_STATIC_LIBRARIES :=

LOCAL_CFLAGS := -Wall -Werror

LOCAL_NDK_STL_VARIANT := none

LOCAL_SDK_VERSION := current

LOCAL_PRODUCT_MODULE := true

include $(BUILD_SHARED_LIBRARY)

```

jni/native.cpp 的内容如下：

```c++
/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "simplejni native.cpp"
#include <android/log.h>

#include <stdio.h>

#include "jni.h"

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static jint
add(JNIEnv* /*env*/, jobject /*thiz*/, jint a, jint b) {
int result = a + b;
    ALOGI("%d + %d = %d", a, b, result);
    return result;
}

static const char *classPathName = "com/example/android/simplejni/Native";

static JNINativeMethod methods[] = {
  {"add", "(II)I", (void*)add },
};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        ALOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }

  return JNI_TRUE;
}


// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */
 
typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;
    
    ALOGI("JNI_OnLoad");

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;

    if (registerNatives(env) != JNI_TRUE) {
        ALOGE("ERROR: registerNatives failed");
        goto bail;
    }
    
    result = JNI_VERSION_1_4;
    
bail:
    return result;
}
```

SimpleJNI.java 的内容如下：

```java

package com.example.android.simplejni;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class SimpleJNI extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView tv = new TextView(this);
        int sum = Native.add(2, 3);
        tv.setText("2 + 3 = " + Integer.toString(sum));
        setContentView(tv);
    }
}

class Native {
    static {
    	// The runtime will add "lib" on the front and ".o" on the end of
    	// the name supplied to loadLibrary.
        System.loadLibrary("simplejni");
    }

    static native int add(int a, int b);
}

```

最外面的 Android.mk 的内容如下：

```Makefile
TOP_LOCAL_PATH:= $(call my-dir)

# Build activity

LOCAL_PATH:= $(TOP_LOCAL_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-subdir-java-files)

LOCAL_PACKAGE_NAME := JNIApp

LOCAL_JNI_SHARED_LIBRARIES := myjnilib

LOCAL_PROGUARD_ENABLED := disabled

LOCAL_SDK_VERSION := current

LOCAL_DEX_PREOPT := false

LOCAL_PRODUCT_MODULE := true

include $(BUILD_PACKAGE)

# ============================================================

# Also build all of the sub-targets under this one: the shared library.
include $(call all-makefiles-under,$(LOCAL_PATH))
```

AndroidManifest.xml 的内容如下：

```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
      package="com.example.android.simplejni">
    <application android:label="Simple JNI">
        <activity android:name="SimpleJNI">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest> 
```

最后在 `device/Jelly/Rice14/Rice14.mk` 中添加：

```Makefile
PRODUCT_PACKAGES += helloworld \
    JNIApp \
```

编译并运行虚拟机就可以看到 JNIApp 了：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230306200043.png)
