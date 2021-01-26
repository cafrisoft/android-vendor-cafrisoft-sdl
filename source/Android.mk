LOCAL_PATH := $(call my-dir)

###########################
#
# hidapi library
#
###########################

include $(CLEAR_VARS)

LOCAL_CPPFLAGS += -std=c++11

LOCAL_SRC_FILES := src/hidapi/android/hid.cpp

LOCAL_SHARED_LIBRARIES := liblog

LOCAL_MODULE := libhidapi
#LOCAL_LDLIBS := -llog
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)


###########################
#
# SDL shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/src/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/dummy/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/openslES/*.c) \
	$(LOCAL_PATH)/src/atomic/SDL_atomic.c.arm \
	$(LOCAL_PATH)/src/atomic/SDL_spinlock.c.arm \
	$(wildcard $(LOCAL_PATH)/src/core/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/core/aosp/*.cpp) \
	$(wildcard $(LOCAL_PATH)/src/cpuinfo/*.c) \
	$(wildcard $(LOCAL_PATH)/src/dynapi/*.c) \
	$(wildcard $(LOCAL_PATH)/src/events/*.c) \
	$(wildcard $(LOCAL_PATH)/src/file/*.c) \
	$(wildcard $(LOCAL_PATH)/src/haptic/*.c) \
	$(wildcard $(LOCAL_PATH)/src/haptic/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/hidapi/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/virtual/*.c) \
	$(wildcard $(LOCAL_PATH)/src/loadso/dlopen/*.c) \
	$(wildcard $(LOCAL_PATH)/src/locale/*.c) \
	$(wildcard $(LOCAL_PATH)/src/locale/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/misc/*.c) \
	$(wildcard $(LOCAL_PATH)/src/misc/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/power/*.c) \
	$(wildcard $(LOCAL_PATH)/src/power/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/filesystem/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/sensor/*.c) \
	$(wildcard $(LOCAL_PATH)/src/sensor/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/render/*.c) \
	$(wildcard $(LOCAL_PATH)/src/render/*/*.c) \
	$(wildcard $(LOCAL_PATH)/src/stdlib/*.c) \
	$(wildcard $(LOCAL_PATH)/src/thread/*.c) \
	$(wildcard $(LOCAL_PATH)/src/thread/pthread/*.c) \
	$(wildcard $(LOCAL_PATH)/src/timer/*.c) \
	$(wildcard $(LOCAL_PATH)/src/timer/unix/*.c) \
	$(wildcard $(LOCAL_PATH)/src/video/*.c) \
	$(wildcard $(LOCAL_PATH)/src/video/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/video/yuv2rgb/*.c) \
	$(wildcard $(LOCAL_PATH)/src/test/*.c))

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(TOP)/external/googletest/googletest/include
LOCAL_C_INCLUDES += $(TOP)/system/core/libion/include

LOCAL_CFLAGS := -DANDROID_CAFRISOFT_AOSP 
LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
LOCAL_CFLAGS += -Wno-error=unused-parameter  -Wno-error=sign-compare
LOCAL_CFLAGS += -Wno-unused-parameter -Wno-sign-compare  

LOCAL_SHARED_LIBRARIES := libhidapi \
    libandroid \
	libOpenSLES libEGL libGLESv2 libGLESv1_CM \
	libstagefright libmedia libmedia_omx libutils libbinder libstagefright_foundation \
	libjpeg libui libgui libcutils liblog

LOCAL_STATIC_LIBRARIES := cpufeatures
LOCAL_MODULE := libSDL2
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)


###########################
# test yuv
###########################
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= test/testyuv.c test/testyuv_cvt.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
  
#LOCAL_CFLAGS += -DWITHOUT_IFADDRS -Wno-sign-compare
#LOCAL_CFLAGS += -Wno-error=date-time
LOCAL_CFLAGS := -DANDROID_CAFRISOFT_AOSP
LOCAL_CFLAGS += -Wno-error=unused-parameter  -Wno-error=sign-compare
LOCAL_CFLAGS += -Wno-unused-parameter -Wno-sign-compare
  
LOCAL_SHARED_LIBRARIES :=  libbinder libutils libcutils libui libgui \
        libSDL2

LOCAL_MODULE:= sdltestyuv
include $(BUILD_EXECUTABLE)



