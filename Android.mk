# Copyright 2013 The Android Open Source Project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
        main.cpp \
        BinderCalling.cpp \
        BinderContainer.cpp

LOCAL_MODULE := bindertest
LOCAL_MODULE_TAGS := optional
LOCAL_FORCE_STATIC_EXECUTABLE := true

LOCAL_CFLAGS := -D__STDC_LIMIT_MACROS -Werror

ifeq ($(strip $(BOARD_CHARGER_DISABLE_INIT_BLANK)),true)
LOCAL_CFLAGS += -DCHARGER_DISABLE_INIT_BLANK
endif

ifeq ($(strip $(BOARD_CHARGER_ENABLE_SUSPEND)),true)
LOCAL_CFLAGS += -DCHARGER_ENABLE_SUSPEND
endif

LOCAL_STATIC_LIBRARIES := libbinder libutils libstdc++ libcutils liblog libm libc

include $(BUILD_EXECUTABLE)
