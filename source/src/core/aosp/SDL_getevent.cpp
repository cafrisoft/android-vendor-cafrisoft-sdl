#define CLOG_TAG "SDL_aosp"
#define ATRACE_TAG ATRACE_TAG_GRAPHICS
#define LOG_NDEBUG 0

#include <Comm/Global/BuildOptions.h>
#include <Comm/OAL/Thread.hpp>
#include <Comm/OAL/Log.hpp>
#include "SDL_aosp.h"

#ifndef WIN_CAFRISOFT_AOSP
#include <pthread.h>
#endif

#include <android/keycodes.h>

/*
130|odroidn2:/ # getevent -lt
add device 1: /dev/input/event6
  name:     "Dell KB216 Wired Keyboard"
add device 2: /dev/input/event5
  name:     "Dell KB216 Wired Keyboard"
add device 3: /dev/input/event3
  name:     "vt-input"
add device 4: /dev/input/event2
  name:     "cec_input"
could not get driver version for /dev/input/mice, Not a typewriter
add device 5: /dev/input/event1
  name:     "aml_vkeypad"
add device 6: /dev/input/event0
  name:     "aml_keypad"
could not get driver version for /dev/input/mouse0, Not a typewriter
could not get driver version for /dev/input/mouse1, Not a typewriter
add device 7: /dev/input/event4
  name:     "PixArt USB Optical Mouse"

[   23719.986376] /dev/input/event5: EV_MSC       MSC_SCAN             000700e1
[   23719.986376] /dev/input/event5: EV_KEY       KEY_LEFTSHIFT        DOWN
[   23719.986376] /dev/input/event5: EV_SYN       SYN_REPORT           00000000
[   23720.114377] /dev/input/event5: EV_MSC       MSC_SCAN             000700e1
[   23720.114377] /dev/input/event5: EV_KEY       KEY_LEFTSHIFT        UP
[   23720.114377] /dev/input/event5: EV_SYN       SYN_REPORT           00000000
[   23720.770374] /dev/input/event5: EV_MSC       MSC_SCAN             00070016
[   23720.770374] /dev/input/event5: EV_KEY       KEY_S                DOWN
[   23720.770374] /dev/input/event5: EV_SYN       SYN_REPORT           00000000
[   23720.882375] /dev/input/event5: EV_MSC       MSC_SCAN             00070016
[   23720.882375] /dev/input/event5: EV_KEY       KEY_S                UP
*/

#ifdef WIN_CAFRISOFT_AOSP

#define AOSP_ROOT_PATH "../../../source/src/core/aosp"

static FILE* CAFRISOFT_POPEN(const char* pname, const char* mode) {

    FILE* fp = fopen(AOSP_ROOT_PATH "/system_bin_getevent_lt.txt", mode);
    assert(fp);
    return fp;
}

static int CAFRISOFT_PCLOSE(FILE* fp) {
    return fclose(fp);
}

#else
#define CAFRISOFT_POPEN popen
#define CAFRISOFT_PCLOSE pclose
#endif

extern "C" int Android_OnKeyDown(int keycode);
extern "C" int Android_OnKeyUp(int keycode);

struct AospKeyCode {
    const char* szKeyCode;
    int androidKeyCodde;
};

static struct AospKeyCode s_AospKeyCode[] = {
    {"KEY_DOWN", AKEYCODE_DPAD_DOWN}
    ,{"KEY_UP", AKEYCODE_DPAD_UP}
    ,{"KEY_LEFT", AKEYCODE_DPAD_LEFT}
    ,{"KEY_RIGHT", AKEYCODE_DPAD_RIGHT}
    ,{"KEY_SPACE", AKEYCODE_SPACE}
    ,{"KEY_ENTER", AKEYCODE_ENTER}
};

static void send_event(const char* szTime, const char* szDev, const char* szEvent, const char* szCode, const char* szValue) {

    if (strcmp(szEvent, "EV_KEY") == 0) {

        int sz = sizeof(s_AospKeyCode) / sizeof(s_AospKeyCode[0]);
        for (int i = 0; i < sz; i++) {
       
            struct AospKeyCode* pCode = &s_AospKeyCode[i];
            if (strcmp(pCode->szKeyCode, szCode) == 0) {
            
                if (strcmp(szValue, "DOWN") == 0) {
                    Android_OnKeyDown(pCode->androidKeyCodde);
                }
                else if (strcmp(szValue, "UP") == 0) {
                    Android_OnKeyUp(pCode->androidKeyCodde);
                }
                break;
            }
        }

#if 0
        if (strcmp(szCode, "KEY_DOWN") == 0) {
            if (strcmp(szValue, "DOWN") == 0) {
                Android_OnKeyDown(AKEYCODE_DPAD_DOWN);
            }
            else if (strcmp(szValue, "UP") == 0) {
                Android_OnKeyUp(AKEYCODE_DPAD_DOWN);
            }
        }
        else if (strcmp(szCode, "KEY_UP") == 0) {
            if (strcmp(szValue, "DOWN") == 0) {
                Android_OnKeyDown(AKEYCODE_DPAD_UP);
            }
            else if (strcmp(szValue, "UP") == 0) {
                Android_OnKeyUp(AKEYCODE_DPAD_UP);
            }
        }
        else if (strcmp(szCode, "KEY_LEFT") == 0) {
            if (strcmp(szValue, "DOWN") == 0) {
                Android_OnKeyDown(AKEYCODE_DPAD_LEFT);
            }
            else if (strcmp(szValue, "UP") == 0) {
                Android_OnKeyUp(AKEYCODE_DPAD_LEFT);
            }
        }
        else if (strcmp(szCode, "KEY_RIGHT") == 0) {
            if (strcmp(szValue, "DOWN") == 0) {
                Android_OnKeyDown(AKEYCODE_DPAD_RIGHT);
            }
            else if (strcmp(szValue, "UP") == 0) {
                Android_OnKeyUp(AKEYCODE_DPAD_RIGHT);
            }
        }
#endif

    }
}

static void service_events_proc(void* arg) {

    char* szbuf = new char[1024];
    assert(szbuf);

    //CLOGD(" ");

    FILE* fp = CAFRISOFT_POPEN("/system/bin/getevent -lt", "r");
    assert(fp);

#if 0
    while (true) {

        CLOGI("AAAAAAA");
        Comm::OAL::Thread::Sleep(1000);
    }
#endif

    while (true) {
    
#if 0
        char ch;
        int bufIdx = 0;
        while (true) {
            fscanf(fp, "%c", &ch);
            if (ch == '\n' || ch == '\r') {
                szbuf[bufIdx++] = '\0';
                break;
            }
            szbuf[bufIdx++] = ch;
        }
#else
        char * p = fgets(szbuf, 1024, fp);

#endif

        if (p != NULL) {

            char* a, * b;
            char szTime[32];
            char szDev[32];
            char szEvent[32];
            char szCode[32];
            char szValue[32];

            //맨끝 \n\r' ' 제거 
            int szlen = (int)strlen(szbuf);
            for (int i = szlen - 1; i >= 0; i--) {
                char c = szbuf[i];
                if (c == ' ' || c == '\r' || c == '\n') {
                    szbuf[i] = '\0';
                }
                else {
                    break;
                }
            }


            //Time 
            a = szbuf;
            b = strchr(a, '[');
            if (b == NULL) continue;

            a = b + 1;
            b = strchr(a, ']');
            if (b == NULL) continue;
            *b = '\0';
            strcpy(szTime, a);
            a = b + 1;

            // dev/input/event
            b = strchr(a, ':');
            if (b == NULL) continue;
            *b = '\0';
            strcpy(szDev, a);
            a = b + 1;

            // event
            b = strstr(a, "EV_");
            if (b == NULL) continue;
            a = b;
            b = strchr(a, ' ');
            if (b == NULL) continue;
            *b = '\0';
            strcpy(szEvent, a);
            a = b + 1;

            // Code
            while (true) {
                if (*a != ' ') {
                    break;
                }
                a++;
            }
            b = strchr(a, ' ');
            if (b == NULL) continue;
            *b = '\0';
            strcpy(szCode, a);
            a = b + 1;

            // Value
            while (true) {
                if (*a != ' ') {
                    break;
                }
                a++;
            }
            strcpy(szValue, a);
            
            //PRINTF("%s %s %s %s \n", szTime, szDev, szEvent, szCode, szValue);
            CLOGI("%s %s %s %s %s \n", szTime, szDev, szEvent, szCode, szValue);
            send_event(szTime, szDev, szEvent, szCode, szValue);
        }
    }
    
    CAFRISOFT_PCLOSE(fp);
    delete[] szbuf;
}



static std::shared_ptr<Comm::OAL::Thread> s_EventThread;

void Android_Aosp_Event_Init(void) {

    s_EventThread = Comm::OAL::Thread::CreateObject(service_events_proc, NULL);
    assert(s_EventThread);

}