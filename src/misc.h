#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#include <cstdint>
#include <vector>
#include <functional>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;

struct iohandler
{
    u16 start;
    u16 end;
    std::function<u8(u16)> rb;
    std::function<void(u16,u8)> wb;
};

namespace IO_XT
{
extern std::vector<iohandler> handlers;

u8 rb(u16 addr);
void wb(u16 addr, u8 value);

} //namespace IO_XT


namespace PIT
{
struct
{
    u8 mode;
    u8 accessmode;
    bool gate_in;
    bool gate_out;
    bool enabled;
    u16 counter;
    u16 reload;
} chan[3];

void tick();
u8 rb(u16 addr);
void wb(u16 addr, u8 data);

extern iohandler pit;

} //namespace PIT

namespace PIC
{
struct
{
    u8 icw1,icw3,icw4;

    bool init1;
    bool init2;

    bool enabled;

    u8 intrmask;

    u8 ocw2,ocw3;

    u8 offset;
} pic[1]; //The [1] is there to ease future AT implementation.

void pic1_w(u16 addr, u8 value);

extern iohandler pic1;

} //namespace PIC

namespace DMA_XT
{
typedef struct DMA_chan
{
    u16 start_addr;
    u8 page;
    u16 count;
    bool access_flip_flop;
} DMA_chan;

extern DMA_chan chan[4];

void page_w(u16 addr, u8 value);
u8 page_r(u16 addr);

extern iohandler handler, handler2;
} //namespace DMA_XT

namespace PPI
{
extern u8 control;
extern u8 porta;
extern u8 portb;
extern u8 portc;
extern bool dipsw1set;

u8 rb(u16 addr);
void wb(u16 addr, u8 data);

extern iohandler handler;
}  //namespace PPI

#endif // MISC_H_INCLUDED
