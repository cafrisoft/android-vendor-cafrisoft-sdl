LOCAL_PATH := $(call my-dir)

##########################################################################################################
# cafriservice
##########################################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    main.cpp
  
#LOCAL_CFLAGS += -Wno-error=unused-parameter  -Wno-error=sign-compare
#LOCAL_CFLAGS += -DWITHOUT_IFADDRS -Wno-sign-compare

#LOCAL_C_INCLUDES += $(USIMOTA_TOP)/inc
LOCAL_CFLAGS += -Wno-error=date-time
  
LOCAL_SHARED_LIBRARIES :=    \
		libbinder            \
        libutils             \
        libcutils            \
        libui                \
		libgui 

LOCAL_MODULE:= cafriserviceTest
include $(BUILD_EXECUTABLE)
