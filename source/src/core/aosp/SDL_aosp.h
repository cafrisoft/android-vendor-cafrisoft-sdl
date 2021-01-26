#include "../../SDL_internal.h"
#include "SDL_system.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern void Aosp_Init();
    extern void Aosp_Print_DisplayInfo();

    extern void* Android_Aosp_GetNativeWindow(void);
    extern void Android_Aosp_SetOrientation(int w, int h, int resizable, const char* hint);
    extern void Android_Aosp_SetSurfaceViewFormat(int format);
    extern void Android_Aosp_SetActivityTitle(const char* title);

    extern void Aosp_TestEx1();

#ifdef __cplusplus
}
#endif

