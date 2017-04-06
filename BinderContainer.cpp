#define LOG_TAG "test-android"
//#define LOG_NDEBUG 0
#include <BinderContainer.h>
#include <utils/Log.h>
#include <binder/Parcel.h>
#include <stdint.h>
#include <sys/types.h>
#include <binder/IPCThreadState.h>
#include <binder/PermissionCache.h>

namespace android {

class BpBinderContainer : public BpInterface<IBinderContainer> {
public:
    BpBinderContainer(const sp<IBinder>& impl)
        : BpInterface<IBinderContainer>(impl) {}

        void hello(char*, int) {
            Parcel data;
            data.writeInterfaceToken(IBinderContainer::getInterfaceDescriptor());
            remote()->transact(HELLO, data, NULL);
        }
};

IMPLEMENT_META_INTERFACE(BinderContainer, "android.os.IBinderContainer");

status_t BinderContainer::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code) {
        case HELLO: {
            CHECK_INTERFACE(IBinderContainer, data, reply);
            int len = 32;
            char pChar[len];
            hello(pChar, len);
            reply->writeString16(String16(pChar));
            return OK;
        }
    }
    return BBinder::onTransact(code, data, reply, flags);
};

void BinderContainer::hello(char* pChar, int len) {
    char info[] = "BinderContainer say hello";
    for (int i = 0; i < len; i++) {
    	pChar[i] = info[i];
    }
}

status_t BinderContainer::dump(int fd, const Vector<String16>& /*args*/) {
    int v;
    char vs[128];

    IPCThreadState* self = IPCThreadState::self();
    const int pid = self->getCallingPid();
    const int uid = self->getCallingUid();
    if (!PermissionCache::checkPermission(
                String16("android.permission.DUMP"), pid, uid))
        return PERMISSION_DENIED;

    snprintf(vs, sizeof(vs), "this is a test\n");
    write(fd, vs, strlen(vs));
    fsync(fd);
    return OK;
}

}; // namespace android
