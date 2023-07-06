#define LOG_TAG "aidl_cpp"

#include <stdlib.h>
#include <utils/RefBase.h>
#include <utils/Log.h>
#include <binder/TextOutput.h>
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "com/yuandaima/IHello.h"
#include "com/yuandaima/BnHello.h"

using namespace android;

class IHelloServer : public com::yuandaima::BnHello
{
public:
    binder::Status hello()
    {
        ALOGI("hello");
        return binder::Status();
    }

    binder::Status sum(int32_t v1, int32_t v2, int32_t *_aidl_return) override
    {
        ALOGI("server: sum: %d + %d", v1, v2);
        *_aidl_return = v1 + v2;
        return binder::Status();
    }
};

int main(int argc, char const *argv[])
{
    defaultServiceManager()->addService(String16("IHello"), new IHelloServer());
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
    return 0;
}
