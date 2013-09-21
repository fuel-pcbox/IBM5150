#ifndef CPU_H_INCLUDE
#define CPU_H_INCLUDE

#include <cstdio>

#include "misc.h"
#include "ram.h"

namespace CPU
{

enum cputype
{
    intel8086 = 0,
    necv20,
    intel286,
    intel386
};

struct locs
{
    union
    {
        u8* src8;
        u16* src16;
    };
    union
    {
        u8* dst8;
        u16* dst16;
    };
};

enum
{
    SEG_DEFAULT = 0,
    SEG_CS,
    SEG_DS,
    SEG_ES,
    SEG_SS
};

typedef union reg16
{
    struct
    {
        u8 lo,hi;
    } parts;
    u16 w;
} reg16;

extern reg16 aw,bw,cw,dw,ew;

#define ax aw.w
#define al aw.parts.lo
#define ah aw.parts.hi

#define bx bw.w
#define bl bw.parts.lo
#define bh bw.parts.hi

#define cx cw.w
#define cl cw.parts.lo
#define ch cw.parts.hi


#define dx dw.w //And this kids, is why you should **avoid* using #define
#define dl dw.parts.lo
#define dh dw.parts.hi

extern u16 ds,es,ss;
extern u16 sp,bp,si,di;
extern u16 tr;
extern u16 cs;
extern u16 ip;
extern u16 flags;

extern int type;

locs decodemodrm(int seg, u8 modrm, bool word, bool segarg);

extern int seg;
extern int rep; //0 is no rep. 1 is repe. 2 is repne.
extern bool i8080; //This is for the NEC V20.

void rtick();
void tick();


extern bool halted;
extern u32 cr0;

extern struct tablereg
{
    u32 base;
    u16 limit;
} gdtr, ldtr, idtr;

extern bool hint; //Hardware interrupts.
extern u8 hintnum;

} //namespace CPU


#endif // CPU_H_INCLUDE
