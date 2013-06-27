#include "interface.h"

namespace INTERFACE
{

#ifndef USE_SDL
#error You need SDL to run this emulator
#endif // USE_SDL

Surface* screen = NULL;

inline int init(int width, int height)
{
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(width, height, 24, SDL_SWSURFACE);
    return ret;
}

inline void quit()
{
    SDL_Quit();
}

inline void window_caption(const char *title, const char *icon)
{
    SDL_WM_SetCaption(title, icon);
}

inline int update_screen()
{
    SDL_Flip(screen);
}

int handle_events(std::function<int(Event)> handle_event)
{
    SDL_Event e;

    while(SDL_PollEvent(&e) && handle_event(e) == 0) ;

    return 0;
}

} //namespace INTERFACE
