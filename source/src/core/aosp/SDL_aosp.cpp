#define LOG_TAG "SDL_aosp"
#define ATRACE_TAG ATRACE_TAG_GRAPHICS

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include <termios.h>
#include <unistd.h>

//#define LOG_NDEBUG 0
#include <utils/Log.h>

#include <binder/IPCThreadState.h>
#include <utils/Errors.h>
#include <utils/Timers.h>
#include <utils/Trace.h>

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <ui/DisplayInfo.h>
#include <media/openmax/OMX_IVCommon.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/MediaCodec.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaMuxer.h>
#include <media/stagefright/PersistentSurface.h>
#include <media/ICrypto.h>
#include <media/MediaCodecBuffer.h>


//#include <gtest/gtest.h>
//#include <android/hardware/configstore/1.0/ISurfaceFlingerConfigs.h>
#include <binder/ProcessState.h>
//#include <configstore/Utils.h>
//#include <cutils/properties.h>
#include <inttypes.h>
#include <gui/BufferItemConsumer.h>
#include <gui/IDisplayEventConnection.h>
#include <gui/IProducerListener.h>
#include <gui/ISurfaceComposer.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <private/gui/ComposerService.h>
#include <ui/Rect.h>
#include <utils/String8.h>

#include <ion/ion.h>


#include "SDL_aosp.h"
#include "../android/SDL_android.h"
#include "../../video/android/SDL_androidvideo.h"

using namespace android;

void Aosp_Print_DisplayInfo() {

    DisplayInfo display_info;
    sp<IBinder> display(SurfaceComposerClient::getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain));
    SurfaceComposerClient::getDisplayInfo(display, &display_info);

    ALOGI("[CMmpRenderer_AndroidSurfaceEx2::Open] DisplayInfo Resol(%d, %d) DPI(%3.1f %3.1f) density=%3.1f orientation=%d ",
        display_info.w, display_info.h,
        display_info.xdpi, display_info.ydpi,
        display_info.density,
        display_info.orientation
        );
    
}

static sp<Surface> s_Surface;
static sp<SurfaceComposerClient> s_ComposerClient;
static sp<SurfaceControl> s_SurfaceControl;
static DisplayInfo s_display_info;
static sp<ANativeWindow> s_NativeWindow;

static int s_UserWindowWidth;
static int s_UserWindowHeight;

void Aosp_Init() {

    sp<IBinder> display(SurfaceComposerClient::getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain));
    SurfaceComposerClient::getDisplayInfo(display, &s_display_info);


    Android_SetScreenResolution(s_display_info.w, s_display_info.h, s_display_info.w, s_display_info.h, 0, 30);

    s_ComposerClient = new SurfaceComposerClient;
    if (s_ComposerClient->initCheck() != NO_ERROR) {
        //MMPDEBUGMSG(MMPZONE_ERROR, (TEXT("[CMmpRenderer_AndroidSurfaceEx2::Open] FAIL: mComposerClient = new SurfaceComposerClient")));
        //mmpResult = MMP_SUCCESS;
        assert(0);
    }

#if 0
    s_SurfaceControl = s_ComposerClient->createSurface(String8("CMmpRenderer_AndroidSurfaceEx2"), s_display_info.w, s_display_info.h, PIXEL_FORMAT_BGRA_8888, 0);
    if (s_SurfaceControl == NULL) {
        //mmpResult = MMP_FAILURE;
        //MMPDEBUGMSG(MMPZONE_ERROR, (TEXT("[CMmpRenderer_AndroidSurfaceEx2::Open] FAIL: m_ComposerClient->createSurface")));
        assert(0);
    }
    else {
        if (s_SurfaceControl->isValid() != 1) {
            //mmpResult = MMP_FAILURE;
            //MMPDEBUGMSG(MMPZONE_ERROR, (TEXT("[CMmpRenderer_AndroidSurfaceEx2::Open] ERROR : m_SurfaceControl->isValid() ")));
            assert(0);
        }
    }

    SurfaceComposerClient::openGlobalTransaction();
    s_SurfaceControl->setLayer(0x7fffffff);
    //m_SurfaceControl->setLayer(0xffffffff);
    s_SurfaceControl->show();
    SurfaceComposerClient::closeGlobalTransaction();

    s_Surface = s_SurfaceControl->getSurface();
    if (s_Surface == NULL) {
        //mmpResult = MMP_FAILURE;
        //MMPDEBUGMSG(MMPZONE_ERROR, (TEXT("[CMmpRenderer_AndroidSurfaceEx2::Open] FAIL: m_SurfaceControl->getSurface ")));
        assert(0);
    }
    else {
        //sp<ANativeWindow> wnd(s_Surface);
        ///s_NativeWindow = wnd;//new ANativeWindow(m_Surface);

    }
#endif

}

void Android_Aosp_SetOrientation(int w, int h, int resizable, const char* hint) {

    s_UserWindowWidth = w;
    s_UserWindowHeight = h;
}

void* Android_Aosp_GetNativeWindow(void) {
    
    void* native_hdl = NULL;
    sp<SurfaceControl> surfaceCtrl = s_ComposerClient->createSurface(String8("MySurface"), s_UserWindowWidth, s_UserWindowHeight, PIXEL_FORMAT_BGRA_8888, 0);
    if (surfaceCtrl == NULL) {
        assert(0);
    }
    else {
        if (surfaceCtrl->isValid() != 1) {
            assert(0);
        }
    }

    //SurfaceComposerClient::openGlobalTransaction();
    //surfaceCtrl->setLayer(0x7fffffff);
    //surfaceCtrl->show();
    //SurfaceComposerClient::closeGlobalTransaction();

    sp<Surface> surface = surfaceCtrl->getSurface();
    if (surface == NULL) {
        assert(0);
    }
    else {
        sp<ANativeWindow> anw(surface);
        s_NativeWindow = anw;
        native_hdl = (void*)s_NativeWindow.get();
    }

    return native_hdl;
}


void Aosp_TestEx1() {

    const int bufferCount = 3;
    const int pixel_format = HAL_PIXEL_FORMAT_RGBA_8888;
    unsigned int usage = 0;
    int err;

    CAFRI_LOGD("s_display_info.w=%d, s_display_info.h=%d \n", s_display_info.w, s_display_info.h);

    void* native_hdl = NULL;
    sp<SurfaceControl> surfaceCtrl = s_ComposerClient->createSurface(String8("MySurface"), s_display_info.w, s_display_info.h, PIXEL_FORMAT_BGRA_8888, 0);
    ALOG_ASSERT(surfaceCtrl);
    ALOG_ASSERT(surfaceCtrl->isValid());

    sp<Surface> surface = surfaceCtrl->getSurface();
    ALOG_ASSERT(surface);

    sp<ANativeWindow> anw(surface);
    ALOG_ASSERT(native_window_api_connect(anw.get(), NATIVE_WINDOW_API_CPU) == NO_ERROR);
    ALOG_ASSERT(native_window_set_buffer_count(anw.get(), bufferCount) == NO_ERROR);
    ALOG_ASSERT(native_window_set_scaling_mode(anw.get(), NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW) == NO_ERROR);
    ALOG_ASSERT(native_window_set_buffers_format(anw.get(), pixel_format /*HAL_PIXEL_FORMAT_YV12*/) == NO_ERROR);
    ALOG_ASSERT(native_window_set_buffers_geometry(anw.get(), s_display_info.w, s_display_info.h, pixel_format) == NO_ERROR);
    ALOG_ASSERT(native_window_set_usage(anw.get(), usage | GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_EXTERNAL_DISP) == NO_ERROR);

    int loop = 0;
    //for (int i = 0; i < bufferCount; i++) {
    while (true) {

        ANativeWindowBuffer* buf = NULL;

        err = native_window_dequeue_buffer_and_wait(anw.get(), &buf);
        ALOG_ASSERT(err == NO_ERROR);

        printf("Dequeue loop=%d err=%d \n", loop, err);
        sleep(1);

        if ((buf != NULL) && (err == 0)) {

            int bufSize = s_display_info.w * s_display_info.h *4;
            int ion_fd = buf->handle->data[0];
            void* virAddr = (void*)mmap(NULL, bufSize, (PROT_READ | PROT_WRITE), MAP_SHARED, ion_fd, 0);
            memset(virAddr, 0x00, bufSize);
            munmap(virAddr, bufSize);

            err = anw->queueBuffer(anw.get(), buf, -1);
            ALOG_ASSERT(err == NO_ERROR);
            printf("EnQueeu loop=%d err=%d  ion_fd=%d virAddr=0x%08LLX\n", loop, err, ion_fd, (long long)virAddr);
        }
        else {
            ALOG_ASSERT(false, "Buf is NULL");
        }
    

        loop++;
    }

    while (true) {
        sleep(1);
    }
}