#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <cutils/properties.h>
#include <utils/Log.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <log/log.h>


#include "IHelloService.h"


using namespace android;

int main(int argc, char const *argv[])
{
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();
    sm->addService(String16("hello"), new BnHelloService());

	ProcessState::self()->startThreadPool();
	IPCThreadState::self()->joinThreadPool();
    
    return 0;
}
