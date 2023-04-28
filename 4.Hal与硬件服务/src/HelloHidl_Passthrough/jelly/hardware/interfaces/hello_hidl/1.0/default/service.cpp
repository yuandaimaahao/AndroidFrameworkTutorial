#include <jelly/hardware/hello_hidl/1.0/IHello.h>
#include <hidl/LegacySupport.h>
#include <log/log.h>

using jelly::hardware::hello_hidl::V1_0::IHello;
using android::hardware::defaultPassthroughServiceImplementation;

int main() {
    ALOGD("hello-hidl is starting...");
    return defaultPassthroughServiceImplementation<IHello>(4);
}
