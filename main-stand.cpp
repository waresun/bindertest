#define LOG_TAG "test-android"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/PermissionCache.h>
#include <utils/String16.h>
#include <cutils/klog.h>
#include <BinderCalling.h>
#include <BinderContainer.h>

using namespace android;

static sp<BinderCalling> gBinderCalling;
static sp<BinderContainer> gBinderContainer;

int main(int argc, char **argv) {
    int ch;
    while ((ch = getopt(argc, argv, "cr")) != -1) {
        switch (ch) {
            default:
                KLOG_ERROR(LOG_TAG, "Unrecognized option: %c\n",
                           optopt);
                //exit(1);
        }
    }

    sp<ProcessState> proc(ProcessState::self());
    gBinderContainer = new BinderContainer();
    defaultServiceManager()->addService(String16("bindercontainer"), gBinderContainer);
    gBinderCalling = new BinderCalling();
    defaultServiceManager()->addService(String16("bindercalling"), gBinderCalling);
    ProcessState::self()->startThreadPool();    
    IPCThreadState::self()->joinThreadPool(); 
    KLOG_ERROR("Main loop terminated, exiting\n");
    return 3;
}
