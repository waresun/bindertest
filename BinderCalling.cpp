#define LOG_TAG "test-android"
//#define LOG_NDEBUG 0
#include <BinderCalling.h>
#include <utils/String16.h>
#include <utils/Log.h>
#include <binder/Parcel.h>
#include <stdint.h>
#include <sys/types.h>
#include <binder/IPCThreadState.h>
#include <binder/PermissionCache.h>
#include <binder/IServiceManager.h>
#include <BinderContainer.h>

namespace android {
//int HELLO = IBinder::FIRST_CALL_TRANSACTION + 1;
class BpBinderCalling : public BpInterface<IBinderCalling> {
public:
    BpBinderCalling(const sp<IBinder>& impl)
        : BpInterface<IBinderCalling>(impl) {}
};

IMPLEMENT_META_INTERFACE(BinderCalling, "android.os.IBinderCalling");

String16 callService(char*, const int);
String16 callService2(char*, const int);

status_t BinderCalling::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    return BBinder::onTransact(code, data, reply, flags);
};

status_t BinderCalling::dump(int fd, const Vector<String16>& /*args*/) {
    int v;
    char vs[128];
    int bytecount = 32;
    char service_name[bytecount] = "bindercontainer";

    IPCThreadState* self = IPCThreadState::self();
    const int pid = self->getCallingPid();
    const int uid = self->getCallingUid();
    if (!PermissionCache::checkPermission(
                String16("android.permission.DUMP"), pid, uid))
        return PERMISSION_DENIED;

    String16 val = callService2(service_name, bytecount);
    snprintf(vs, sizeof(vs), "calling2 result: %s\n", String8(val).string());
    write(fd, vs, strlen(vs));
    fsync(fd);
    return OK;
}
/*调用方法1： 裸奔*/
String16 callService(char* pchar, const int bytecount) {
	int temp = bytecount;
    Parcel data, reply;
	String16 mService = String16(pchar);
	String16 result;
	sp<IServiceManager> sm = defaultServiceManager();
	if (sm != NULL) {
		sp<IBinder> service = sm->checkService(mService);
		if (service != NULL) {
			data.writeInterfaceToken(String16("android.os.IBinderContainer"));
			int32_t err = service->transact(HELLO, data, &reply);
			if (err == 0) {
				result = reply.readString16();
				ALOGE("transact %s OK", pchar);
			} else {
				ALOGE("transact %s error", pchar);
			}
		} else {
			ALOGE("check service %s error", pchar);
		}
	} else {
		ALOGE("Unable to get default service manager");
	}
	return result;
}
/*调用方法2： 用代理类BpBinderContainer*/
String16 callService2(char* pchar, const int bytecount) {
	int temp = bytecount;
    Parcel data, reply;
	String16 mService = String16(pchar);
	String16 result;
	sp<IServiceManager> sm = defaultServiceManager();
	if (sm != NULL) {
		sp<IBinderContainer> service = interface_cast<IBinderContainer>(sm->getService(mService));
		if (service != NULL) {
			service->hello(pchar, bytecount);
			result = String16(pchar);
		} else {
			ALOGE("check service %s error", pchar);
		}
	} else {
		ALOGE("Unable to get default service manager");
	}
	return result;
}
}; // namespace android
