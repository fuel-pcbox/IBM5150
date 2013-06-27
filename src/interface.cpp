#include "interface.h"

#include "savestate.h"

namespace INTERFACE
{

#ifndef USE_SDL
#error You need SDL to run this emulator
#endif // USE_SDL

Surface* screen = NULL;
bool quitflag = false;

inline int init(int width, int height)
{
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(width, height, 24, SDL_SWSURFACE);
    return ret;
}

void quit()
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

int handle_events()
{
    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT) quitflag = true;
        if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
            case SDLK_s:
            {
                savestate_save();
                break;
            }
            }
        }
    }
}

} //namespace INTERFACE
