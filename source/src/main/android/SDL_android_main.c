/*
    SDL_android_main.c, placed in the public domain by Sam Lantinga  3/13/14

    As of SDL 2.0.6 this file is no longer necessary.
*/

/* vi: set ts=4 sw=4 expandtab: */


#ifdef ANDROID_CAFRISOFT_AOSP

/* Include the SDL main definition header */
#include "SDL.h"
#include "SDL_main.h"

#ifdef main
#  undef main
#endif /* main */

typedef int bool;
#define true 1
#define false 0

#define ANDROID_POPEN  popen
#define ANDROID_PCLOSE  pclose

#define ANDROID_GET_DISPLAY_RESOLUTION_COMMAND "wm size"

bool Android_GetDisplayResolution(int* width, int* height, int default_width, int default_height) {

    bool bRet = false;
    static int s_DisplayWidth = -1;
    static int s_DisplayHeight = -1;



    if ((s_DisplayWidth == -1)
        || (s_DisplayHeight == -1)) {


        FILE* fp = ANDROID_POPEN(ANDROID_GET_DISPLAY_RESOLUTION_COMMAND, "r");  // expected  'Physical size: 1644x3840'
        if (fp) {
            char buf[512];
            while (1) {
                char* p = fgets(buf, 512, fp);
                if (p) {
                    // Expected : "Proc # 0: fore  T/A/T trm : 0 4247 : com.anapass.androidcanvasexample / u0a0(top - activity)";
                    char* b = strstr(p, "Physical size:");
                    if (b) {

                        b = strchr(b, ':');
                        if (b) {

                            char* szW = b + 1;
                            char* szH;
                            b = strchr(szW, 'x');
                            if (b) {
                                *b = '\0';
                                szH = b + 1;

                                int w = atoi(szW);
                                int h = atoi(szH);

                                if (((w > 100) && (w < 16440))
                                    && (h > 100 && (h < 38400))) {

                                    s_DisplayWidth = w;
                                    s_DisplayHeight = h;

                                    bRet = true;
                                }

                            }
                            break;
                        }
                    }
                }
                else {
                    break;
                }
            }
            ANDROID_PCLOSE(fp);
        }
    }
    else {
        bRet = true;
    }

    if (bRet) {
        if (width) *width = s_DisplayWidth;
        if (height) *height = s_DisplayHeight;
    }
    else {
        if (width) *width = default_width;
        if (height) *height = default_height;
        bRet = true;
    }

    return bRet;
}

int main(int argc, char* argv[]) {

    int iret;

    iret = SDL_main(argc, argv);

    return iret;
}

#endif

