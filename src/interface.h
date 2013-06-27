#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

/* A thin wrapper around SDL (and potentially other libraries), that handles screen,
 * keyboard & mouse on the host computer.
 */

#include <functional>

//TODO: why not a class?
namespace INTERFACE
{

#define USE_SDL
#ifdef USE_SDL

//TODO: fix this dirty workaround
#ifdef dx
#undef dx
#include <SDL/SDL.h>
#define dx dw.w
#else
#include <SDL/SDL.h>
#endif

typedef SDL_Surface Surface;
typedef SDL_Event Event;
#endif // USE_SDL

extern Surface* screen;

int init(int width = 720, int height = 350);
void quit();
void window_caption(const char *title, const char *icon = NULL);
int update_screen();
int handle_events(std::function<void(Event)> handle_event);


} //namespace INTERFACE

#endif // INTERFACE_H_INCLUDED
