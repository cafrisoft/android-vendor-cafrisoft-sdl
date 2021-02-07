#define CLOG_TAG "SDL_aosp"
#include <Comm/Global/BuildOptions.h>
#include <Comm/OAL/Log.hpp>

#ifdef WIN_CAFRISOFT_AOSP
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <assert.h>

typedef int WindowSurface;

#else
#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <gui/Surface.h>
#include <ui/DisplayInfo.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "EGLUtils.h"
#include "WindowSurface.h"

#include "../android/SDL_android.h"
#include "../../video/android/SDL_androidvideo.h"

using namespace android;
#endif

#include "SDL_aosp.h"


static int s_ScreenWidth = 1920;
static int s_ScreenHeight = 1080;

static int s_UserWindowWidth = 1920;
static int s_UserWindowHeight = 1080;
static bool s_IsAospInit = false;

#ifdef WIN_CAFRISOFT_AOSP

#else
static EGLDisplay eglDisplay;
static EGLSurface eglSurface;
static EGLContext eglContext;
static GLuint texture;

#endif

static void gluLookAt(float eyeX, float eyeY, float eyeZ,
    float centerX, float centerY, float centerZ, float upX, float upY,
    float upZ)
{
    // See the OpenGL GLUT documentation for gluLookAt for a description
    // of the algorithm. We implement it in a straightforward way:

    float fx = centerX - eyeX;
    float fy = centerY - eyeY;
    float fz = centerZ - eyeZ;

    // Normalize f
    float rlf = 1.0f / sqrtf(fx * fx + fy * fy + fz * fz);
    fx *= rlf;
    fy *= rlf;
    fz *= rlf;

    // Normalize up
    float rlup = 1.0f / sqrtf(upX * upX + upY * upY + upZ * upZ);
    upX *= rlup;
    upY *= rlup;
    upZ *= rlup;

    // compute s = f x up (x means "cross product")

    float sx = fy * upZ - fz * upY;
    float sy = fz * upX - fx * upZ;
    float sz = fx * upY - fy * upX;

    // compute u = s x f
    float ux = sy * fz - sz * fy;
    float uy = sz * fx - sx * fz;
    float uz = sx * fy - sy * fx;

    float m[16];
    m[0] = sx;
    m[1] = ux;
    m[2] = -fx;
    m[3] = 0.0f;

    m[4] = sy;
    m[5] = uy;
    m[6] = -fy;
    m[7] = 0.0f;

    m[8] = sz;
    m[9] = uz;
    m[10] = -fz;
    m[11] = 0.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;

    glMultMatrixf(m);
    glTranslatef(-eyeX, -eyeY, -eyeZ);
}

static void printGLString(const char* name, GLenum s) {
    const char* v = (const char*)glGetString(s);
    fprintf(stderr, "GL %s = %s\n", name, v);
}

static int init_gl_surface(const WindowSurface* pWindowSurface)
{
#ifdef WIN_CAFRISOFT_AOSP
    return 1;
#else
    EGLConfig myConfig = { 0 };
    EGLint attrib[] =
    {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };

    if ((eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY)
    {
        printf("eglGetDisplay failed\n");
        return 0;
    }

    if (eglInitialize(eglDisplay, NULL, NULL) != EGL_TRUE)
    {
        printf("eglInitialize failed\n");
        return 0;
    }

#if 0
    if (!printEGLConfigurations(eglDisplay)) {
        printf("printEGLConfigurations failed.\n");
        return 0;
    }
#endif

    EGLNativeWindowType window = pWindowSurface->getSurface();
    EGLUtils::selectConfigForNativeWindow(eglDisplay, attrib, window, &myConfig);

    if ((eglSurface = eglCreateWindowSurface(eglDisplay, myConfig,
        window, 0)) == EGL_NO_SURFACE)
    {
        printf("eglCreateWindowSurface failed\n");
        return 0;
    }

    if ((eglContext = eglCreateContext(eglDisplay, myConfig, 0, 0)) == EGL_NO_CONTEXT)
    {
        printf("eglCreateContext failed\n");
        return 0;
    }

    if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) != EGL_TRUE)
    {
        printf("eglMakeCurrent failed\n");
        return 0;
    }

    int w, h;

    eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &w);
    //checkEglError("eglQuerySurface");
    eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &h);
    //checkEglError("eglQuerySurface");

    fprintf(stderr, "Window dimensions: %d x %d\n", w, h);

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    return 1;
#endif
}

static void free_gl_surface(void)
{
#ifdef WIN_CAFRISOFT_AOSP
    return ;
#else
    if (eglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE,
            EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(eglDisplay, eglContext);
        eglDestroySurface(eglDisplay, eglSurface);
        eglTerminate(eglDisplay);
        eglDisplay = EGL_NO_DISPLAY;
    }
#endif
}

void init_scene(void)
{
    glDisable(GL_DITHER);
    glEnable(GL_CULL_FACE);
    float ratio = 320.0f / 480.0f;
    glViewport(0, 0, 320, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifndef WIN_CAFRISOFT_AOSP
    glFrustumf(-ratio, ratio, -1, 1, 1, 10);
#endif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 0, 3,  // eye
        0, 0, 0,  // center
        0, 1, 0); // up
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Aosp_Init() {
    
#ifdef WIN_CAFRISOFT_AOSP

#else
    sp<IBinder> mainDpy = SurfaceComposerClient::getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain);
    DisplayInfo mainDpyInfo;
    int err = SurfaceComposerClient::getDisplayInfo(mainDpy, &mainDpyInfo);
    if (err != NO_ERROR) {
        fprintf(stderr, "ERROR: unable to get display characteristics\n");
        assert(0);
        return;
    }

    uint32_t width, height;
    if (mainDpyInfo.orientation != DISPLAY_ORIENTATION_0 &&
        mainDpyInfo.orientation != DISPLAY_ORIENTATION_180) {
        // rotated
        width = mainDpyInfo.h;
        height = mainDpyInfo.w;
    }
    else {
        width = mainDpyInfo.w;
        height = mainDpyInfo.h;
    }

    s_ScreenWidth = width;
    s_ScreenHeight = height;

    Android_SetScreenResolution(width, height, width, height, PIXEL_FORMAT_RGBX_8888, 30);
#endif

    Android_Aosp_Event_Init();

    Comm::OAL::Log::SysLogOpen(CLOG_TAG);
    Comm::OAL::Log::EnableDebug();

}

void Android_Aosp_SetOrientation(int w, int h, int resizable, const char* hint) {

    CAFRI_LOGD("w=%d h=%d \n", w, h);

    s_UserWindowWidth = w;
    s_UserWindowHeight = h;
}

int Android_Aosp_GetUserWindowWidth() {

    return s_UserWindowWidth;
}

int Android_Aosp_GetUserWindowHeight() {

    return s_UserWindowHeight;
}

static WindowSurface* s_windowSurface = NULL;

#ifndef WIN_CAFRISOFT_AOSP
static EGLNativeWindowType s_ANW;
#endif

void* Android_Aosp_GetNativeWindow(void) {

#ifdef WIN_CAFRISOFT_AOSP
    return NULL;
#else
    assert(s_windowSurface == NULL);

    //WindowSurface* windowSurface = new WindowSurface(s_UserWindowWidth, s_UserWindowHeight);
    WindowSurface* windowSurface = new WindowSurface(s_ScreenWidth, s_ScreenHeight);
    s_ANW = windowSurface->getSurface();
#if 0
    if (!init_gl_surface(windowSurface))
    {
        printf("GL initialisation failed - exiting\n");
        return 0;
    }
    //init_scene();
#endif

    return (void*)s_ANW;
#endif
}

void Android_Aosp_SetWindowStyle(SDL_bool fullscreen) {

}

void Android_Aosp_SetSurfaceViewFormat(int format) {

#ifdef WIN_CAFRISOFT_AOSP

#else
    assert(s_windowSurface);
    native_window_set_buffers_format(s_ANW, format);
#endif
}

void Android_Aosp_SetActivityTitle(const char* title) {

}

const char* SDL_AospGetInternalStoragePath(void) {

    return "/data/SDL";
}

int Android_Aosp_FileOpen(SDL_RWops* ctx, const char* fileName, const char* mode) {

#ifdef WIN_CAFRISOFT_AOSP

    return -1;
#else

    int iret;
    FILE* fp = fopen(fileName, mode);
    if (fp) {
        ctx->hidden.androidio.asset = (void*)fp;
        iret = 0;
    }
    else {
        ctx->hidden.androidio.asset = (void*)NULL;
        iret = -1;
    }
    return iret;
#endif
}

int Android_Aosp_FileClose(SDL_RWops* ctx) {

#ifdef WIN_CAFRISOFT_AOSP

    return -1;
#else

    FILE* fp = (FILE*)ctx->hidden.androidio.asset;
    if (fp) {
        fclose(fp);
    }
    ctx->hidden.androidio.asset = (void*)NULL;
    return 0;
#endif
}

size_t Android_Aosp_FileRead(SDL_RWops* ctx, void* buffer, size_t size, size_t maxnum) {

#ifdef WIN_CAFRISOFT_AOSP

    return 0;
#else

    size_t rdsz = 0;
    FILE* fp = (FILE*)ctx->hidden.androidio.asset;
    if (fp) {
        rdsz = fread(buffer, size, maxnum, fp);
    }
    return rdsz;

#endif
}

size_t Android_Aosp_FileWrite(SDL_RWops* ctx, const void* buffer, size_t size, size_t num) {

#ifdef WIN_CAFRISOFT_AOSP

    return 0;
#else

    size_t wrsz = 0;
    FILE* fp = (FILE*)ctx->hidden.androidio.asset;
    if (fp) {
        wrsz = fwrite(buffer, size, num, fp);
    }
    return wrsz;
#endif
}

Sint64 Android_Aosp_FileSize(SDL_RWops* ctx) {

#ifdef WIN_CAFRISOFT_AOSP

    return 0;
#else

    Sint64 fsize = 0;
    FILE* fp = (FILE*)ctx->hidden.androidio.asset;
    if (fp) {
        long long fpos = ftello(fp);
        fseeko(fp, 0, SEEK_END);
        fsize = ftello(fp);
        fseeko(fp, fpos, SEEK_SET);
    }
    return fsize;

#endif
}

Sint64 Android_Aosp_FileSeek(SDL_RWops* ctx, Sint64 offset, int whence) {

#ifdef WIN_CAFRISOFT_AOSP

    return 0;
#else

    Sint64 fpos = 0;
    FILE* fp = (FILE*)ctx->hidden.androidio.asset;
    if (fp) {
        fseeko(fp, offset, whence);
        fpos = ftello(fp);
    }
    return fpos;

#endif
}

void Android_Aosp_PollInputDevices(void) {

}

SDL_bool Android_Aosp_IsDeXMode(void) {
    return SDL_FALSE;
}

SDL_bool Android_Aosp_IsChromebook(void) {
    return SDL_FALSE;
}