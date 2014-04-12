LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_MODULE    := WorkingJni
#LOCAL_SRC_FILES := com_example_jnicallbcak_WorkingJni.c 

#LOCAL_MODULE    := jnicallbcak
LOCAL_MODULE    := jniNet
LOCAL_SRC_FILES := jniNet.cpp MGNet.h MGNet.cpp NodeList.cpp NodeList.h

LOCAL_LDLIBS+= -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
