#include "interface.h"

#include "savestate.h"   

namespace INTERFACE
{

bool quitflag = false;
bool emulatingflag = true;
Surface* screen = NULL;

extern inline int init(int width, int height);
extern inline void quit();
extern inline void window_caption(const char *title, const char *icon);
extern inline int update_screen();


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
            case SDLK_SCROLLOCK:
            {
                emulatingflag ^= 1; //Toggle the emulating flag.
                break;
            }
            case SDLK_s:
            {
                if(!emulatingflag) savestate_save();
                else
                {
                    CPU::hint = true;
                    CPU::hintnum = 1;
                    PPI::keyboardshift.push_back(0x1F);
                }
                break;
            }
            }
        }
        if(e.type == SDL_KEYUP)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_s:
                {
                    if(emulatingflag)
                    {
                        CPU::hint = true;
                        CPU::hintnum = 1;
                        PPI::keyboardshift.push_back(0x9F);
                    }
                    break;
                }
            }
        }
    }
}

} //namespace INTERFACE
