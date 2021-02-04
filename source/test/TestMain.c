/*
  Copyright (C) 1997-2020 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL.h"
#include "SDL_test_font.h"
#include "testyuv_cvt.h"

#ifdef ANDROID_CAFRISOFT_AOSP
#include <unistd.h>
#endif

#ifdef ANDROID_CAFRISOFT_AOSP
extern void Aosp_Init();
#endif

extern int  main_testyuv(int argc, char** argv);

int  main(int argc, char** argv) {

    int iret;

#ifdef ANDROID_CAFRISOFT_AOSP
    Aosp_Init();
#endif

    //main_test(argc, argv);
    iret = main_testyuv(argc, argv);
    return iret;
}
