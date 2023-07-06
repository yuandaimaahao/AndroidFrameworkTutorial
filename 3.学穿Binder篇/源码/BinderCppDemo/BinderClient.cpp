#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <cutils/properties.h>
#include <utils/Log.h>
#include <unistd.h>

#include "IHelloService.h"

using namespace android;

int main(int argc, char const *argv[])
{
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();

    sp<IBinder> binder = sm->getService(String16("hello"));

    sp<IHelloService> service =
		    interface_cast<IHelloService>(binder);

    if (binder == 0)
	{
		ALOGI("can't get hello service\n");
		return -1;
	}

    service->sayHello();
    int cnt = service->sayHelloTo("nihao");
	ALOGI("client call sayhello_to, cnt = %d", cnt);


    return 0;
}
