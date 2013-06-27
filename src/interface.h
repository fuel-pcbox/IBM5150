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
#endif

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

extern int init(int width = 720, int height = 350);
extern void quit();
extern void window_caption(const char *title, const char *icon = NULL);
extern int update_screen();
extern int handle_events();


} //namespace INTERFACE

#endif // INTERFACE_H_INCLUDED
