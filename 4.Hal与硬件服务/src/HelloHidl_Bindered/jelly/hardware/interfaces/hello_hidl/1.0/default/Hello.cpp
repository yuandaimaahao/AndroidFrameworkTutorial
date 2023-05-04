// FIXME: your file license if you have one

#include "Hello.h"
#define LOG_TAG "hello_hidl"
#include <log/log.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

namespace jelly
{
    namespace hardware
    {
        namespace hello_hidl
        {
            namespace V1_0
            {
                namespace implementation
                {

                    // Methods from ::jelly::hardware::hello_hidl::V1_0::IHello follow.
                    Return<uint32_t> Hello::addition_hidl(uint32_t a, uint32_t b)
                    {
                        ALOGE("hello_hidl service is init success....a :%d,b:%d", a, b);
                        return uint32_t{};
                        return uint32_t{};
                    }

                    Return<uint32_t> Hello::write(const hidl_string &name)
                    {

                        ALOGD("service is writing \n");
                        int fd = open("/dev/hello", O_RDWR);
                        if (fd == -1)
                        {
                            ALOGD("can not open file /dev/hello\n");
                            return uint32_t{0};
                        }

                        ::write(fd, name.c_str(), 100);

                        close(fd);
                        return uint32_t{1};
                    }

                    // 通过 read_cb 接口，回传数据
                    Return<void> Hello::read(read_cb _hidl_cb)
                    {
                        ALOGD("service is reading \n");
                        char buf[100];
                        int fd = open("/dev/hello", O_RDWR);
                        if (fd == -1)
                        {
                            ALOGD("can not open file /dev/hello\n");
                            return Void();
                        }

                        ::read(fd, buf, 100);

                        hidl_string result(buf);
                        _hidl_cb(result);

                        close(fd);
                        return Void();
                    }

                    // Methods from ::android::hidl::base::V1_0::IBase follow.

                    // IHello* HIDL_FETCH_IHello(const char* /* name */) {
                    // return new Hello();
                    //}
                    //
                } // namespace implementation
            }     // namespace V1_0
        }         // namespace hello_hidl
    }             // namespace hardware
} // namespace jelly
