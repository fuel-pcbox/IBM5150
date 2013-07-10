#include <cstdio>
#include "misc.h"
#include "cpu.h"

namespace IO_XT
{
std::vector<iohandler> handlers;

u8 rb(u16 addr)
{
    int i;
    for(i = 0; i<handlers.size(); i++)
    {
        if(addr>=handlers[i].start && addr<=handlers[i].end && handlers[i].rb != NULL) return handlers[i].rb(addr-handlers[i].start);
    }
    return 0;
}
void wb(u16 addr, u8 value)
{
    int i;
    printf("I/O port write to %04X with a value of %02X!\n",addr,value);
    if(handlers.size() != 0)
    {
        for(i = 0; i<handlers.size(); i++)
        {
            if(addr>=handlers[i].start && addr<=handlers[i].end && handlers[i].wb != NULL){ handlers[i].wb(addr-handlers[i].start,value); return;}
        }
        printf("Calling callback!\n");
    }
}

} //namespace IO_XT

namespace PIT
{
void tick()
{
    for(int i = 0; i<3; i++)
    {
        switch(chan[i].mode)
        {
        case 0:
        {
            if(i == 2)
            {
            }
            else
            {
                if(chan[i].enabled)
                {
                    if(chan[i].counter==0)
                    {
                        //chan[i].counter = chan[i].reload;
                        chan[i].gate_out = true;
                        CPU::hint = true;
                        CPU::hintnum = 0;
                    }
                    chan[i].counter--;
                }
            }
            break;
        }
        case 2:
        {
            if(i == 2)
            {
            }
            else
            {
                if(chan[i].enabled)
                {
                    chan[i].gate_out = true;
                    chan[i].counter--;
                    if(chan[i].counter==1) chan[i].gate_out = false;
                    if(chan[i].counter==0)
                    {
                        chan[i].gate_out = true;
                        chan[i].counter = chan[i].reload;
                    }
                }
            }
            break;
        }
        case 3:
        {
            if(i == 2)
            {
            }
            else
            {
                if(chan[i].enabled)
                {
                    if(chan[i].counter != 0 && chan[i].counter != 1)
                    {
                        if(chan[i].flip_flop == false)
                        {
                            chan[i].gate_out ^= 1;
                            if(chan[i].gate_out == true)
                            {
                                CPU::hint = true;
                                CPU::hintnum = 0;
                            } 
                        }
                        chan[i].flip_flop = true;
                    }
                    chan[i].counter--;
                    chan[i].counter--;
                    if(chan[i].counter==1)
                    {
                        chan[i].flip_flop = false;
                        chan[i].gate_out ^= 1;
                        if(chan[i].gate_out == true)
                        {
                            CPU::hint = true;
                            CPU::hintnum = 0;
                        }
                    }
                    if(chan[i].counter==0)
                    {
                        chan[i].flip_flop = true;
                        chan[i].gate_out ^= 1;
                        if(chan[i].gate_out == true)
                        {
                            CPU::hint = true;
                            CPU::hintnum = 0;
                        }
                        chan[i].counter = chan[i].reload;
                    }
                }
            }
            break;
        }
        }
    }
}

u8 rb(u16 addr)
{
    switch(addr)
    {
    case 1:
    {
        if(chan[1].accessmode == 0)
        {
            chan[1].enabled = true;
            return chan[1].counter;
        }
        break;
    }
    }
    return 0;
}

void wb(u16 addr, u8 data)
{
    switch(addr)
    {
    case 0:
    {
        switch(chan[0].accessmode)
        {
        case 1:
        {
            chan[0].reload = (chan[0].reload & 0xFF00) | data;
            chan[0].enabled = true;
            break;
        }
        case 3:
        {
            if(!chan[0].flip_flop)
            {
                chan[0].reload = (chan[0].reload & 0xFF00) | data;
            }
            else
            {
                chan[0].reload = (data << 8) | (chan[0].reload & 0x00FF);
            }
            break;
        }
        }
        break;
    }
    case 1:
    {
        switch(chan[1].accessmode)
        {
        case 1:
        {
            chan[1].reload = (chan[1].reload & 0xFF00) | data;
            chan[1].enabled = true;
            break;
        }
        }
        break;
    }
    case 3:
    {
        switch(data & 0xC0)
        {
        case 0:
        {
            chan[0].accessmode = (data >> 4) & 3;
            chan[0].mode = (data >> 1) & 7;
            chan[0].enabled = false;
            chan[0].flip_flop = false;
            break;
        }
        case 1:
        {
            chan[1].accessmode = (data >> 4) & 3;
            chan[1].mode = (data >> 1) & 7;
            chan[1].enabled = false;
            break;
        }
        case 2:
        {
            chan[2].accessmode = (data >> 4) & 3;
            chan[2].mode = (data >> 1) & 7;
            chan[2].enabled = false;
            break;
        }
        }
        break;
    }
    }
}

iohandler pit = {0x0040,0x0043,rb,wb};
} //namespace PIT

//Programmable Interrupt Controller
namespace PIC
{

pic_t pic[1];

void pic1_w(u16 addr, u8 value)
{
    switch(addr)
    {
    case 0:
    {
        if(value & 0x10)
        {
            pic[0].icw1 = value & 0x1F;
            pic[0].enabled = false;
            pic[0].init1 = true;
        }
        break;
    }
    case 1:
    {
        if(pic[0].init1 == true)
        {
            pic[0].offset = value;
            pic[0].init2 = true;
            pic[0].init1 = false;
        }
        else if(pic[0].init2 == true)
        {
            pic[0].icw3 = 0;
            pic[0].init2 = false;
        }
        else
        {
            pic[0].intrmask = value;
        }
        break;
    }
    }
}
u8 pic1_r(u16 addr)
{
    switch(addr)
    {
        case 0:
        {
            return 0;
            break;
        }
        case 1:
        {
            return pic[0].intrmask;
            break;
        }
    }
}

iohandler pic1 = {0x0020,0x0021,pic1_r,pic1_w};

} //namespace PIC


namespace DMA_XT
{

DMA_chan chan[4];

void page_w(u16 addr, u8 value)
{
    switch(addr)
    {
    case 3:
    {
        chan[3].page = value & 0x0F;
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
        return chan[3].page & 0x0F;
        break;
    }
    }
}

void dma1_w(u16 addr, u8 value)
{
    switch(addr)
    {
    case 0:
    {
        if(!chan[0].access_flip_flop)
        {
            chan[0].start_addr = (chan[0].start_addr & 0xFF00) | value;
            chan[0].access_flip_flop = true;
        }
        else
        {
            chan[0].start_addr = (chan[0].start_addr & 0xFF) | (value << 8);
            chan[0].access_flip_flop = false;
        }
        break;
    }
    case 1:
    {
        if(!chan[0].access_flip_flop)
        {
            chan[0].count = (chan[0].count & 0xFF00) | value;
            chan[0].access_flip_flop = true;
        }
        else
        {
            chan[0].count = (chan[0].count & 0xFF) | (value << 8);
            chan[0].access_flip_flop = false;
        }
        break;
    }
    case 2:
    {
        if(!chan[1].access_flip_flop)
        {
            chan[1].start_addr = (chan[1].start_addr & 0xFF00) | value;
            chan[1].access_flip_flop = true;
        }
        else
        {
            chan[1].start_addr = (chan[1].start_addr & 0xFF) | (value << 8);
            chan[1].access_flip_flop = false;
        }
        break;
    }
    case 3:
    {
        if(!chan[1].access_flip_flop)
        {
            chan[1].count = (chan[1].count & 0xFF00) | value;
            chan[1].access_flip_flop = true;
        }
        else
        {
            chan[1].count = (chan[1].count & 0xFF) | (value << 8);
            chan[1].access_flip_flop = false;
        }
        break;
    }
	case 4:
    {
        if(!chan[2].access_flip_flop)
        {
            chan[2].start_addr = (chan[2].start_addr & 0xFF00) | value;
            chan[2].access_flip_flop = true;
        }
        else
        {
            chan[2].start_addr = (chan[2].start_addr & 0xFF) | (value << 8);
            chan[2].access_flip_flop = false;
        }
        break;
    }
    case 5:
    {
        if(!chan[2].access_flip_flop)
        {
            chan[2].count = (chan[2].count & 0xFF00) | value;
            chan[2].access_flip_flop = true;
        }
        else
        {
            chan[2].count = (chan[2].count & 0xFF) | (value << 8);
            chan[2].access_flip_flop = false;
        }
        break;
    }
	case 6:
    {
        if(!chan[3].access_flip_flop)
        {
            chan[3].start_addr = (chan[3].start_addr & 0xFF00) | value;
            chan[3].access_flip_flop = true;
        }
        else
        {
            chan[3].start_addr = (chan[3].start_addr & 0xFF) | (value << 8);
            chan[3].access_flip_flop = false;
        }
        break;
    }
    case 7:
    {
        if(!chan[3].access_flip_flop)
        {
            chan[3].count = (chan[3].count & 0xFF00) | value;
            chan[3].access_flip_flop = true;
        }
        else
        {
            chan[3].count = (chan[3].count & 0xFF) | (value << 8);
            chan[3].access_flip_flop = false;
        }
        break;
    }
    }
}

u8 dma1_r(u16 addr)
{
    switch(addr)
    {
    case 0:
    {
        if(!chan[0].access_flip_flop)
        {
            chan[0].access_flip_flop = true;
            return (chan[0].start_addr & 0xFF);
        }
        else
        {
            chan[0].access_flip_flop = false;
            return (chan[0].start_addr & 0xFF00) >> 8;
        }
        break;
    }
    case 1:
    {
        if(!chan[0].access_flip_flop)
        {
            chan[0].access_flip_flop = true;
            return (chan[0].count & 0xFF);
        }
        else
        {
            chan[0].access_flip_flop = false;
            return (chan[0].count & 0xFF00) >> 8;
        }
        break;
    }
    case 2:
    {
        if(!chan[1].access_flip_flop)
        {
            chan[1].access_flip_flop = true;
            return (chan[1].start_addr & 0xFF);
        }
        else
        {
            chan[1].access_flip_flop = false;
            return (chan[1].start_addr & 0xFF00) >> 8;
        }
        break;
    }
    case 3:
    {
        if(!chan[1].access_flip_flop)
        {
            chan[1].access_flip_flop = true;
            return (chan[1].count & 0xFF);
        }
        else
        {
            chan[1].access_flip_flop = false;
            return (chan[1].count & 0xFF00) >> 8;
        }
        break;
    }
	case 4:
    {
        if(!chan[2].access_flip_flop)
        {
            chan[2].access_flip_flop = true;
            return (chan[2].start_addr & 0xFF);
        }
        else
        {
            chan[2].access_flip_flop = false;
            return (chan[2].start_addr & 0xFF00) >> 8;
        }
        break;
    }
    case 5:
    {
        if(!chan[2].access_flip_flop)
        {
            chan[2].access_flip_flop = true;
            return (chan[2].count & 0xFF);
        }
        else
        {
            chan[2].access_flip_flop = false;
            return (chan[2].count & 0xFF00) >> 8;
        }
        break;
    }
	case 6:
    {
        if(!chan[3].access_flip_flop)
        {
            chan[3].access_flip_flop = true;
            return (chan[3].start_addr & 0xFF);
        }
        else
        {
            chan[3].access_flip_flop = false;
            return (chan[3].start_addr & 0xFF00) >> 8;
        }
        break;
    }
    case 7:
    {
        if(!chan[3].access_flip_flop)
        {
            chan[3].access_flip_flop = true;
            return (chan[3].count & 0xFF);
        }
        else
        {
            chan[3].access_flip_flop = false;
            return (chan[3].count & 0xFF00) >> 8;
        }
        break;
    }
    }
}

iohandler handler = {0x0080, 0x0083, page_r, page_w};
iohandler handler2 = {0x0000,0x0007, dma1_r, dma1_w};
} //namespace DMA_XT

namespace PPI
{
u8 control;
u8 porta;
u8 portb;
u8 portc;
bool dipsw1set;
bool keyboardclk;
bool keyboardena;
bool keyboardena2;

std::vector<u8> keyboardshift; //This is a vector so that input doesn't get lost.

u8 rb(u16 addr)
{
    switch(addr)
    {
        case 0:
        {
            if((keyboardshift.size() != 0))
            {
                u8 ret = keyboardshift.at(keyboardshift.size()-1);
                return ret;
            }
            return 0x00;
            break;
        }
        case 2:
        {
            //TODO: currently hardcoded. 
            if(!dipsw1set) return 0x0C;
            else return 0x00;
            break;
        }
    }
}
void wb(u16 addr, u8 data)
{
    switch(addr)
    {
    case 1:
    {
        if(control & 2) return;
        if(!(control & 4))
        {
            keyboardena = (data & 4) ? true : false;
            dipsw1set = (data & 8) ? true : false;
            keyboardclk = (data & 0x40) ? true : false;    
            keyboardena2 = (data & 0x80) ? true : false;
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
iohandler handler = {0x0060,0x0063,rb,wb};
}  //namespace PPI
