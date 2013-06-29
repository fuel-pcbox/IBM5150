#include "ram.h"

namespace RAM
{
u8 RAM[0x100000];
std::vector<memhandler> handlers;
bool write;

u32 getaddr(u16 seg, u16 offset)
{
    if(!(CPU::cr0 & 1)) return ((seg<<4)+offset)&0xFFFFF;
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

u8 rb(u16 seg, u16 off)
{
    int i;
    u32 addr = getaddr(seg,off);
    for(i = 0; i<handlers.size(); i++)
    {
        if(i == handlers.size()) break;
        if(addr>handlers[i].start && addr<handlers[i].end) return handlers[i].rb(addr-handlers[i].start);
    }
    return RAM[addr];
}

void wb(u16 seg, u16 off, u8 value)
{
    int i;
    u32 addr = getaddr(seg,off);
    for(i = 0; i<handlers.size(); i++)
    {
        if(i == handlers.size()) break;
        if(addr>handlers[i].start && addr<handlers[i].end)
        {
            handlers[i].wb(addr,value);
            return;
        }
    }
    RAM[addr] = value;
    else return;
}

} //namespace RAM
