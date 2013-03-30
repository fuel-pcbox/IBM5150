#include <cstdint>
#include <cstdio>
#include <vector>
#include <functional>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;

enum
{
	update_frame = 1,
	update_scanline = 2,
	update_pixel = 4,
	update_clock = 8
};

struct iohandler
{
    u16 start;
    u16 end;
    std::function<u8(u16)> rb;
    std::function<void(u16,u8)> wb;
};

struct memhandler
{
    u32 start;
    u32 end;
    std::function<u8(u32)> rb;
    std::function<void(u32,u8)> wb;
};

namespace CPU
{
    u32 cr0;
    struct
    {
        u32 base;
        u16 limit;
    }gdtr, ldtr;
};

namespace RAM
{
u8 RAM[0x100000];
std::vector<memhandler> handlers;
bool write;
u8 rb(u16 seg, u16 off)
{
    int i;
    u32 addr = getaddr(seg,off);
    for(i = 0; i<=handlers.size(); i++)
    {
        if(i == handlers.size()) break;
        if(addr>handlers[i].start && addr<handlers[i].end) break;
    }
    if(i != handlers.size())return handlers[i].rb(addr);
    else RAM[addr];
}
u32 getaddr(u16 seg, u16 off)
{
    if(!(CPU::cr0 & 1)) return ((seg<<4)+off)&0xFFFFF;
    else
    {
        u8 tmp = seg & 0xFFF8;
        if(seg & 0x0004)
        {
            u32 tmp1 = (CPU::ldtr.base + tmp);
            if(tmp1 > (CPU::ldtr.base + CPU::ldtr.limit)) return 0;
            u32 addr = (RAM[tmp1 + 3]<<16) | ((RAM[tmp1+5]<<8)|(RAM[tmp1+4]));
            if(RAM[tmp1+2] & 0x08)
            {
                if(RAM[tmp1+2]&0x04) addr+=offset;
                else addr-=offset;
            }
            else addr+=offset;
            return addr;
        }
        else
        {
            u32 tmp1 = (CPU::gdtr.base + tmp);
            if(tmp1 > (CPU::gdtr.base + CPU::gdtr.limit)) return 0;
            u32 addr = (RAM[tmp1 + 3]<<16) | ((RAM[tmp1+5]<<8)|(RAM[tmp1+4]));
            if(RAM[tmp1+2] & 0x08)
            {
                if(RAM[tmp1+2]&0x04) addr+=offset;
                else addr-=offset;
            }
            else addr+=offset;
            return addr;
        }
    }
}
void wb(u16 seg, u16 off, u8 value)
{
    int i;
    u32 addr = getaddr(seg,off);
    for(i = 0; i<handlers.size(); i++)
    {
        if(i == handlers.size()) break;
        if(addr>handlers[i].start && addr<handlers[i].end) break;
    }
    if(i != handlers.size()) handlers[i].wb(addr,value);
    else RAM[addr] = value;
}
};

namespace DMA_XT
{
struct DMA_chan
{
    u16 start_addr;
    u8 page;
    u16 count;
} chan0,chan1,chan2,chan3;
void page_w(u16 addr, u8 value)
{
    switch(addr)
    {
    case 3:
    {
        chan3.page = value & 0x0F;
        break;
    }
    }
}
u8 page_r(u16 addr)
{
    switch(addr)
    {
    case 3:
    {
        return chan3.page & 0x0F;
        break;
    }
    }
}
iohandler handler;
void init()
{
    handler.start = 0x0080;
    handler.end = 0x0083;
    handler.rb = &page_r;
    handler.wb = &page_w;
}
};

namespace PPI
{
u8 control;
u8 porta;
u8 portb;
u8 portc;
bool dipsw1set;
void wb(u16 addr, u8 data)
{
    switch(addr)
    {
    case 1:
    {
        if(control & 2) return;
        if(!(control & 4))
        {
            dipsw1set = (data & 8) ? true : false;
        }
        break;
    }
    case 3:
    {
        control = data;
        break;
    }
    }
}
iohandler handler = {0x0060,0x0063,NULL,wb};
};

namespace IO_XT
{
std::vector<iohandler> handlers;
u8 rb(u16 addr)
{
    int i;
    for(i = 0; i<handlers.size(); i++)
    {
        if(addr>=handlers[i].start && addr<=handlers[i].end) break;
    }
    if(handlers[i].wb != NULL) return handlers[i].rb(addr-handlers[i].start);
    else return 0;
}
void wb(u16 addr, u8 value)
{
    int i;
    printf("I/O port write to %04X with a value of %02X!\n",addr,value);
    if(handlers.size() != 0)
    {
        for(i = 0; i<handlers.size(); i++)
        {
            if(addr>=handlers[i].start && addr<=handlers[i].end) break;
        }
        printf("Calling callback!\n");
        if(handlers[i].wb != NULL) handlers[i].wb(addr-handlers[i].start,value);
        else printf("Function uninitialized!\n");
    }
}
};

#include "cpu.h"

int main(int ac, char** av)
{
    if(ac < 2) return 1;
    FILE* bios = fopen(av[1],"rb");
    fseek(bios,0,SEEK_END);
    long size = ftell(bios);
    fseek(bios,0,SEEK_SET);
    fread(RAM::RAM + (0x100000 - size),1,size,bios);
    fclose(bios);

    DMA_XT::init();
    IO_XT::handlers.push_back(DMA_XT::handler);
    IO_XT::handlers.push_back(PPI::handler);

    for(int i = 0; i<65536; i++)
    {
        CPU::tick();
    }
    return 0;
}