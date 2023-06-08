#ifndef AIDL_GENERATED_COM_YUANDAIMA_BN_HELLO_H_
#define AIDL_GENERATED_COM_YUANDAIMA_BN_HELLO_H_

#include <binder/IInterface.h>
#include <com/yuandaima/IHello.h>

namespace com {

namespace yuandaima {

class BnHello : public ::android::BnInterface<IHello> {
public:
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnHello

}  // namespace yuandaima

}  // namespace com

#endif  // AIDL_GENERATED_COM_YUANDAIMA_BN_HELLO_H_
