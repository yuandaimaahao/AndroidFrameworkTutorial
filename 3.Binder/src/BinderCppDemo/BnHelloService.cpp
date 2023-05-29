#define LOG_TAG "HelloService"
#include <log/log.h>
#include "IHelloService.h"


namespace android {


status_t BnHelloService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {

    switch(code) {
        case HELLO_SVR_CMD_SAYHELLO:  {
            sayHello();
            reply->writeInt32(0);
            return NO_ERROR;
        } break;
           
        case HELLO_SVR_CMD_SAYHELLO_TO: {
            int32_t policy =  data.readInt32();
			String16 name16_tmp = data.readString16(); 
			
			String16 name16 = data.readString16();
			String8 name8(name16);

			int cnt = sayHelloTo(name8.string());

			reply->writeInt32(0); 
			reply->writeInt32(cnt);

            return NO_ERROR;
        } break;
            
        default:
            return BBinder::onTransact(code, data, reply, flags);
        
    }
}

void BnHelloService::sayHello() {
    static int count = 0;
    ALOGI("say hello :%d\n ", ++count);
}

int BnHelloService::sayHelloTo(const char *name) {
    static int cnt = 0;
	ALOGI("say hello to %s : %d\n", name, ++cnt);
	return cnt;
}

}