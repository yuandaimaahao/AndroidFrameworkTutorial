#include "Hello.h"
#define LOG_TAG "hello_hidl"
#include <log/log.h>

namespace jelly {
namespace hardware {
namespace hello_hidl {
namespace V1_0 {
namespace implementation {

// Methods from ::jelly::hardware::hello_hidl::V1_0::IHello follow.
Return<uint32_t> Hello::addition_hidl(uint32_t a, uint32_t b) {
    ALOGE("hello_hidl service is init success....a :%d,b:%d",a,b);
    return uint32_t {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IHello* HIDL_FETCH_IHello(const char* /* name */) {
     ALOGE("hello_hidl service is init success....");
    return new Hello();
}
//
}  // namespace implementation
}  // namespace V1_0
}  // namespace hello_hidl
}  // namespace hardware
}  // namespace jelly
