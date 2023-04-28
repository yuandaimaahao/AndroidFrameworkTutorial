#include <jelly/hardware/hello_hidl/1.0/IHello.h>
#include <hidl/LegacySupport.h>

#define LOG_TAG "hello_hidl"
#include <log/log.h>

using android::sp;
using jelly::hardware::hello_hidl::V1_0::IHello;
using android::hardware::Return;
int main(){
    android::sp<IHello> hw_device = IHello::getService();
    if (hw_device == nullptr) {
              ALOGD("failed to get hello-hidl");
              return -1;
        }
    ALOGD("success to get hello-hidl....");
    Return<uint32_t> total = hw_device->addition_hidl(3,4);
    return 0;
}
