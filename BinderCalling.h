#ifndef ANDROID_IBINDERCALLING_H
#define ANDROID_IBINDERCALLING_H

#include <binder/IBinder.h>
#include <binder/IInterface.h>

namespace android {

class IBinderCalling :  public IInterface {
public:
    DECLARE_META_INTERFACE(BinderCalling);
};

class BinderCalling : public BnInterface<IBinderCalling> {
public:
    virtual status_t onTransact(uint32_t code, const Parcel& data,
                                Parcel* reply, uint32_t flags = 0);
    status_t dump(int fd, const Vector<String16>& args);
};

}; // namespace android
#endif // ANDROID_IBINDERCALLING_H
