#include <hidl/HidlTransportSupport.h>
#include <utils/Looper.h>
#include <utils/StrongPointer.h>
#include <log/log.h>
#include "Hello.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using jelly::hardware::hello_hidl::V1_0::IHello;
using jelly::hardware::hello_hidl::V1_0::implementation::Hello;

int main() {
    ALOGD("hello-hidl is starting...");

    configureRpcThreadpool(4, true /* callerWillJoin */);

    android::sp<IHello> service = new Hello();
    android::status_t ret = service->registerAsService();

    if (ret != android::NO_ERROR) {
    }

    joinRpcThreadpool();

    return 0;
    //Passthrough模式
    //return defaultPassthroughServiceImplementation<IHello>(4);
}
