#include "interface.h"
#include "cga.h"

namespace CGA
{
u8 ROM[0x2000];
u8 crtcindex = 0;
u8 htotal = 0;
u8 hdisp = 0;
u8 hsyncp = 0;
u8 hsyncw = 0;
u8 vtotal = 0;
u8 vtotala = 0;
u8 vdisp = 0;
u8 vsyncp = 0;
u8 maxscan = 1;
u8 curstart = 0;
u8 curend = 0;
u16 startaddr = 0;
u16 curaddr = 0;

int framecount = 0;

u8 dispmode = 2; //Bit 1 must be set at all times, unless the processor wants to be halted.
u8 status = 0x80;

u8 status_r(u16 addr)
{
    return status;
}

void putpix(int x, int y, u8 r, u8 g, u8 b)
{
    u8* p = (u8*)INTERFACE::screen->pixels;
    p[(((y*INTERFACE::screen->w)+x)*3)] = b;
    p[(((y*INTERFACE::screen->w)+x)*3)+1] = g;
    p[(((y*INTERFACE::screen->w)+x)*3)+2] = r;
}

void tick_frame()
{
    for(int y = 0; y<(vdisp); y++)
    {
        for(int x = 0; x<(hdisp+1); x++)
        {
            u8 chr = RAM::RAM[0xB8000 + (x + (y*(hdisp+1))<<1)];
            u8 attr = RAM::RAM[0xB8001 + (x + (y*(hdisp+1))<<1)];
            u8 fg[3], bg[3];
            bool blink = false;
            bool underline = false;
            switch(attr)
            {
            case 0x00:
            case 0x08:
            case 0x80:
            case 0x88:
            {
                for(int i = 0; i<3; i++)
                {
                    fg[i] = 0;
                    bg[i] = 0;
                }
                break;
            }
            case 0x70:
            {
                for(int i = 0; i<3; i++)
                {
                    fg[i] = 0;
                    bg[i] = 192;
                }
                break;
            }
            case 0x78:
            {
                for(int i = 0; i<3; i++)
                {
                    fg[i] = 128;
                    bg[i] = 192;
                }
                break;
            }
            case 0xF0:
            {
                for(int i = 0; i<3; i++)
                {
                    fg[i] = 128;
                    bg[i] = 192;
                }
                blink = true;
                break;
            }
            case 0xF8:
            {
                for(int i = 0; i<3; i++)
                {
                    fg[i] = 128;
                    bg[i] = 192;
                }
                blink = true;
                break;
            }
            default:
            {
                for(int i = 0; i<3; i++)
                {
                    fg[i] = 128;
                    bg[i] = 0;
                }
                if(attr & 2) underline = true;
                if(attr & 8)
                {
                    fg[0] = fg[1] = fg[2] = 192;
                }
                if(attr & 0x80) blink = true;
            }
            }
            for(int iy = 0; iy<(maxscan+1); iy++)
            {
                if(iy==maxscan && underline)
                {
                    for(int i = 0; i<9; i++)
                    {
                        putpix((x*9)+i,(y*(maxscan+1))+iy,192,192,192);
                    }
                }
                int tmp = iy % (maxscan+1);
                u8 chrdata;
                if(tmp & 8) chrdata = ROM[(0x1800 | (tmp & 7)) + (chr*8)];
                else chrdata = ROM[0x1000 + (chr*8) + tmp];
                if(chrdata & 0x80) putpix(x*9,(y*(maxscan+1))+iy,fg[0],fg[1],fg[2]);
                if(blink && (framecount & 0x10)) putpix(x*9,(y*(maxscan+1))+iy,bg[2],bg[2],bg[2]);
                if(chrdata & 0x40) putpix((x*9)+1,(y*(maxscan+1))+iy,fg[2],fg[2],fg[2]);
                if(blink && (framecount & 0x10)) putpix((x*9)+1,(y*(maxscan+1))+iy,bg[2],bg[2],bg[2]);
                if(chrdata & 0x20) putpix((x*9)+2,(y*(maxscan+1))+iy,fg[2],fg[2],fg[2]);
                if(blink && (framecount & 0x10)) putpix((x*9)+2,(y*(maxscan+1))+iy,bg[2],bg[2],bg[2]);
                if(chrdata & 0x10) putpix((x*9)+3,(y*(maxscan+1))+iy,fg[2],fg[2],fg[2]);
                if(blink && (framecount & 0x10)) putpix((x*9)+3,(y*(maxscan+1))+iy,bg[2],bg[2],bg[2]);
                if(chrdata & 0x08) putpix((x*9)+4,(y*(maxscan+1))+iy,fg[2],fg[2],fg[2]);
                if(blink && (framecount & 0x10)) putpix((x*9)+4,(y*(maxscan+1))+iy,bg[2],bg[2],bg[2]);
                if(chrdata & 0x04) putpix((x*9)+5,(y*(maxscan+1))+iy,fg[2],fg[2],fg[2]);
                if(blink && (framecount & 0x10)) putpix((x*9)+5,(y*(maxscan+1))+iy,bg[2],bg[2],bg[2]);
                if(chrdata & 0x02) putpix((x*9)+6,(y*(maxscan+1))+iy,fg[2],fg[2],fg[2]);
                if(blink && (framecount & 0x10)) putpix((x*9)+6,(y*(maxscan+1))+iy,bg[2],bg[2],bg[2]);
                if(chrdata & 0x01) putpix((x*9)+7,(y*(maxscan+1))+iy,fg[2],fg[2],fg[2]);
                if(blink && (framecount & 0x10)) putpix((x*9)+7,(y*(maxscan+1))+iy,bg[2],bg[2],bg[2]);
                putpix((x*9)+8,(y*(maxscan+1))+iy,0,0,0);
            }
        }
    }
    framecount++;
    if(framecount == 0x1F) framecount = 0;
}
void cga_w(u16 addr, u8 value)
{
    switch(addr)
    {
    case 4:
        crtcindex = value;
        break;
    case 5:
    {
        switch(crtcindex)
        {
        case 0:
        {
            htotal = value;
            break;
        }
        case 1:
        {
            hdisp = value;
            break;
        }
        case 2:
        {
            hsyncp = value;
            break;
        }
        case 3:
        {
            hsyncw = value;
            break;
        }
        case 4:
        {
            vtotal = value;
            break;
        }
        case 5:
        {
            vtotala = value;
            break;
        }
        case 6:
        {
            vdisp = value;
            break;
        }
        case 7:
        {
            vsyncp = value;
            break;
        }
        case 9:
        {
            maxscan = value;
            break;
        }
        case 0x0A:
        {
            curstart = value;
            break;
        }
        case 0x0B:
        {
            curend = value;
            break;
        }
        case 0x0C:
        {
            startaddr = (startaddr & 0x00FF) | (value << 8);
            break;
        }
        case 0x0D:
        {
            startaddr = (startaddr & 0xFF00) | value;
            break;
        }
        case 0x0E:
        {
            curaddr = (curaddr & 0x00FF) | (value << 8);
            break;
        }
        case 0x0F:
        {
            curaddr = (curaddr & 0xFF00) | value;
            break;
        }
        }
        break;
    }
    case 8:
    {
        dispmode = value;
        break;
    }
    }
}

iohandler cgacrtc = {0x03D0, 0x03D8, NULL, cga_w};
}
