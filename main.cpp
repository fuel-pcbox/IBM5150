#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>
#include <functional>
#include <SDL/SDL.h>

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

SDL_Surface* screen = NULL;

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
bool halted = false;
u32 cr0;
struct
{
    u32 base;
    u16 limit;
} gdtr, ldtr;

bool hint = false; //Hardware interrupts.
u8 hintnum = 0;
};

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
            }
            break;
        }
    }
}

iohandler pic1 = {0x0020,0x0021,NULL,pic1_w};

};

namespace RAM
{
u8 RAM[0x100000];
std::vector<memhandler> handlers;
bool write;
u32 getaddr(u16 seg, u16 off);
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
iohandler handler = {0x0080, 0x0083, page_r, page_w};
};

namespace PPI
{
u8 control;
u8 porta;
u8 portb;
u8 portc;
bool dipsw1set;
u8 rb(u16 addr)
{
    return 0;
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
iohandler handler = {0x0060,0x0063,rb,wb};
};

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
    void tick()
    {
        for(int i = 0;i<3;i++)
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
                            chan[i].counter--;
                            if(chan[i].counter==0)
                            {
                                chan[i].counter = chan[i].reload;
                                chan[i].gate_out = true; 
                            }
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
}

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
            if(addr>=handlers[i].start && addr<=handlers[i].end) handlers[i].wb(addr-handlers[i].start,value);
        }
        printf("Calling callback!\n");
    }
}
};

#include "cpu.h"
#include "mda.h"
#include "cga.h"

void savestate_save()
{
    FILE* fp = fopen("save/mem.dump","wb");
    fwrite(RAM::RAM,0x100000,1,fp);
    fclose(fp);
    fp = fopen("save/reg.dump","wb");
    fwrite(&CPU::ax,2,1,fp);
    fwrite(&CPU::bx,2,1,fp);
    fwrite(&CPU::cx,2,1,fp);
    fwrite(&CPU::dx,2,1,fp);
    fwrite(&CPU::si,2,1,fp);
    fwrite(&CPU::di,2,1,fp);
    fwrite(&CPU::sp,2,1,fp);
    fwrite(&CPU::bp,2,1,fp);
    fwrite(&CPU::ip,2,1,fp);
    fwrite(&CPU::cs,2,1,fp);
    fwrite(&CPU::ds,2,1,fp);
    fwrite(&CPU::es,2,1,fp);
    fwrite(&CPU::ss,2,1,fp);
    fwrite(&CPU::flags,2,1,fp);
    fclose(fp);
    fp = fopen("save/mda.dump","wb");
    fwrite(&MDA::hdisp,2,1,fp);
    fwrite(&MDA::vdisp,2,1,fp);
    fwrite(&MDA::maxscan,2,1,fp);
    fwrite(&MDA::dispmode,2,1,fp);
}

int main(int ac, char** av)
{
    PIC::pic[0].init1 = false;
    PIC::pic[0].init2 = false;
    PIC::pic[0].enabled = false;
    if(ac < 4) return 1;
    FILE* bios = fopen(av[1],"rb");
    fseek(bios,0,SEEK_END);
    long size = ftell(bios);
    fseek(bios,0,SEEK_SET);
    fread(RAM::RAM + (0x100000 - size),1,size,bios);
    FILE* mda = fopen(av[2],"rb");
    fread(MDA::ROM,1,0x2000,mda);
    fseek(mda,0,SEEK_SET);
    fread(CGA::ROM,1,0x2000,mda);
    fclose(mda);
    fclose(bios);
    
    char* isa1 = new char[10];
    
    FILE* config = fopen(av[3],"r");
    fscanf(config,"isa1=%s\n",isa1);
    fclose(config);
    
    std::string isa1slot = isa1;
    delete[] isa1;
    
    SDL_Init(SDL_INIT_EVERYTHING);

    screen = SDL_SetVideoMode(720,350,24,SDL_SWSURFACE);
    
    if(isa1slot == "mda")
    {
        IO_XT::handlers.push_back(MDA::mdacrtc);
        SDL_WM_SetCaption("IBM5150:  CPU: 8086 SYSTEM: IBM PC 5150 ISA1: MDA",NULL);
    }
    if(isa1slot == "cga")
    {
        IO_XT::handlers.push_back(CGA::cgacrtc);
        SDL_WM_SetCaption("IBM5150:  CPU: 8086 SYSTEM: IBM PC 5150 ISA1: CGA",NULL);
    }

    IO_XT::handlers.push_back(DMA_XT::handler);
    IO_XT::handlers.push_back(PPI::handler);
    IO_XT::handlers.push_back(PIT::pit);
    IO_XT::handlers.push_back(PIC::pic1);

    bool quit = false;
    int i = 0;
    
    FILE* fp = fopen("save/mem.dump","rb");
    if(fp != NULL)
    {
        fread(RAM::RAM,0x100000,1,fp);
        fclose(fp);
    }
    fp = fopen("save/reg.dump","rb");
    if(fp != NULL)
    {
        fread(&CPU::ax,2,1,fp);
        fread(&CPU::bx,2,1,fp);
        fread(&CPU::cx,2,1,fp);
        fread(&CPU::dx,2,1,fp);
        fread(&CPU::si,2,1,fp);
        fread(&CPU::di,2,1,fp);
        fread(&CPU::sp,2,1,fp);
        fread(&CPU::bp,2,1,fp);
        fread(&CPU::ip,2,1,fp);
        fread(&CPU::cs,2,1,fp);
        fread(&CPU::ds,2,1,fp);
        fread(&CPU::es,2,1,fp);
        fread(&CPU::ss,2,1,fp);
        fread(&CPU::flags,2,1,fp);
        fclose(fp);
    }
    
    while(quit == false)
    {
        CPU::tick();
        if(i==5)
        {
            i=0;
            PIT::tick();
            if(isa1slot == "mda") MDA::tick_frame();
            if(isa1slot == "cga") CGA::tick_frame();
            SDL_Flip(screen);
        }

        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT) quit = true;
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
        
        i++;
    }

    SDL_Quit();
    return 0;
}
