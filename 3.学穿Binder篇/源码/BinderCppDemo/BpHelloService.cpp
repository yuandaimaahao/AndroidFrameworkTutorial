#include "IHelloService.h"

namespace android {

BpHelloService::BpHelloService(const sp<IBinder>& impl):BpInterface<IHelloService>(impl) {

}

void BpHelloService::sayHello() {
    Parcel data, reply;
    data.writeInt32(0);
    data.writeString16(String16("IHelloService"));
    remote()->transact(HELLO_SVR_CMD_SAYHELLO, data, &reply);
}

int BpHelloService::sayHelloTo(const char *name) {
    Parcel data, reply;
    int exception;

    data.writeInt32(0);
    data.writeString16(String16("IHelloService"));
    data.writeString16(String16(name));
    remote()->transact(HELLO_SVR_CMD_SAYHELLO_TO, data, &reply);
    exception = reply.readInt32();
	if (exception)
		return -1;
	else
		return reply.readInt32();
}

IMPLEMENT_META_INTERFACE(HelloService, "android.media.IHelloService");
}