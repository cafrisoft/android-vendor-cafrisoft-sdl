LOCAL_PATH := $(call my-dir)

###########################
#
# SDL2_ttf shared library
#
###########################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := SDL_ttf.c
LOCAL_C_INCLUDES := $(TOP)/external/freetype/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../source/include
#LOCAL_CFLAGS := -DANDROID_CAFRISOFT_AOSP 
LOCAL_SHARED_LIBRARIES := libft2 libSDL2

LOCAL_MODULE := libSDL2_ttf
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)

