#define LOG_TAG "test-android"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/PermissionCache.h>
#include <utils/String16.h>
#include <cutils/klog.h>
#include <sys/epoll.h>
#include <BinderCalling.h>
#include <BinderContainer.h>

using namespace android;

static int gBinderFd;
static int epollfd;
#define MAX_EPOLL_EVENTS 40
static int eventct;
// -1 for no epoll timeout
static int awake_poll_interval = -1;
static sp<BinderContainer> gBinderContainer;
static sp<BinderCalling> gBinderCalling;


int preparetowait(void) {
    IPCThreadState::self()->flushCommands();
    return -1;
}

static void binder_event(uint32_t /*epevents*/) {
    IPCThreadState::self()->handlePolledCommands();
}

int epoll_register_event(int fd, void (*handler)(uint32_t)) {
    struct epoll_event ev;

    ev.events = EPOLLIN | EPOLLWAKEUP;
    ev.data.ptr = (void *)handler;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        KLOG_ERROR(LOG_TAG,
                   "epoll_ctl failed; errno=%d\n", errno);
        return -1;
    }

    eventct++;
    return 0;
}

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

    epollfd = epoll_create(MAX_EPOLL_EVENTS);
    if (epollfd == -1) {
        KLOG_ERROR(LOG_TAG,
                   "epoll_create failed; errno=%d\n",
                   errno);
        return -1;
    }

    ProcessState::self()->setThreadPoolMaxThreadCount(0);
    IPCThreadState::self()->disableBackgroundScheduling(true);
    IPCThreadState::self()->setupPolling(&gBinderFd);

    if (gBinderFd >= 0) {
        if (epoll_register_event(gBinderFd, binder_event))
            KLOG_ERROR(LOG_TAG,
                       "Register for binder events failed\n");
    }
    gBinderContainer = new BinderContainer();
    defaultServiceManager()->addService(String16("bindercontainer"), gBinderContainer);
    gBinderCalling = new BinderCalling();
    defaultServiceManager()->addService(String16("bindercalling"), gBinderCalling);
    while (1) {
        struct epoll_event events[eventct];
        int nevents;
        int timeout = awake_poll_interval;
        int mode_timeout;

        mode_timeout = preparetowait();
        if (timeout < 0 || (mode_timeout > 0 && mode_timeout < timeout))
            timeout = mode_timeout;
        nevents = epoll_wait(epollfd, events, eventct, timeout);

        if (nevents == -1) {
            if (errno == EINTR)
                continue;
            KLOG_ERROR(LOG_TAG, "healthd_mainloop: epoll_wait failed\n");
            break;
        }

        for (int n = 0; n < nevents; ++n) {
            if (events[n].data.ptr)
                (*(void (*)(int))events[n].data.ptr)(events[n].events);
        }
    }
    KLOG_ERROR("Main loop terminated, exiting\n");
    return 3;
}
