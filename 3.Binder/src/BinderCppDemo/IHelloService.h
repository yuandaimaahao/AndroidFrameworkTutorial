#ifndef IHELLOSERVICE_H
#define IHELLOSERVICE_H

#include <utils/Errors.h>  // for status_t
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <utils/String8.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

#define HELLO_SVR_CMD_SAYHELLO     1
#define HELLO_SVR_CMD_SAYHELLO_TO  2

namespace android {

class IHelloService: public IInterface {

public:
    DECLARE_META_INTERFACE(HelloService);
	virtual void sayHello() = 0;
	virtual int sayHelloTo(const char *name) = 0;
	
};

//服务端
class BnHelloService: public BnInterface<IHelloService> {

public:
    status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);
    void sayHello();
    int sayHelloTo(const char *name);
};

//客户端
class BpHelloService: public BpInterface<IHelloService> {
public:
    BpHelloService(const sp<IBinder>& impl);
    void sayHello();
    int sayHelloTo(const char *name);
};

}

#endif