#include "../../SDL_internal.h"
#include "SDL_system.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern void Aosp_Init();
    extern void Aosp_Print_DisplayInfo();
    
    extern void Android_Aosp_Event_Init(void);

    extern void* Android_Aosp_GetNativeWindow(void);
    extern void Android_Aosp_SetOrientation(int w, int h, int resizable, const char* hint);
    extern void Android_Aosp_SetSurfaceViewFormat(int format);
    extern void Android_Aosp_SetActivityTitle(const char* title);
    extern const char* SDL_AospGetInternalStoragePath(void);

    extern int Android_Aosp_GetUserWindowWidth();
    extern int Android_Aosp_GetUserWindowHeight();

    extern void Aosp_TestEx1();

    extern int Android_Aosp_FileOpen(SDL_RWops* ctx, const char* fileName, const char* mode);
    extern int Android_Aosp_FileClose(SDL_RWops* ctx);
    extern size_t Android_Aosp_FileRead(SDL_RWops* ctx, void* buffer, size_t size, size_t maxnum);
    extern size_t Android_Aosp_FileWrite(SDL_RWops* ctx, const void* buffer, size_t size, size_t num);
    extern Sint64 Android_Aosp_FileSize(SDL_RWops* ctx);
    extern Sint64 Android_Aosp_FileSeek(SDL_RWops* ctx, Sint64 offset, int whence);
    
    extern void Android_Aosp_PollInputDevices(void);
    extern void Android_Aosp_SetWindowStyle(SDL_bool fullscreen);
    extern SDL_bool Android_Aosp_IsDeXMode(void);
    extern SDL_bool Android_Aosp_IsChromebook(void);

#ifdef __cplusplus
}
#endif

