#ifndef __CUBE_H_39D08C98_4195_11E9_9D78_57E78AF59FD9__
#define __CUBE_H_39D08C98_4195_11E9_9D78_57E78AF59FD9__

#define _FILE_OFFSET_BITS 64

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include <time.h>

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #ifdef _WIN32_WINNT
  #undef _WIN32_WINNT
  #endif
  #define _WIN32_WINNT 0x0500
  #include "windows.h"
  #ifndef _WINDOWS
    #define _WINDOWS
  #endif
  #ifndef __GNUC__
    #include <eh.h>
    #include <dbghelp.h>
    #include <intrin.h>
  #endif
  #define ZLIB_DLL
#endif

#ifndef STANDALONE
  #ifdef __APPLE__
    #include "SDL2/SDL.h"
    #define GL_GLEXT_LEGACY
    #define __glext_h_
    #include <OpenGL/gl.h>
    #define main SDL_main
  #else
    #include <SDL.h>
    #include <SDL_opengl.h>
  #endif
#endif

#include <enet/enet.h>

#include <zlib.h>

#include "tools.hh"
#include "geom.hh"
#include "ents.hh"
#include "command.hh"

#ifndef STANDALONE
#include "glexts.hh"
#include "glemu.hh"
#endif

#include "iengine.hh"
#include "igame.hh"

#endif // __CUBE_H_39D08C98_4195_11E9_9D78_57E78AF59FD9__
