#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

/* A thin wrapper around SDL (and potentially other libraries), that handles screen,
 * keyboard & mouse on the host computer.
 */

#include <functional>

#define USE_SDL

#ifdef USE_SDL
    //TODO: fix this dirty workaround
    #ifdef dx
        #undef dx
        #include <SDL/SDL.h>
        #define dx dw.w
    #else
        #include <SDL/SDL.h>
    #endif //dx
#endif // USE_SDL

//TODO: why not a class?
namespace INTERFACE
{

extern bool quitflag;

#ifdef USE_SDL
typedef SDL_Surface Surface;
typedef SDL_Event Event;
#endif

extern Surface* screen;

inline int init(int width = 720, int height = 350)
{
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(width, height, 24, SDL_SWSURFACE);
    return ret;
}

inline void quit()
{
    SDL_Quit();
}

inline void window_caption(const char *title, const char *icon = NULL)
{
    SDL_WM_SetCaption(title, icon);
}

inline int update_screen()
{
    SDL_Flip(screen);
}

int handle_events();


} //namespace INTERFACE

#endif // INTERFACE_H_INCLUDED
