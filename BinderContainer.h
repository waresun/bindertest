#ifndef ANDROID_IBINDERCONTAINER_H
#define ANDROID_IBINDERCONTAINER_H

#include <binder/IBinder.h>
#include <binder/IInterface.h>

namespace android {
enum {
    HELLO = IBinder::FIRST_CALL_TRANSACTION + 1,
};

class IBinderContainer :  public IInterface {
public:
    DECLARE_META_INTERFACE(BinderContainer);

    virtual void hello(char*, int) = 0;
};

class BinderContainer : public BnInterface<IBinderContainer> {
public:
    virtual status_t onTransact(uint32_t code, const Parcel& data,
                                Parcel* reply, uint32_t flags = 0);
    void hello(char*, int);
    status_t dump(int fd, const Vector<String16>& args);
};

}; // namespace android
#endif // ANDROID_IBINDERCONTAINER_H
