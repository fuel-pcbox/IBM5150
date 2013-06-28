#ifndef CGA_H_INCLUDE
#define CGA_H_INCLUDE

#include "interface.h"
#include "misc.h"
#include "mda.h"

namespace CGA
{
extern u8 ROM[0x2000];
extern u8 crtcindex;
extern u8 htotal;
extern u8 hdisp;
extern u8 hsyncp;
extern u8 hsyncw;
extern u8 vtotal;
extern u8 vtotala;
extern u8 vdisp;
extern u8 vsyncp;
extern u8 maxscan;
extern u8 curstart;
extern u8 curend;
extern u16 startaddr;
extern u16 curaddr;

extern int framecount;

extern bool rgb; //Not used yet.

extern u8 dispmode; //Bit 1 must be set at all times, unless the processor wants to be halted.
extern u8 status;

extern u8 color;

extern u8 palette[16][3];

u8 status_r(u16 addr);
void putpix(int x, int y, u8 r, u8 g, u8 b);

void tick_frame();
void cga_w(u16 addr, u8 value);

extern iohandler cgacrtc;
}

#endif // CGA_H_INCLUDE
