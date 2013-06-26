#include "cpu.h"

namespace CPU {

u16 ds,es,ss;
u16 sp,bp,si,di;
u16 tr;
u16 cs = 0xF000;
u16 ip = 0xFFF0;
u16 flags = 0xF002;

int type = intel8086;

locs decodemodrm(int seg, u8 modrm, bool word, bool segarg)
{
    locs res;
    int tmp = word | (segarg<<1);
    switch(modrm & 0xC7)
    {
    case 0x00:
    {
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bx+si)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bx+si)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bx+si)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bx+si)];
            break;
        }
        }
        break;
    }
    case 0x01:
    {
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bx+di)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bx+di)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bx+di)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bx+di)];
            break;
        }
        }
        break;
    }
    case 0x02:
    {
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+si)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bp+si)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bp+si)];
            break;
        }
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bp+si)];
            break;
        }
        }
        break;
    }
    case 0x03:
    {
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+di)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bp+di)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bp+di)];
            break;
        }
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bp+di)];
            break;
        }
        }
        break;
    }
    case 0x04:
    {
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,si)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,si)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,si)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,si)];
            break;
        }
        }
        break;
    }
    case 0x05:
    {
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,di)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,di)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,di)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,di)];
            break;
        }
        }
        break;
    }
    case 0x06:
    {
        u16 tmp1 = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,tmp1)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,tmp1)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,tmp1)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,tmp1)];
            break;
        }
        }
        break;
    }
    case 0x07:
    {
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bx)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bx)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bx)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bx)];
            break;
        }
        }
        break;
    }
    case 0x40:
    {
        u16 tmp1 = RAM::rb(cs,ip+1);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bx+si+tmp1)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bx+si+tmp1)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bx+si+tmp1)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bx+si+tmp1)];
            break;
        }
        }
        break;
    }
    case 0x41:
    {
        u16 tmp1 = RAM::rb(cs,ip+1);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bx+di)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bx+di)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bx+di)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bx+di)];
            break;
        }
        }
        break;
    }
    case 0x42:
    {
        u16 tmp1 = RAM::rb(cs,ip+1);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+si)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bp+si)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bp+si)];
            break;
        }
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bp+si)];
            break;
        }
        }
        break;
    }
    case 0x43:
    {
        u16 tmp1 = RAM::rb(cs,ip+1);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+di)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bp+di)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bp+di)];
            break;
        }
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bp+di)];
            break;
        }
        }
        break;
    }
    case 0x44:
    {
        u16 tmp1 = RAM::rb(cs,ip+1);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,si)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,si)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,si)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,si)];
            break;
        }
        }
        break;
    }
    case 0x45:
    {
        u16 tmp1 = RAM::rb(cs,ip+1);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,di)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,di)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,di)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,di)];
            break;
        }
        }
        break;
    }
    case 0x46:
    {
        u16 tmp1 = RAM::rb(cs,ip+1);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+tmp1)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bp+tmp1)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bp+tmp1)];
            break;
        }
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+tmp1)];
            break;
        }
        }
        break;
    }
    case 0x47:
    {
        u16 tmp1 = RAM::rb(cs,ip+1);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bx+tmp1)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bx+tmp1)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bx+tmp1)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bx+tmp1)];
            break;
        }
        }
        break;
    }
    case 0x80:
    {
        u16 tmp1 = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bx+si+tmp1)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bx+si+tmp1)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bx+si+tmp1)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bx+si+tmp1)];
            break;
        }
        }
        break;
    }
    case 0x81:
    {
        u16 tmp1 = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bx+di)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bx+di)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bx+di)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bx+di)];
            break;
        }
        }
        break;
    }
    case 0x82:
    {
        u16 tmp1 = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+si)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bp+si)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bp+si)];
            break;
        }
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bp+si)];
            break;
        }
        }
        break;
    }
    case 0x83:
    {
        u16 tmp1 = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+di)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bp+di)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bp+di)];
            break;
        }
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bp+di)];
            break;
        }
        }
        break;
    }
    case 0x84:
    {
        u16 tmp1 = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,si)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,si)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,si)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,si)];
            break;
        }
        }
        break;
    }
    case 0x85:
    {
        u16 tmp1 = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,di)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,di)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,di)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,di)];
            break;
        }
        }
        break;
    }
    case 0x86:
    {
        u16 tmp1 = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+tmp1)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bp+tmp1)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bp+tmp1)];
            break;
        }
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bp+tmp1)];
            break;
        }
        }
        break;
    }
    case 0x87:
    {
        u16 tmp1 = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
        switch(seg)
        {
        case SEG_DEFAULT:
        case SEG_DS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ds,bx+tmp1)];
            break;
        }
        case SEG_CS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(cs,bx+tmp1)];
            break;
        }
        case SEG_ES:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(es,bx+tmp1)];
            break;
        }
        case SEG_SS:
        {
            res.src8 = &RAM::RAM[RAM::getaddr(ss,bx+tmp1)];
            break;
        }
        }
        break;
    }
    case 0xC0:
    {
        switch(tmp&1)
        {
        case 0:
        {
            res.src8 = &al;
            break;
        }
        case 1:
        {
            res.src16 = &ax;
            break;
        }
        }
        break;
    }
    case 0xC1:
    {
        switch(tmp&1)
        {
        case 0:
        {
            res.src8 = &cl;
            break;
        }
        case 1:
        {
            res.src16 = &cx;
            break;
        }
        }
        break;
    }
    case 0xC2:
    {
        switch(tmp&1)
        {
        case 0:
        {
            res.src8 = &dl;
            break;
        }
        case 1:
        {
            res.src16 = &dx;
            break;
        }
        }
        break;
    }
    case 0xC3:
    {
        switch(tmp&1)
        {
        case 0:
        {
            res.src8 = &bl;
            break;
        }
        case 1:
        {
            res.src16 = &bx;
            break;
        }
        }
        break;
    }
    case 0xC4:
    {
        switch(tmp&1)
        {
        case 0:
        {
            res.src8 = &ah;
            break;
        }
        case 1:
        {
            res.src16 = &sp;
            break;
        }
        }
        break;
    }
    case 0xC5:
    {
        switch(tmp&1)
        {
        case 0:
        {
            res.src8 = &ch;
            break;
        }
        case 1:
        {
            res.src16 = &bp;
            break;
        }
        }
        break;
    }
    case 0xC6:
    {
        switch(tmp&1)
        {
        case 0:
        {
            res.src8 = &dh;
            break;
        }
        case 1:
        {
            res.src16 = &si;
            break;
        }
        }
        break;
    }
    case 0xC7:
    {
        switch(tmp&1)
        {
        case 0:
        {
            res.src8 = &bh;
            break;
        }
        case 1:
        {
            res.src16 = &di;
            break;
        }
        }
        break;
    }
    }
    switch(modrm & 0x38)
    {
    case 0x00:
    {
        switch(tmp)
        {
        case 0:
        {
            res.dst8 = &al;
            break;
        }
        case 1:
        {
            res.dst16 = &ax;
            break;
        }
        case 3:
        {
            res.dst16 = &es;
            break;
        }
        }
        break;
    }
    case 0x08:
    {
        switch(tmp)
        {
        case 0:
        {
            res.dst8 = &cl;
            break;
        }
        case 1:
        {
            res.dst16 = &cx;
            break;
        }
        case 3:
        {
            res.dst16 = &cs;
            break;
        }
        }
        break;
    }
    case 0x10:
    {
        switch(tmp)
        {
        case 0:
        {
            res.dst8 = &dl;
            break;
        }
        case 1:
        {
            res.dst16 = &dx;
            break;
        }
        case 3:
        {
            res.dst16 = &ss;
            break;
        }
        }
        break;
    }
    case 0x18:
    {
        switch(tmp)
        {
        case 0:
        {
            res.dst8 = &bl;
            break;
        }
        case 1:
        {

            res.dst16 = &bx;
            break;
        }
        case 3:
        {
            res.dst16 = &ds;
            break;
        }
        }
        break;
    }
    case 0x20:
    {
        switch(tmp)
        {
        case 0:
        {
            res.dst8 = &ah;
            break;
        }
        case 1:
        {
            res.dst16 = &sp;
            break;
        }
        }
        break;
    }
    case 0x28:
    {
        switch(tmp)
        {
        case 0:
        {
            res.dst8 = &ch;
            break;
        }
        case 1:
        {
            res.dst16 = &bp;
            break;
        }
        }
        break;
    }
    case 0x30:
    {
        switch(tmp)
        {
        case 0:
        {
            res.dst8 = &dh;
            break;
        }
        case 1:
        {
            res.dst16 = &si;
            break;
        }
        }
        break;
    }
    case 0x38:
    {
        switch(tmp)
        {
        case 0:
        {
            res.dst8 = &bh;
            break;
        }
        case 1:
        {
            res.dst16 = &di;
            break;
        }
        }
        break;
    }
    }
    if(modrm < 0xC0)
    {
        if(modrm >= 0x40 && modrm < 0x80) ip++;
        else if(modrm >= 0x80) ip+=2;
    }
    return res;
}

int seg = SEG_DEFAULT;
int rep = 0; //0 is no rep. 1 is repe. 2 is repne.
bool i8080 = false; //This is for the NEC V20.

void rtick()
{
    u8 op = RAM::rb(cs,ip);
    if(!i8080 && !halted)
    {
        switch(op)
        {
        case 0x00:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.src8 += *loc.dst8;
            u8 tmp = *loc.src8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            u8 v = 0;
            for(int i = 0; i<8; i++)
            {
                if(tmp & (1 << i)) v ^= 1;
            }
            if(!v) flags |= 0x0004;
            else flags &= 0xFFFB;
            ip+=2;
            printf("ADD Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x01:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.src16 += *loc.dst16;
            u16 tmp = *loc.src16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("ADD Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x02:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.dst8 += *loc.dst8;
            u8 tmp = *loc.dst8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("ADD Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x03:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.dst16 += *loc.src16;
            u16 tmp = *loc.dst16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("ADD Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x04:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            al += tmp;
            if(al == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=3;
            printf("ADD AL,%02x\n",al);
            break;
        }
        case 0x05:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            ax += tmp;
            if(ax == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("ADD AX,%04x\n",ax);
            break;
        }
        case 0x06:
        {
            printf("PUSH ES\n");
            sp -= 2;
            RAM::wb(ss,sp,es & 0xFF);
            RAM::wb(ss,sp+1,es >> 8);
            ip++;
            break;
        }
        case 0x07:
        {
            printf("POP ES\n");
            es = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x08:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.src8 |= *loc.dst8;
            u8 tmp = *loc.src8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("OR Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x09:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.src16 |= *loc.dst16;
            u16 tmp = *loc.src16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("OR Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x0A:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.dst8 |= *loc.src8;
            u8 tmp = *loc.dst8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("OR Gb,Eb modrm=%02x\n",modrm);
            break;
        }
        case 0x0B:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.dst16 |= *loc.src16;
            u16 tmp = *loc.dst16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("OR Gv,Ev modrm=%02x\n",modrm);
            break;
        }
        case 0x0C:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            al |= tmp;
            if(al == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=3;
            printf("OR AL,%02x\n",al);
            break;
        }
        case 0x0D:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            ax |= tmp;
            if(ax == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("OR AX,%04x\n",ax);
            break;
        }
        case 0x0E:
        {
            printf("PUSH CS\n");
            sp -= 2;
            RAM::wb(ss,sp,cs & 0xFF);
            RAM::wb(ss,sp+1,cs >> 8);
            ip++;
            break;
        }
        case 0x0F:
        {
            if(type == intel8086)
            {
                printf("POP CS\n");
                cs = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
                sp+=2;
                ip++;
            }
            else if(type == necv20)
            {
                u8 op2 = RAM::rb(cs,ip+1);
                switch(op2)
                {
                case 0x31:
                {
                    u8 modrm = RAM::rb(cs,ip+3);
                    locs loc = decodemodrm(seg,modrm,false,false);
                    u8 tmp1 = *loc.src8 & 0x0F;
                    u8 tmp2 = 0;
                    for(int i = 0; i<tmp1; i++)
                    {
                        tmp2 |= (1<<i);
                    }
                    u8 tmp3 = tmp2 >> (*loc.dst8 & 0x0F);
                    u8 tmp4 = tmp2 << (8 - (*loc.dst8 & 0x0F));
                    RAM::wb(es,di,ax&tmp3);
                    if(tmp4 != 0)
                    {
                        RAM::wb(es,di+1,ax&tmp4);
                        di++;
                    }
                    di++;
                    ip+=3;
                    printf("INS Gb,Eb\n");
                    break;
                }
                case 0x39:
                {
                    u8 modrm = RAM::rb(cs,ip+3);
                    locs loc = decodemodrm(seg,modrm,false,false);
                    u8 tmp = RAM::rb(cs,ip+2);
                    u8 tmp1 = tmp & 0x0F;
                    u8 tmp2 = 0;
                    for(int i = 0; i<tmp1; i++)
                    {
                        tmp2 |= (1<<i);
                    }
                    u8 tmp3 = tmp2 >> (*loc.dst8 & 0x0F);
                    u8 tmp4 = tmp2 << (8 - (*loc.dst8 & 0x0F));
                    RAM::wb(es,di,ax&tmp3);
                    if(tmp4 != 0)
                    {
                        RAM::wb(es,di+1,ax&tmp4);
                        di++;
                    }
                    di++;
                    ip+=3;
                    printf("INS Gb,%02x\n",tmp);
                    break;
                }
                case 0xFF:
                {
                    i8080 = true;
                    u8 tmp = RAM::rb(cs,ip+2);
                    sp-=2;
                    RAM::wb(ss,sp,flags & 0xFF);
                    RAM::wb(ss,sp+1,flags >> 8);
                    flags &= 0xFCFF;
                    sp-=2;
                    RAM::wb(ss,sp,cs & 0xFF);
                    RAM::wb(ss,sp+1,cs >> 8);
                    sp-=2;
                    RAM::wb(ss,sp,(ip+1) & 0xFF);
                    RAM::wb(ss,sp+1,(ip+1) >> 8);
                    cs = RAM::rb(0,(tmp<<2)+3)|(RAM::rb(0,(tmp<<2)+2)<<8);
                    ip = RAM::rb(0,tmp<<2)|(RAM::rb(0,(tmp<<2)+1)<<8);
                    printf("BRKEM\n");
                    break;
                }
                }
            }
            else if(type>=intel286)
            {
                u8 op2 = RAM::rb(cs,ip+1);
                switch(op2)
                {
                case 0x02:
                {
                    u8 modrm = RAM::rb(cs,ip+1);
                    locs loc = decodemodrm(seg,modrm,true,false);
                    *loc.dst16 = *loc.src16 & 0xFF00;
                    flags |= 0x0040;
                    printf("LAR Ev,Gv\n");
                    break;
                }
                case 0x05:
                {
                    /*if(type==intel286)
                    {
                    cr0 = (cr0 & 0xFFFF0000) | (RAM::RAM[0x806]|(RAM::RAM[0x807]<<8));
                    tr = RAM::RAM[0x816]|(RAM::RAM[0x817]<<8);
                    flags = RAM::RAM[0x818]|(RAM::RAM[0x819]<<8);
                    ip = RAM::RAM[0x81A]|(RAM::RAM[0x81B]<<8);
                    ldtr = RAM::RAM[0x81C]|(RAM::RAM[0x81D]<<8);
                    ds = RAM::RAM[0x81E]|(RAM::RAM[0x81F]<<8);
                    ss = RAM::RAM[0x820]|(RAM::RAM[0x821]<<8);
                    cs = RAM::RAM[0x822]|(RAM::RAM[0x823]<<8);
                    es = RAM::RAM[0x824]|(RAM::RAM[0x825]<<8);
                    printf("LOADALL\n");*/
                    break;
                }
                case 0x06:
                {
                    cr0 &= 0xFFFFFFF7;
                    printf("CLTS\n");
                    break;
                }
                }
            }
            break;
        }
        case 0x10:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.src8 += (*loc.dst8) + (flags & 1);
            u8 tmp = *loc.src8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("ADC Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x11:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.src16 += (*loc.dst16) + (flags & 1);
            u16 tmp = *loc.src16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("ADC Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x12:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.dst8 += (*loc.src8) + (flags & 1);
            u8 tmp = *loc.dst8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("ADC Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x13:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.dst16 += (*loc.src16) + (flags & 1);
            u16 tmp = *loc.dst16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("ADC Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x14:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            al += tmp + (flags & 1);
            if(al == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=3;
            printf("ADC AL,%02x\n",al);
            break;
        }
        case 0x15:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            ax += tmp + (flags & 1);
            if(ax == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("ADC AX,%04x\n",ax);
            break;
        }
        case 0x16:
        {
            printf("PUSH SS\n");
            sp -= 2;
            RAM::wb(ss,sp,ss & 0xFF);
            RAM::wb(ss,sp+1,ss >> 8);
            ip++;
            break;
        }
        case 0x17:
        {
            printf("POP SS\n");
            ss = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x18:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.src8 -= (*loc.dst8) + (flags & 1);
            u8 tmp = *loc.src8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SBB Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x19:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.src16 -= (*loc.dst16) + (flags & 1);
            u16 tmp = *loc.src16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SBB Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x1A:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.dst8 -= (*loc.src8) + (flags & 1);
            u8 tmp = *loc.dst8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SBB Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x1B:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.dst16 -= (*loc.src16) + (flags & 1);
            u16 tmp = *loc.dst16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SBB Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x1C:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            al -= tmp + (flags & 1);
            if(al == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=3;
            printf("SBB AL,%02x\n",al);
            break;
        }
        case 0x1D:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            ax -= tmp + (flags & 1);
            if(ax == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SBB AX,%04x\n",ax);
            break;
        }
        case 0x1E:
        {
            printf("PUSH DS\n");
            sp -= 2;
            RAM::wb(ss,sp,ds & 0xFF);
            RAM::wb(ss,sp+1,ds >> 8);
            ip++;
            break;
        }
        case 0x1F:
        {
            printf("POP DS\n");
            ds = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x20:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.dst8 &= *loc.src8;
            u8 tmp = *loc.dst8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("AND Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x21:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.src16 &= *loc.dst16;
            u16 tmp = *loc.src16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("AND Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x22:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.src8 &= *loc.dst8;
            u8 tmp = *loc.src8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("AND Gb,Eb modrm=%02x\n",modrm);
            break;
        }
        case 0x23:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.dst16 &= *loc.src16;
            u16 tmp = *loc.dst16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("AND Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x24:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            al &= tmp;
            if(al == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=3;
            printf("AND AL,%02x\n",al);
            break;
        }
        case 0x25:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            ax &= tmp;
            if(ax == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("AND AX,%04x\n",ax);
            break;
        }
        case 0x27:
        {
            if(((al & 0x0F) > 9) || (flags & 0x0010))
            {
                al += 6;
                flags |= 0x0010;
            }
            else flags &= 0xFFEF;
            if((al > 0x9F) || (flags & 0x0001))
            {
                al += 0x60;
                flags |= 0x0001;
            }
            else flags &= 0xFFFE;
            printf("DAA\n");
            ip++;
            break;
        }
        case 0x28:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.src8 -= *loc.dst8;
            u8 tmp = *loc.src8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SUB Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x29:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.src16 -= *loc.dst16;
            u16 tmp = *loc.src16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SUB Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x2A:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.dst8 -= *loc.src8;
            u8 tmp = *loc.dst8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SUB Gb,Eb modrm=%02x\n",modrm);
            break;
        }
        case 0x2B:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.dst16 -= *loc.src16;
            u16 tmp = *loc.dst16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SUB Gv,Ev modrm=%02x\n",modrm);
            break;
        }
        case 0x2C:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            al -= tmp;
            if(al == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("SUB AL,%02x\n",al);
            break;
        }
        case 0x2D:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            ax -= tmp;
            if(ax == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=3;
            printf("SUB AX,%04x\n",ax);
            break;
        }
        case 0x2F:
        {
            if(((al & 0x0F) > 9) || (flags & 0x0010))
            {
                al -= 6;
                flags |= 0x0010;
            }
            else flags &= 0xFFEF;
            if((al > 0x9F) || (flags & 0x0001))
            {
                al -= 0x60;
                flags |= 0x0001;
            }
            else flags &= 0xFFFE;
            printf("DAS\n");
            ip++;
            break;
        }
        case 0x30:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.src8 ^= *loc.dst8;
            u8 tmp = *loc.src8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("XOR Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x31:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.src16 ^= *loc.dst16;
            u16 tmp = *loc.src16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("XOR Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x32:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.dst8 ^= *loc.src8;
            u8 tmp = *loc.dst8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("XOR Gb,Eb modrm=%02x\n",modrm);
            break;
        }
        case 0x33:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.dst16 ^= *loc.src16;
            u16 tmp = *loc.dst16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            u16 v = 0;
            for(int i = 0; i<16; i++)
            {
                if(tmp & (1 << i)) v ^= 1;
            }
            if(!v) flags |= 0x0004;
            else flags &= 0xFFFB;
            ip+=2;
            printf("XOR Gv,Ev modrm=%02x\n",modrm);
            break;
        }
        case 0x34:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            al ^= tmp;
            if(al == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("XOR AL,%02x\n",al);
            break;
        }
        case 0x35:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            ax ^= tmp;
            if(ax == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=3;
            printf("XOR AX,%04x\n",ax);
            break;
        }
        case 0x37:
        {
            if(((al & 0x0F) > 9) || (flags & 0x0010))
            {
                al = (al + 6) & 0x0F;
                ah++;
                flags |= 0x0011;
            }
            else flags &= 0xFFEE;
            printf("AAA\n");
            ip++;
            break;
        }
        case 0x38:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            u8 tmp = *loc.src8 - *loc.dst8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("CMP Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x39:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            u16 tmp = *loc.src16 - *loc.dst16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("CMP Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x3A:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            u8 tmp = *loc.dst8 - *loc.src8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("CMP Gb,Eb modrm=%02x\n",modrm);
            break;
        }
        case 0x3B:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            u16 tmp = *loc.dst16 - *loc.src16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("CMP Gv,Ev modrm=%02x\n",modrm);
            break;
        }
        case 0x3C:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            u8 tmp1 = al - tmp;
            if(tmp1 == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("CMP AL,%02x\n",tmp);
            break;
        }
        case 0x3D:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            u16 tmp1 = ax - tmp;
            if(tmp1 == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=3;
            printf("CMP AX,%04x\n",tmp);
            break;
        }
        case 0x3F:
        {
            if(((al & 0x0F) > 9) || (flags & 0x0010))
            {
                al = (al - 6) & 0x0F;
                ah--;
                flags |= 0x0011;
            }
            else flags &= 0xFFEE;
            printf("AAS\n");
            ip++;
            break;
        }
        case 0x40:
        {
            u16 tmp = ax;
            printf("INC AX\n");
            ax++;
            if(ax == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            if(ax >= 0x8000) flags |= 0x0800;
            else flags &= 0xF7FF;
            ip++;
            break;
        }
        case 0x41:
        {
            printf("INC CX\n");
            cx++;
            if(cx == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x42:
        {
            printf("INC DX\n");
            dx++;
            if(dx == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x43:
        {
            printf("INC BX\n");
            bx++;
            if(bx == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x44:
        {
            printf("INC SP\n");
            sp++;
            if(sp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x45:
        {
            printf("INC BP\n");
            bp++;
            if(bp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x46:
        {
            printf("INC SI\n");
            si++;
            if(si == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x47:
        {
            printf("INC DI\n");
            di++;
            if(di == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x48:
        {
            printf("DEC AX\n");
            ax--;
            if(ax == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x49:
        {
            printf("DEC CX\n");
            cx--;
            if(cx == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x4A:
        {
            printf("DEC DX\n");
            dx--;
            if(dx == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x4B:
        {
            printf("DEC CX\n");
            cx--;
            if(cx == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x4C:
        {
            printf("DEC SP\n");
            sp--;
            if(sp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x4D:
        {
            printf("DEC BP\n");
            bp--;
            if(bp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x4E:
        {
            printf("DEC SI\n");
            si--;
            if(si == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x4F:
        {
            printf("DEC DI\n");
            di--;
            if(di == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip++;
            break;
        }
        case 0x50:
        {
            printf("PUSH AX\n");
            sp -= 2;
            RAM::wb(ss,sp,al);
            RAM::wb(ss,sp+1,ah);
            ip++;
            break;
        }
        case 0x51:
        {
            printf("PUSH CX\n");
            sp -= 2;
            RAM::wb(ss,sp,cl);
            RAM::wb(ss,sp+1,ch);
            ip++;
            break;
        }
        case 0x52:
        {
            printf("PUSH DX\n");
            sp -= 2;
            RAM::wb(ss,sp,dl);
            RAM::wb(ss,sp+1,dh);
            ip++;
            break;
        }
        case 0x53:
        {
            printf("PUSH BX\n");
            sp -= 2;
            RAM::wb(ss,sp,bl);
            RAM::wb(ss,sp+1,bh);
            ip++;
            break;
        }
        case 0x54:
        {
            if(type == intel8086)
            {
                printf("PUSH SP\n");
                sp -= 2;
                RAM::wb(ss,sp,sp & 0xFF);
                RAM::wb(ss,sp+1,sp >> 8);
                ip++;
            }
            break;
        }
        case 0x55:
        {
            printf("PUSH BP\n");
            sp -= 2;
            RAM::wb(ss,sp,bp & 0xFF);
            RAM::wb(ss,sp+1,bp >> 8);
            ip++;
            break;
        }
        case 0x56:
        {
            printf("PUSH SI\n");
            sp -= 2;
            RAM::wb(ss,sp,si & 0xFF);
            RAM::wb(ss,sp+1,si >> 8);
            ip++;
            break;
        }
        case 0x57:
        {
            printf("PUSH DI\n");
            sp -= 2;
            RAM::wb(ss,sp,di & 0xFF);
            RAM::wb(ss,sp+1,di >> 8);
            ip++;
            break;
        }
        case 0x58:
        {
            printf("POP AX\n");
            ax = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x59:
        {
            printf("POP CX\n");
            cx = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x5A:
        {
            printf("POP DX\n");
            dx = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x5B:
        {
            printf("POP BX\n");
            bx = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x5C:
        {
            printf("POP SP\n");
            sp = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x5D:
        {
            printf("POP BP\n");
            bp = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x5E:
        {
            printf("POP SI\n");
            si = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        case 0x5F:
        {
            printf("POP DI\n");
            di = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
            sp+=2;
            ip++;
            break;
        }
        //Opcodes 0x60-0x6F are just aliases of 0x70-0x7F on the 8086. Confirmed by hardware tests.
        case 0x60:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JO %02x\n",tmp);
                if((flags&0x0800)) ip += (s8)tmp;
                ip+=2;
            }
            else
            {
                sp -= 2;
                RAM::wb(ss,sp,al);
                RAM::wb(ss,sp+1,ah);
                sp -= 2;
                RAM::wb(ss,sp,cl);
                RAM::wb(ss,sp+1,ch);
                sp -= 2;
                RAM::wb(ss,sp,dl);
                RAM::wb(ss,sp+1,dh);
                sp -= 2;
                RAM::wb(ss,sp,bl);
                RAM::wb(ss,sp+1,bh);
                u16 tmp = sp;
                sp -= 2;
                RAM::wb(ss,sp,tmp & 0xFF);
                RAM::wb(ss,sp+1,tmp >> 8);
                sp -= 2;
                RAM::wb(ss,sp,bp & 0xFF);
                RAM::wb(ss,sp+1,bp >> 8);
                sp -= 2;
                RAM::wb(ss,sp,si & 0xFF);
                RAM::wb(ss,sp+1,si >> 8);
                sp -= 2;
                RAM::wb(ss,sp,di & 0xFF);
                RAM::wb(ss,sp+1,di >> 8);
                printf("PUSHA\n");
                ip++;
            }
            break;
        }
        case 0x61:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JNO %02x\n",tmp);
                if(!(flags&0x0800)) ip += (s8)tmp;
                ip+=2;
            }
            else
            {
                di = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
                sp+=2;
                si = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
                sp+=2;
                bp = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
                sp+=4;
                bx = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
                sp+=2;
                dx = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
                sp+=2;
                cx = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
                sp+=2;
                ax = RAM::rb(ss,sp) | (RAM::rb(ss,sp+1)<<8);
                sp+=2;
                printf("POPA\n");
                ip++;
            }
            break;
        }
        case 0x62:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JC %02x\n",tmp);
                if((flags&0x0001)) ip += (s8)tmp;
                ip+=2;
            }
            else
            {
                u8 modrm = RAM::rb(cs,ip+1);
                u16 tmp = RAM::rb(cs,ip+2)|(RAM::rb(cs,ip+3)<<8);
                u16 tmp1 = RAM::rb(cs,ip+4)|(RAM::rb(cs,ip+5)<<8);
                locs loc = decodemodrm(seg,modrm,true,false);
                if((*loc.dst16) < (*loc.src16) || (*loc.dst16) > (*(loc.src16 + 2)))
                {
                    sp-=2;
                    RAM::wb(ss,sp,flags & 0xFF);
                    RAM::wb(ss,sp+1,flags >> 8);
                    flags &= 0xFCFF;
                    sp-=2;
                    RAM::wb(ss,sp,cs & 0xFF);
                    RAM::wb(ss,sp+1,cs >> 8);
                    sp-=2;
                    RAM::wb(ss,sp,(ip+1) & 0xFF);
                    RAM::wb(ss,sp+1,(ip+1) >> 8);
                    cs = RAM::rb(0,23)|(RAM::rb(0,22)<<8);
                    ip = RAM::rb(0,20)|(RAM::rb(0,21)<<8);
                }
            }
            break;
        }
        case 0x63:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JNC %02x\n",tmp);
                if(!(flags&0x0001)) ip += (s8)tmp;
                ip+=2;
            }
            else if(type>=intel286)
            {
                u8 modrm = RAM::rb(cs,ip+1);
                locs loc = decodemodrm(seg,modrm,true,false);
                if((*loc.dst16 & 3) < (*loc.src16 & 3))
                {
                    flags|=0x0040;
                    *loc.dst16 = (*loc.dst16 & 0xFC) | (*loc.src16 & 3);
                }
                else flags&=0xFFBF;
                printf("ARPL Gv,Ev\n");
                ip+=2;
            }
            break;
        }
        case 0x64:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JZ %02x\n",tmp);
                if((flags&0x0040)) ip += (s8)tmp;
                ip+=2;
            }
            break;
        }
        case 0x65:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JNZ %02x\n",tmp);
                if(!(flags&0x0040)) ip += (s8)tmp;
                ip+=2;
            }
            break;
        }
        case 0x66:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JBE %02x\n",tmp);
                if((flags&0x0040) || (flags&0x0001)) ip += (s8)tmp;
                ip+=2;
            }
            break;
        }
        case 0x67:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JA %02x\n",tmp);
                if(!(flags&0x0040) && !(flags&0x0001)) ip += (s8)tmp;
                ip+=2;
            }
            break;
        }
        case 0x68:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JS %02x\n",tmp);
                if((flags&0x0080)) ip += (s8)tmp;
                ip+=2;
            }
            break;
        }
        case 0x69:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JNS %02x\n",tmp);
                if(!(flags&0x0080)) ip += (s8)tmp;
                ip+=2;
            }
            break;
        }
        case 0x6A:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JP %02x\n",tmp);
                if((flags&0x0004)) ip += (s8)tmp;
                ip+=2;
            }
            break;
        }
        case 0x6B:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                printf("JNP %02x\n",tmp);
                if(!(flags&0x0004)) ip += (s8)tmp;
                ip+=2;
            }
            break;
        }
        case 0x6C:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                bool tmp1 = (flags >> 7) & 1;
                bool tmp2 = (flags >> 11) & 1;
                printf("JL %02x\n",tmp);
                if(tmp1 != tmp2) ip += (s8)tmp;
                ip+=2;
            }
            else
            {
                switch(rep)
                {
                case 0:
                {
                    RAM::wb(es,di,IO_XT::rb(dx));
                    if(!(flags & 0x0400)) di++;
                    else di--;
                    printf("INSB\n");
                    break;
                }
                default:
                {
                    for(; cx!=0; cx--)
                    {
                        RAM::wb(es,di,IO_XT::rb(dx));
                        if(!(flags & 0x0400)) di++;
                        else di--;
                    }
                    printf("REP INSB\n");
                    break;
                }
                break;
                }
                ip++;
            }
            break;
        }
        case 0x6D:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                bool tmp1 = (flags >> 7) & 1;
                bool tmp2 = (flags >> 11) & 1;
                printf("JGE %02x\n",tmp);
                if(tmp1 == tmp2) ip += (s8)tmp;
                ip+=2;
            }
            else
            {
                switch(rep)
                {
                case 0:
                {
                    RAM::wb(es,di,IO_XT::rb(dx));
                    RAM::wb(es,di+1,IO_XT::rb(dx));
                    if(!(flags & 0x0400)) di+=2;
                    else di-=2;
                    printf("INSW\n");
                    break;
                }
                default:
                {
                    for(; cx!=0; cx--)
                    {
                        RAM::wb(es,di,IO_XT::rb(dx));
                        RAM::wb(es,di+1,IO_XT::rb(dx));
                        if(!(flags & 0x0400)) di+=2;
                        else di-=2;
                    }
                    printf("REP INSW\n");
                    break;
                }
                break;
                }
                ip++;
            }
            break;
        }
        case 0x6E:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                bool tmp1 = (flags >> 7) & 1;
                bool tmp2 = (flags >> 11) & 1;
                printf("JLE %02x\n",tmp);
                if((tmp1 != tmp2) || (flags & 0x0040)) ip += (s8)tmp;
                ip+=2;
            }
            else
            {
                switch(rep)
                {
                case 0:
                {
                    IO_XT::wb(dx,RAM::rb(ds,si));
                    if(!(flags & 0x0400)) si++;
                    else si--;
                    printf("OUTSB\n");
                    break;
                }
                default:
                {
                    for(; cx!=0; cx--)
                    {
                        IO_XT::wb(dx,RAM::rb(ds,si));
                        if(!(flags & 0x0400)) si++;
                        else si--;
                    }
                    printf("REP OUTSB\n");
                    break;
                }
                break;
                }
                ip++;
            }
            break;
        }
        case 0x6F:
        {
            if(type==intel8086)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                bool tmp1 = (flags >> 7) & 1;
                bool tmp2 = (flags >> 11) & 1;
                printf("JG %02x\n",tmp);
                if((tmp1 == tmp2) && !(flags & 0x0040)) ip += (s8)tmp;
                ip+=2;
            }
            else
            {
                switch(rep)
                {
                case 0:
                {
                    IO_XT::wb(dx,RAM::rb(ds,si));
                    IO_XT::wb(dx+1,RAM::rb(ds,si+1));
                    if(!(flags & 0x0400)) si+=2;
                    else si-=2;
                    printf("OUTSw\n");
                    break;
                }
                default:
                {
                    for(; cx!=0; cx--)
                    {
                        IO_XT::wb(dx,RAM::rb(ds,si));
                        IO_XT::wb(dx+1,RAM::rb(ds,si+1));
                        if(!(flags & 0x0400)) si+=2;
                        else si-=2;
                    }
                    printf("REP OUTSw\n");
                    break;
                }
                break;
                }
                ip++;
            }
            break;
        }
        case 0x70:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JO %02x\n",tmp);
            if((flags&0x0800)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x71:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JNO %02x\n",tmp);
            if(!(flags&0x0800)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x72:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JC %02x\n",tmp);
            if((flags&0x0001)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x73:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JNC %02x\n",tmp);
            if(!(flags&0x0001)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x74:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JZ %02x\n",tmp);
            if((flags&0x0040)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x75:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JNZ %02x\n",tmp);
            if(!(flags&0x0040)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x76:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JBE %02x\n",tmp);
            if((flags&0x0040) || (flags&0x0001)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x77:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JA %02x\n",tmp);
            if(!(flags&0x0040) && !(flags&0x0001)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x78:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JS %02x\n",tmp);
            if((flags&0x0080)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x79:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JNS %02x\n",tmp);
            if(!(flags&0x0080)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x7A:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JP %02x\n",tmp);
            if((flags&0x0004)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x7B:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JNP %02x\n",tmp);
            if(!(flags&0x0004)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x7C:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            bool tmp1 = (flags >> 7) & 1;
            bool tmp2 = (flags >> 11) & 1;
            printf("JL %02x\n",tmp);
            if(tmp1 != tmp2) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x7D:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            bool tmp1 = (flags >> 7) & 1;
            bool tmp2 = (flags >> 11) & 1;
            printf("JGE %02x\n",tmp);
            if(tmp1 == tmp2) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x7E:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            bool tmp1 = (flags >> 7) & 1;
            bool tmp2 = (flags >> 11) & 1;
            printf("JLE %02x\n",tmp);
            if((tmp1 != tmp2) || (flags & 0x0040)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x7F:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            bool tmp1 = (flags >> 7) & 1;
            bool tmp2 = (flags >> 11) & 1;
            printf("JG %02x\n",tmp);
            if((tmp1 == tmp2) && !(flags & 0x0040)) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0x80:
        case 0x82:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            u8 op3 = RAM::rb(cs,ip+2);
            locs loc = decodemodrm(seg,op2,false,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                printf("ADD Eb,%02x\n",op3);
                *loc.src8 += op3;
                break;
            }
            case 0x08:
            {
                printf("OR Eb,%02x\n",op3);
                *loc.src8 |= op3;
                break;
            }
            case 0x10:
            {
                printf("ADC Eb,%02x\n",op3);
                *loc.src8 += op3 + (flags & 0x0001);
                break;
            }
            case 0x18:
            {
                printf("SBB Eb,%02x\n",op3);
                *loc.src8 -= op3 + (flags & 0x0001);
                break;
            }
            case 0x20:
            {
                printf("AND Eb,%02x\n",op3);
                *loc.src8 &= op3;
                break;
            }
            case 0x28:
            {
                printf("SUB Eb,%02x\n",op3);
                *loc.src8 -= op3;
                break;
            }
            case 0x30:
            {
                printf("XOR Eb,%02x\n",op3);
                *loc.src8 &= op3;
                break;
            }
            case 0x38:
            {
                printf("CMP Eb,%02x\n",op3);
                u8 tmp = *loc.src8 - op3;
                if(tmp==0) flags |= 0x0040;
                else flags &= 0xFFBF;
                break;
            }
            }
            ip+=3;
            break;
        }
        case 0x81:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            u8 op3 = RAM::rb(cs,ip+2) | (RAM::rb(cs,ip+3)<<8);
            locs loc = decodemodrm(seg,op2,true,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                printf("ADD Ev,%04x\n",op3);
                *loc.src16 += op3;
                break;
            }
            case 0x08:
            {
                printf("OR Ev,%04x\n",op3);
                *loc.src16 |= op3;
                break;
            }
            case 0x10:
            {
                printf("ADC Ev,%04x\n",op3);
                *loc.src16 += op3 + (flags & 0x0001);
                break;
            }
            case 0x18:
            {
                printf("SBB Ev,%04x\n",op3);
                *loc.src16 -= op3 + (flags & 0x0001);
                break;
            }
            case 0x20:
            {
                printf("AND Ev,%04x\n",op3);
                *loc.src16 &= op3;
                break;
            }
            case 0x28:
            {
                printf("SUB Eb,%04x\n",op3);
                *loc.src16 -= op3;
                break;
            }
            case 0x30:
            {
                printf("XOR Ev,%04x\n",op3);
                *loc.src16 &= op3;
                break;
            }
            case 0x38:
            {
                printf("CMP Ev,%04x\n",op3);
                u16 tmp = *loc.src16 - op3;
                if(tmp==0) flags |= 0x0040;
                else flags &= 0xFFBF;
                break;
            }
            }
            ip+=3;
            break;
        }
        case 0x83:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            u8 op3 = RAM::rb(cs,ip+2);
            locs loc = decodemodrm(seg,op2,true,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                printf("ADD Ev,%02x\n",op3);
                *loc.src16 += op3;
                break;
            }
            case 0x08:
            {
                printf("OR Ev,%02x\n",op3);
                *loc.src16 |= op3;
                break;
            }
            case 0x10:
            {
                printf("ADC Ev,%02x\n",op3);
                *loc.src16 += op3 + (flags & 0x0001);
                break;
            }
            case 0x18:
            {
                printf("SBB Ev,%02x\n",op3);
                *loc.src16 -= op3 + (flags & 0x0001);
                break;
            }
            case 0x20:
            {
                printf("AND Ev,%02x\n",op3);
                *loc.src16 &= op3;
                break;
            }
            case 0x28:
            {
                printf("SUB Eb,%02x\n",op3);
                *loc.src16 -= op3;
                break;
            }
            case 0x30:
            {
                printf("XOR Ev,%02x\n",op3);
                *loc.src16 &= op3;
                break;
            }
            case 0x38:
            {
                printf("CMP Ev,%02x\n",op3);
                u16 tmp = *loc.src16 - op3;
                if(tmp==0) flags |= 0x0040;
                else flags &= 0xFFBF;
                break;
            }
            }
            ip+=3;
            break;
        }
        case 0x84:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            u8 tmp = *loc.src8 & *loc.dst8;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("TEST Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x85:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            u16 tmp = *loc.src16 & *loc.dst16;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            ip+=2;
            printf("TEST Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x86:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            u8 tmp = *loc.src8;
            *loc.src8 = *loc.dst8;
            *loc.dst8 = tmp;
            ip+=2;
            printf("XCHG Gb,Eb modrm=%02x\n",modrm);
            break;
        }
        case 0x87:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            u16 tmp = *loc.src16;
            *loc.src16 = *loc.dst16;
            *loc.dst16 = tmp;
            ip+=2;
            printf("XCHG Gv,Ev modrm=%02x\n",modrm);
            break;
        }
        case 0x88:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.src8 = *loc.dst8;
            ip+=2;
            printf("MOV Eb,Gb modrm=%02x\n",modrm);
            break;
        }
        case 0x89:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.src16 = *loc.dst16;
            ip+=2;
            printf("MOV Ev,Gv modrm=%02x\n",modrm);
            break;
        }
        case 0x8A:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            *loc.dst8 = *loc.src8;
            ip+=2;
            printf("MOV Gb,Eb modrm=%02x\n",modrm);
            break;
        }
        case 0x8B:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            *loc.dst16 = *loc.src16;
            ip+=2;
            printf("MOV Gv,Ev modrm=%02x\n",modrm);
            break;
        }
        case 0x8C:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,true);
            *loc.src16 = *loc.dst16;
            ip+=2;
            printf("MOV Ew,Sw modrm=%02x\n",modrm);
            break;
        }
        case 0x8D:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            u16 tmp = (u16)(loc.dst16 - (u16*)&RAM::RAM[0]);
            switch(seg)
            {
            case SEG_DEFAULT:
            case SEG_DS:
            {
                tmp -= (ds<<4);
                break;
            }
            case SEG_CS:
            {
                tmp -= (cs<<4);
                break;
            }
            case SEG_ES:
            {
                tmp -= (es<<4);
                break;
            }
            case SEG_SS:
            {
                tmp -= (ss<<4);
                break;
            }
            }
            *loc.src16 = tmp;
            ip+=2;
            printf("LEA Gv,M modrm=%02x\n",modrm);
            break;
        }
        case 0x8E:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,true);
            *loc.dst16 = *loc.src16;
            ip+=2;
            printf("MOV Sw,Ew modrm=%02x\n",modrm);
            break;
        }
        case 0x8F:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,true);
            *loc.src16 = RAM::rb(ss,sp+1)|(RAM::rb(ss,sp+2)<<8);
            sp+=2;
            ip+=2;
            printf("POP Ev modrm=%02x\n",modrm);
            break;
        }
        case 0x90:
        {
            ip++;
            printf("NOP\n");
            break;
        }
        case 0x91:
        {
            ip++;
            u16 tmp = cx;
            cx = ax;
            ax = tmp;
            printf("XCHG CX,AX\n");
            break;
        }
        case 0x92:
        {
            ip++;
            u16 tmp = dx;
            dx = ax;
            ax = tmp;
            printf("XCHG DX,AX\n");
            break;
        }
        case 0x93:
        {
            ip++;
            u16 tmp = bx;
            bx = ax;
            ax = tmp;
            printf("XCHG BX,AX\n");
            break;
        }
        case 0x94:
        {
            ip++;
            u16 tmp = sp;
            sp = ax;
            ax = tmp;
            printf("XCHG SP,AX\n");
            break;
        }
        case 0x95:
        {
            ip++;
            u16 tmp = bp;
            bp = ax;
            ax = tmp;
            printf("XCHG BP,AX\n");
            break;
        }
        case 0x96:
        {
            ip++;
            u16 tmp = si;
            si = ax;
            ax = tmp;
            printf("XCHG SI,AX\n");
            break;
        }
        case 0x97:
        {
            ip++;
            u16 tmp = di;
            di = ax;
            ax = tmp;
            printf("XCHG DI,AX\n");
            break;
        }
        case 0x98:
        {
            if(al & 0x80) ah = 0xFF;
            else ah = 0;
            ip++;
            printf("CBW");
            break;
        }
        case 0x99:
        {
            if(ax & 0x8000) dx = 0xFFFF;
            else dx = 0;
            ip++;
            printf("CWD");
            break;
        }
        case 0x9A:
        {
            u16 tmp = cs;
            u16 tmp1 = ip;
            sp-=2;
            RAM::wb(ss,sp,(ip+5)&0xFF);
            RAM::wb(ss,sp+1,(ip+5)>>8);
            cs = (RAM::rb(tmp,tmp1+4)<<8)|RAM::rb(tmp,tmp1+3);
            ip = (RAM::rb(tmp,tmp1+2)<<8)|RAM::rb(tmp,tmp1+1);
            printf("CALL %04x:%04x\n",cs,ip);
            break;
        }
        case 0x9C:
        {
            sp-=2;
            RAM::wb(ss,sp,flags&0xFF);
            RAM::wb(ss,sp+1,flags>>8);
            ip++;
            printf("PUSHF\n");
            break;
        }
        case 0x9D:
        {
            flags = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp-=2;
            ip++;
            printf("POPF\n");
            break;
        }
        case 0x9E:
        {
            printf("SAHF\n");
            flags &= 0xFF2A;
            flags |= ah & 0xD5;
            ip++;
            break;
        }
        case 0x9F:
        {
            printf("LAHF\n");
            ah = (ah & 0x2A) | (flags & 0x00D5);
            ip++;
            break;
        }
        case 0xA0:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            al = RAM::rb(ds,tmp);
            ip+=3;
            printf("MOV AL, BYTE PTR DS:%04x\n",tmp);
            break;
        }
        case 0xA1:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            ax = RAM::rb(ds,tmp)|(RAM::rb(ds,tmp+1)<<8);
            ip+=3;
            printf("MOV AX, WORD PTR DS:%04x\n",tmp);
            break;
        }
        case 0xA2:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            RAM::wb(ds,tmp,al);
            ip+=3;
            printf("MOV BYTE PTR DS:%04x\n, AL",tmp);
            break;
        }
        case 0xA3:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            RAM::wb(ds,tmp,al);
            RAM::wb(ds,tmp+1,ah);
            ip+=3;
            printf("MOV WORD PTR DS:%04x\n, AX",tmp);
            break;
        }
        case 0xA4:
        {
            switch(rep)
            {
            case 0:
                RAM::wb(es,di,RAM::rb(ds,si));
                if(!(flags & 0x0400))
                {
                    di++;
                    si++;
                }
                else
                {
                    di--;
                    si--;
                }
                ip++;
                printf("MOVSB\n");
                break;
            case 1:
            case 2:
                for(; cx!=0; cx--)
                {
                    RAM::wb(es,di,RAM::rb(ds,si));
                    if(!(flags & 0x0400))
                    {
                        di++;
                        si++;
                    }
                    else
                    {
                        di--;
                        si--;
                    }
                }
                ip++;
                printf("REP MOVSB\n");
                break;
            }
            break;
        }
        case 0xA5:
        {
            switch(rep)
            {
            case 0:
                RAM::wb(es,di,RAM::rb(ds,si));
                RAM::wb(es,di+1,RAM::rb(ds,si+1));
                if(!(flags & 0x0400))
                {
                    di+=2;
                    si+=2;
                }
                else
                {
                    di-=2;
                    si-=2;
                }
                ip++;
                printf("MOVSW\n");
                break;
            case 1:
            case 2:
                for(; cx!=0; cx--)
                {
                    RAM::wb(es,di,RAM::rb(ds,si));
                    RAM::wb(es,di+1,RAM::rb(ds,si+1));
                    if(!(flags & 0x0400))
                    {
                        di+=2;
                        si+=2;
                    }
                    else
                    {
                        di-=2;
                        si-=2;
                    }
                }
                ip++;
                printf("REP MOVSW\n");
                break;
            }
            break;
        }
        case 0xA6:
        {
            switch(rep)
            {
            case 0:
            {
                u8 tmp = RAM::rb(ds,si) - RAM::rb(es,di);
                if(!(flags & 0x0400))
                {
                    di++;
                    si++;
                }
                else
                {
                    di--;
                    si--;
                }
                ip++;
                if(tmp == 0) flags |= 0x0040;
                else flags &= 0xFFBF;
                printf("CMPSB\n");
                break;
            }
            case 1:
            {
                for(; cx!=0; cx--)
                {
                    u8 tmp = RAM::rb(ds,si) - RAM::rb(es,di);
                    if(!(flags & 0x0400))
                    {
                        di++;
                        si++;
                    }
                    else
                    {
                        di--;
                        si--;
                    }
                    ip++;
                    if(tmp == 0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if(!(flags & 0x0040)) break;
                }
                printf("REPE CMPSB\n");
                break;
            }
            case 2:
                for(; cx!=0; cx--)
                {
                    u8 tmp = RAM::rb(ds,si) - RAM::rb(es,di);
                    if(!(flags & 0x0400))
                    {
                        di++;
                        si++;
                    }
                    else
                    {
                        di--;
                        si--;
                    }
                    ip++;
                    if(tmp == 0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if((flags & 0x0040)) break;
                }
                printf("REPNE CMPSB\n");
                break;
            case 3:
                for(; cx!=0; cx--)
                {
                    u8 tmp = RAM::rb(ds,si) - RAM::rb(es,di);
                    if(!(flags & 0x0400))
                    {
                        di++;
                        si++;
                    }
                    else
                    {
                        di--;
                        si--;
                    }
                    ip++;
                    if(tmp == 0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if(!(flags & 0x0001)) break;
                }
                printf("REPC CMPSB\n");
                break;
            case 4:
                for(; cx!=0; cx--)
                {
                    u8 tmp = RAM::rb(ds,si) - RAM::rb(es,di);
                    if(!(flags & 0x0400))
                    {
                        di++;
                        si++;
                    }
                    else
                    {
                        di--;
                        si--;
                    }
                    ip++;
                    if(tmp == 0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if((flags & 0x0001)) break;
                }
                printf("REPNC CMPSB\n");
                break;
            }
            break;
        }
        case 0xA7:
        {
            switch(rep)
            {
            case 0:
            {
                u16 tmp = (RAM::rb(ds,si)|(RAM::rb(ds,si+1)<<8)) - (RAM::rb(es,di)|(RAM::rb(es,di+1)<<8));
                if(!(flags & 0x0400))
                {
                    di+=2;
                    si+=2;
                }
                else
                {
                    di-=2;
                    si-=2;
                }
                ip++;
                if(tmp == 0) flags |= 0x0040;
                else flags &= 0xFFBF;
                printf("CMPSW\n");
                break;
            }
            case 1:
                for(; cx!=0; cx--)
                {
                    u16 tmp = (RAM::rb(ds,si)|(RAM::rb(ds,si+1)<<8)) - (RAM::rb(es,di)|(RAM::rb(es,di+1)<<8));
                    if(!(flags & 0x0400))
                    {
                        di+=2;
                        si+=2;
                    }
                    else
                    {
                        di-=2;
                        si-=2;
                    }
                    ip++;
                    if(tmp == 0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if(!(flags&0x0040)) break;
                }
                printf("REPE CMPSW\n");
                break;
            case 2:
                for(; cx!=0; cx--)
                {
                    u16 tmp = (RAM::rb(ds,si)|(RAM::rb(ds,si+1)<<8)) - (RAM::rb(es,di)|(RAM::rb(es,di+1)<<8));
                    if(!(flags & 0x0400))
                    {
                        di+=2;
                        si+=2;
                    }
                    else
                    {
                        di-=2;
                        si-=2;
                    }
                    ip++;
                    if(tmp == 0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if((flags&0x0040)) break;
                }
                printf("REPNE CMPSW\n");
                break;
            }
            break;
        }
        case 0xA8:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            u8 tmp1 = al & tmp;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            printf("TEST AL, %02x\n",tmp);
            ip+=2;
            break;
        }
        case 0xA9:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            u16 tmp1 = ax & tmp;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            printf("TEST AX, %04x\n",tmp);
            ip+=3;
            break;
        }
        case 0xAA:
        {
            switch(rep)
            {
            case 0:
                RAM::wb(es,di,al);
                if(!(flags&0x0400)) di++;
                else di--;
                ip++;
                printf("STOSB\n");
                break;
            case 1:
            case 2:
                for(; cx!=0; cx--)
                {
                    RAM::wb(es,di,al);
                    if(!(flags&0x0400)) di++;
                    else di--;
                }
                ip++;
                printf("REP STOSB\n");
                break;
            }
            break;
        }
        case 0xAB:
        {
            switch(rep)
            {
            case 0:
                RAM::wb(es,di,al);
                RAM::wb(es,di+1,ah);
                if(!(flags&0x0400)) di+=2;
                else di-=2;
                ip++;
                printf("STOSW\n");
                break;
            case 1:
            case 2:
                for(; cx!=0; cx--)
                {
                    RAM::wb(es,di,al);
                    RAM::wb(es,di+1,ah);
                    if(!(flags&0x0400)) di+=2;
                    else di-=2;
                }
                ip++;
                printf("STOSW\n");
                break;
            }
            break;
        }
        case 0xAC:
        {
            al = RAM::rb(es,di);
            if(!(flags&0x0400)) di++;
            else di--;
            ip++;
            printf("LODSB\n");
            break;
        }
        case 0xAD:
        {
            ax = RAM::rb(es,di)|(RAM::rb(es,di+1)<<8);
            if(!(flags&0x0400)) di+=2;
            else di-=2;
            ip++;
            printf("LODSW\n");
            break;
        }
        case 0xAE:
        {
            switch(rep)
            {
            case 0:
            {
                u8 tmp = al - RAM::rb(es,di);
                if(!(flags&0x0400)) di++;
                else di--;
                if(tmp==0) flags |= 0x0040;
                else flags &= 0xFFBF;
                ip++;
                printf("SCASB\n");
                break;
            }
            case 1:
                for(; cx!=0; cx--)
                {
                    u8 tmp = al - RAM::rb(es,di);
                    if(!(flags&0x0400)) di++;
                    else di--;
                    if(tmp==0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if(!(flags&0x0040)) break;
                }
                ip++;
                printf("REPE SCASB\n");
                break;
            case 2:
                for(; cx!=0; cx--)
                {
                    u8 tmp = al - RAM::rb(es,di);
                    if(!(flags&0x0400)) di++;
                    else di--;
                    if(tmp==0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if((flags&0x0040)) break;
                }
                ip++;
                printf("REPNE SCASB\n");
                break;
            }
            break;
        }
        case 0xAF:
        {
            switch(rep)
            {
            case 0:
            {
                u16 tmp = ax - (RAM::rb(es,di)|(RAM::rb(es,di+1)<<8));
                if(!(flags&0x0400)) di+=2;
                else di-=2;
                if(tmp==0) flags |= 0x0040;
                else flags &= 0xFFBF;
                ip++;
                printf("SCASW\n");
                break;
            }
            case 1:
                for(; cx!=0; cx--)
                {
                    u16 tmp = ax - (RAM::rb(es,di)|(RAM::rb(es,di+1)<<8));
                    if(!(flags&0x0400)) di+=2;
                    else di-=2;
                    if(tmp==0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if(!(flags&0x0040)) break;
                }
                ip++;
                printf("REPE SCASW\n");
                break;
            case 2:
                for(; cx!=0; cx--)
                {
                    u16 tmp = ax - (RAM::rb(es,di)|(RAM::rb(es,di+1)<<8));
                    if(!(flags&0x0400)) di+=2;
                    else di-=2;
                    if(tmp==0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    if((flags&0x0040)) break;
                }
                ip++;
                printf("REPNE SCASW\n");
                break;
            }
            break;
        }
        case 0xB0:
        {
            al = RAM::rb(cs,ip+1);
            printf("MOV AL,%02x\n",al);
            ip+=2;
            break;
        }
        case 0xB1:
        {
            cl = RAM::rb(cs,ip+1);
            printf("MOV CL,%02x\n",cl);
            ip+=2;
            break;
        }
        case 0xB2:
        {
            dl = RAM::rb(cs,ip+1);
            printf("MOV DL,%02x\n",dl);
            ip+=2;
            break;
        }
        case 0xB3:
        {
            bl = RAM::rb(cs,ip+1);
            printf("MOV BL,%02x\n",bl);
            ip+=2;
            break;
        }
        case 0xB4:
        {
            ah = RAM::rb(cs,ip+1);
            printf("MOV AH,%02x\n",ah);
            ip+=2;
            break;
        }
        case 0xB5:
        {
            ch = RAM::rb(cs,ip+1);
            printf("MOV CH,%02x\n",ch);
            ip+=2;
            break;
        }
        case 0xB6:
        {
            dh = RAM::rb(cs,ip+1);
            printf("MOV DH,%02x\n",dh);
            ip+=2;
            break;
        }
        case 0xB7:
        {
            bh = RAM::rb(cs,ip+1);
            printf("MOV BH,%02x\n",bh);
            ip+=2;
            break;
        }
        case 0xB8:
        {
            ax = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            printf("MOV AX,%04x\n",ax);
            ip+=3;
            break;
        }
        case 0xB9:
        {
            cx = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            printf("MOV CX,%04x\n",cx);
            ip+=3;
            break;
        }
        case 0xBA:
        {
            dx = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            printf("MOV DX,%04x\n",dx);
            ip+=3;
            break;
        }
        case 0xBB:
        {
            bx = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            printf("MOV BX,%04x\n",bx);
            ip+=3;
            break;
        }
        case 0xBC:
        {
            sp = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            printf("MOV SP,%04x\n",sp);
            ip+=3;
            break;
        }
        case 0xBD:
        {
            bp = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            printf("MOV BP,%04x\n",bp);
            ip+=3;
            break;
        }
        case 0xBE:
        {
            si = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            printf("MOV SI,%04x\n",si);
            ip+=3;
            break;
        }
        case 0xBF:
        {
            di = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            printf("MOV DI,%04x\n",di);
            ip+=3;
            break;
        }
        case 0xC2:
        {
            u16 tmp = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            ip = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp+=tmp+2;
            printf("RET %04x\n",tmp);
            break;
        }
        case 0xC3:
        {
            ip = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp+=2;
            printf("RET\n");
            break;
        }
        case 0xC4:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            u16 tmp = RAM::rb(cs,ip+2)|(RAM::rb(cs,ip+3)<<8);
            u16 tmp1 = RAM::rb(cs,ip+4)|(RAM::rb(cs,ip+5)<<8);
            *loc.dst16 = tmp;
            es = tmp1;
            ip+=6;
            printf("LES Gv,Mp modrm=%02x\n",modrm);
            break;
        }
        case 0xC5:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            u16 tmp = RAM::rb(cs,ip+2)|(RAM::rb(cs,ip+3)<<8);
            u16 tmp1 = RAM::rb(cs,ip+4)|(RAM::rb(cs,ip+5)<<8);
            *loc.dst16 = tmp;
            ds = tmp1;
            ip+=6;
            printf("LDS Gv,Mp modrm=%02x\n",modrm);
            break;
        }
        case 0xC6:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,false,false);
            u8 tmp = RAM::rb(cs,ip+2);
            *loc.src8 = tmp;
            ip+=3;
            printf("MOV Eb, %02x modrm=%02x\n",tmp,modrm);
            break;
        }
        case 0xC7:
        {
            u8 modrm = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,modrm,true,false);
            u16 tmp = RAM::rb(cs,ip+2)|(RAM::rb(cs,ip+3)<<8);
            *loc.src16 = tmp;
            ip+=4;
            printf("MOV Ev, %04x modrm=%02x\n",tmp,modrm);
            break;
        }
        case 0xC8:
        {
            if(type!=intel8086)
            {
                u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
                u8 tmp1 = RAM::rb(cs,ip+3);
                u8 tmp2 = tmp1 & 0x1F;
                sp-=2;
                RAM::wb(ss,sp,bp&0xFF);
                RAM::wb(ss,sp+1,bp>>8);
                u16 tmp3 = sp;
                if(tmp2 > 0)
                {
                    for(int i = 0; i<tmp2; i++)
                    {
                        bp-=2;
                        sp-=2;
                        RAM::wb(ss,sp,bp&0xFF);
                        RAM::wb(ss,sp+1,bp>>8);
                    }
                    sp-=2;
                    RAM::wb(ss,sp,tmp3&0xFF);
                    RAM::wb(ss,sp+1,tmp3>>8);
                }
                bp = tmp3;
                sp-=tmp;
                printf("ENTER %04x,%02x\n",tmp,tmp1);
                ip+=3;
            }
            ip++;
            break;
        }
        case 0xC9:
        {
            if(type!=intel8086)
            {
                sp = bp;
                bp = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
                printf("LEAVE\n");
            }
            ip++;
            break;
        }
        case 0xCA:
        {
            u16 tmp = (RAM::rb(cs,ip+2)<<8)|RAM::rb(cs,ip+1);
            ip = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp+=2;
            cs = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp+=tmp+2;
            printf("RETF %04x\n",tmp);
            break;
        }
        case 0xCB:
        {
            ip = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp+=2;
            cs = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp+=2;
            printf("RETF\n");
            break;
        }
        case 0xCC:
        {
            sp-=2;
            RAM::wb(ss,sp,flags & 0xFF);
            RAM::wb(ss,sp+1,flags >> 8);
            flags &= 0xFCFF;
            sp-=2;
            RAM::wb(ss,sp,cs & 0xFF);
            RAM::wb(ss,sp+1,cs >> 8);
            sp-=2;
            RAM::wb(ss,sp,(ip+1) & 0xFF);
            RAM::wb(ss,sp+1,(ip+1) >> 8);
            cs = RAM::rb(0,15)|(RAM::rb(0,14)<<8);
            ip = RAM::rb(0,12)|(RAM::rb(0,13)<<8);
            printf("INT 3\n");
            break;
        }
        case 0xCD:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            sp-=2;
            RAM::wb(ss,sp,flags & 0xFF);
            RAM::wb(ss,sp+1,flags >> 8);
            flags &= 0xFCFF;
            sp-=2;
            RAM::wb(ss,sp,cs & 0xFF);
            RAM::wb(ss,sp+1,cs >> 8);
            sp-=2;
            RAM::wb(ss,sp,(ip+2) & 0xFF);
            RAM::wb(ss,sp+1,(ip+2) >> 8);
            cs = RAM::rb(0,(tmp<<2)+3)|(RAM::rb(0,(tmp<<2)+2)<<8);
            ip = RAM::rb(0,(tmp<<2))|(RAM::rb(0,(tmp<<2)+1)<<8);
            printf("INT %02x\n",tmp);
            break;
        }
        case 0xCE:
        {
            sp-=2;
            RAM::wb(ss,sp,flags & 0xFF);
            RAM::wb(ss,sp+1,flags >> 8);
            flags &= 0xFCFF;
            sp-=2;
            RAM::wb(ss,sp,cs & 0xFF);
            RAM::wb(ss,sp+1,cs >> 8);
            sp-=2;
            RAM::wb(ss,sp,(ip+1) & 0xFF);
            RAM::wb(ss,sp+1,(ip+1) >> 8);
            cs = RAM::rb(0,19)|(RAM::rb(0,18)<<8);
            ip = RAM::rb(0,16)|(RAM::rb(0,17)<<8);
            printf("INTO\n");
            break;
        }
        case 0xCF:
        {
            ip = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp+=2;
            cs = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp+=2;
            flags = RAM::rb(ss,sp)|(RAM::rb(ss,sp+1)<<8);
            sp+=2;
            printf("IRET\n");
            break;
        }
        case 0xD0:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,op2,false,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                printf("ROL Eb,1 modrm=%02x\n",op2);
                *loc.src8 = (*loc.src8 << 1) | (*loc.src8 >> 7);
                break;
            }
            case 0x08:
            {
                printf("ROR Eb,1 modrm=%02x\n",op2);
                *loc.src8 = (*loc.src8 >> 1) | (*loc.src8 << 7);
                break;
            }
            case 0x10:
            {
                printf("RCL Eb,1 modrm=%02x\n",op2);
                u8 tmp = *loc.src8;
                u8 tmp1 = flags & 0x0001;
                *loc.src8 = (*loc.src8 << 1) | tmp1;
                flags = (flags & 0xFFFE) | (tmp >> 7);
                break;
            }
            case 0x18:
            {
                printf("RCR Eb,1 modrm=%02x\n",op2);
                u8 tmp = *loc.src8 & 1;
                u8 tmp1 = flags & 0x0001;
                *loc.src8 = (*loc.src8 >> 1) | tmp1 << 7;
                flags = (flags & 0xFFFE) | tmp;
                break;
            }
            case 0x20:
            {
                printf("SHL Eb,1 modrm=%02x\n",op2);
                flags = (flags & 0xFFFE) | ((*loc.src8 >> 7) & 1);
                u8 tmp = ((*loc.src8 >> 7) & 1);
                *loc.src8 <<= 1;
                flags = (flags & 0xF7FF) | ((((*loc.src8 >> 7) & 1) ^ tmp) << 11);
                break;
            }
            case 0x28:
            {
                printf("SHR Eb,1 modrm=%02x\n",op2);
                *loc.src8 >>= 1;
                break;
            }
            case 0x30:
            {
                printf("SAL Eb,1 modrm=%02x\n",op2);
                *loc.src8 <<= 1;
                break;
            }
            case 0x38:
            {
                printf("SAR Eb,1 modrm=%02x\n",op2);
                *loc.src8 = (*loc.src8 & 0x80) | (*loc.src8 >> 1);
                break;
            }
            }
            ip+=2;
            break;
        }
        case 0xD1:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,op2,true,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                printf("ROL Ev,1 modrm=%02x\n",op2);
                *loc.src16 = (*loc.src16 << 1) | (*loc.src16 >> 15);
                break;
            }
            case 0x08:
            {
                printf("ROR Ev,1 modrm=%02x\n",op2);
                *loc.src16 = (*loc.src16 >> 1) | (*loc.src16 << 15);
                break;
            }
            case 0x10:
            {
                printf("RCL Ev,1 modrm=%02x\n",op2);
                u8 tmp = *loc.src16;
                u8 tmp1 = flags & 0x0001;
                *loc.src16 = (*loc.src16 << 1) | tmp1;
                flags = (flags & 0xFFFE) | (tmp >> 15);
                break;
            }
            case 0x18:
            {
                printf("RCR Ev,1 modrm=%02x\n",op2);
                u8 tmp = *loc.src16 & 1;
                u8 tmp1 = flags & 0x0001;
                *loc.src16 = (*loc.src16 >> 1) | tmp1 << 15;
                flags = (flags & 0xFFFE) | tmp;
                break;
            }
            case 0x20:
            {
                printf("SHL Ev,1 modrm=%02x\n",op2);
                u16 tmp = *loc.src16 >> 15;
                *loc.src16 <<= 1;
                flags = (flags & 0xFFFE) | tmp;
                if(*loc.src16 == 0) flags |= 0x0040;
                else flags &= 0xFFBF;
                break;
            }
            case 0x28:
            {
                printf("SHR Ev,1 modrm=%02x\n",op2);
                *loc.src16 >>= 1;
                break;
            }
            case 0x30:
            {
                printf("SAL Ev,1 modrm=%02x\n",op2);
                *loc.src16 <<= 1;
                break;
            }
            case 0x38:
            {
                printf("SAR Ev,1 modrm=%02x\n",op2);
                *loc.src16 = (*loc.src16 & 0x8000) | (*loc.src16 >> 1);
                break;
            }
            }
            ip+=2;
            break;
        }
        case 0xD2:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,op2,false,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                printf("ROL Eb,CL modrm=%02x\n",op2);
                *loc.src8 = (*loc.src8 << cl) | (*loc.src8 >> (8-cl));
                break;
            }
            case 0x08:
            {
                printf("ROR Eb,CL modrm=%02x\n",op2);
                *loc.src8 = (*loc.src8 >> cl) | (*loc.src8 << (8-cl));
                break;
            }
            case 0x10:
            {
                printf("RCL Eb,CL modrm=%02x\n",op2);
                u8 tmp = *loc.src8;
                u8 tmp1 = flags & 0x0001;
                *loc.src8 = (*loc.src8 << cl) | tmp1;
                flags = (flags & 0xFFFE) | (tmp >> (8-cl));
                break;
            }
            case 0x18:
            {
                printf("RCR Eb,CL modrm=%02x\n",op2);
                u8 tmp = *loc.src8 & 1;
                u8 tmp1 = flags & 0x0001;
                *loc.src8 = (*loc.src8 >> cl) | tmp1 << (8-cl);
                flags = (flags & 0xFFFE) | tmp;
                break;
            }
            case 0x20:
            {
                printf("SHL Eb,CL modrm=%02x\n",op2);
                *loc.src8 <<= cl;
                break;
            }
            case 0x28:
            {
                printf("SHR Eb,CL modrm=%02x\n",op2);
                *loc.src8 >>= cl;
                break;
            }
            case 0x30:
            {
                printf("SAL Eb,CL modrm=%02x\n",op2);
                *loc.src8 <<= cl;
                break;
            }
            case 0x38:
            {
                printf("SAR Eb,CL modrm=%02x\n",op2);
                *loc.src8 = (*loc.src8 & 0x80) | (*loc.src8 >> cl);
                break;
            }
            }
            ip+=2;
            break;
        }
        case 0xD3:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,op2,true,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                printf("ROL Ev,CL modrm=%02x\n",op2);
                *loc.src16 = (*loc.src16 << cl) | (*loc.src16 >> (16-cl));
                break;
            }
            case 0x08:
            {
                printf("ROR Ev,CL modrm=%02x\n",op2);
                *loc.src16 = (*loc.src16 >> cl) | (*loc.src16 << (16-cl));
                break;
            }
            case 0x10:
            {
                printf("RCL Ev,CL modrm=%02x\n",op2);
                u8 tmp = *loc.src16;
                u8 tmp1 = flags & 0x0001;
                *loc.src16 = (*loc.src16 << cl) | tmp1;
                flags = (flags & 0xFFFE) | (tmp >> (16-cl));
                break;
            }
            case 0x18:
            {
                printf("RCR Ev,CL modrm=%02x\n",op2);
                u8 tmp = *loc.src16 & 1;
                u8 tmp1 = flags & 0x0001;
                *loc.src16 = (*loc.src16 >> cl) | tmp1 << (16-cl);
                flags = (flags & 0xFFFE) | tmp;
                break;
            }
            case 0x20:
            {
                printf("SHL Ev,CL modrm=%02x\n",op2);
                *loc.src16 <<= cl;
                break;
            }
            case 0x28:
            {
                printf("SHR Ev,CL modrm=%02x\n",op2);
                *loc.src16 >>= cl;
                break;
            }
            case 0x30:
            {
                printf("SAL Ev,CL modrm=%02x\n",op2);
                *loc.src16 <<= cl;
                break;
            }
            case 0x38:
            {
                printf("SAR Ev,CL modrm=%02x\n",op2);
                *loc.src16 = (*loc.src16 & 0x8000) | (*loc.src16 >> cl);
                break;
            }
            }
            ip+=2;
            break;
        }
        case 0xD4:
        {
            if(type != necv20)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                u8 tmp1 = al / tmp;
                ah = tmp1;
                u8 tmp2 = al - (tmp1 * tmp);
                al = tmp2;
                ip+=2;
                printf("AAM %02x\n",tmp);
            }
            else
            {
                u8 tmp1 = al / 10;
                ah = tmp1;
                u8 tmp2 = al - (tmp1 * 10);
                al = tmp2;
                ip+=2;
                printf("AAM 10\n");
            }
            break;
        }
        case 0xD5:
        {
            if(type != necv20)
            {
                u8 tmp = RAM::rb(cs,ip+1);
                al = (ah * tmp) + al;
                ah = 0;
                ip+=2;
                printf("AAD %02x\n",tmp);
            }
            else
            {
                al = (ah * 10) + al;
                ah = 0;
                ip+=2;
                printf("AAD 10\n");
            }
            break;
        }
        case 0xD6:
        {
            if(type != necv20)
            {
                if(flags&0x0001) al = 0xFF;
                else al = 0;
                printf("SALC\n");
            }
            ip++;
            break;
        }
        case 0xD7:
        {
            al = RAM::rb(ds,bx+al);
            ip++;
            printf("XLAT\n");
            break;
        }
        case 0xE0:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("LOOPNZ %02x\n",tmp);
            cx--;
            if((cx!=0) && (!(flags & 0x0040))) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0xE1:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("LOOPZ %02x\n",tmp);
            cx--;
            if((cx!=0) && ((flags & 0x0040))) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0xE2:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("LOOP %02x\n",tmp);
            cx--;
            if(cx!=0) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0xE3:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JCXZ %02x\n",tmp);
            if(cx==0) ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0xE4:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("IN AL,%02x\n",tmp);
            al = IO_XT::rb((u16)tmp);
            ip+=2;
            break;
        }
        case 0xE5:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("IN AX,%02x\n",tmp);
            ax = IO_XT::rb((u16)tmp)|(IO_XT::rb((u16)(tmp+1))<<8);
            ip+=2;
            break;
        }
        case 0xE6:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("OUT %02x,AL\n",tmp);
            IO_XT::wb((u16)tmp,al);
            ip+=2;
            break;
        }
        case 0xE7:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("OUT %02x,AX\n",tmp);
            IO_XT::wb((u16)tmp,al);
            IO_XT::wb((u16)(tmp+1),ah);
            ip+=2;
            break;
        }
        case 0xE8:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            printf("CALL %04x\n",tmp);
            sp-=2;
            RAM::wb(ss,sp,(ip+3)&0xFF);
            RAM::wb(ss,sp+1,(ip+3)>>8);
            ip += (s16)tmp;
            ip+=3;
            break;
        }
        case 0xE9:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            printf("JMP %04x\n",tmp);
            ip += (s16)tmp;
            ip+=3;
            break;
        }
        case 0xEA:
        {
            u16 tmp = cs;
            u16 tmp1 = ip;
            cs = (RAM::rb(tmp,tmp1+4)<<8)|RAM::rb(tmp,tmp1+3);
            ip = (RAM::rb(tmp,tmp1+2)<<8)|RAM::rb(tmp,tmp1+1);
            printf("JMP %04x:%04x\n",cs,ip);
            break;
        }
        case 0xEB:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            printf("JMP %02x\n",tmp);
            ip += (s8)tmp;
            ip+=2;
            break;
        }
        case 0xEC:
        {
            printf("IN AL,DX\n");
            al = IO_XT::rb(dx);
            ip++;
            break;
        }
        case 0xED:
        {
            printf("IN AX,DX\n");
            al = IO_XT::rb(dx);
            ah = IO_XT::rb(dx+1);
            ip++;
            break;
        }
        case 0xEE:
        {
            printf("OUT DX,AL\n");
            IO_XT::wb(dx,al);
            ip++;
            break;
        }
        case 0xEF:
        {
            printf("OUT DX,AX\n");
            IO_XT::wb(dx,al);
            IO_XT::wb(dx+1,ah);
            ip++;
            break;
        }
        case 0xF5:
        {
            printf("CMC\n");
            flags ^= 0x0001;
            ip++;
            break;
        }
        case 0xF6:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,op2,false,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                u8 op3 = RAM::rb(cs,ip+2);
                u8 tmp = *loc.src8 & op3;
                if(tmp == 0) flags |= 0x0040;
                else flags &= 0xFFBF;
                ip++;
                printf("TEST Eb,%02x\n",op3);
                break;
            }
            case 0x10:
            {
                *loc.src8 = ~*loc.src8;
                printf("NOT Eb\n");
                break;
            }
            case 0x18:
            {
                *loc.src8 = (~*loc.src8) + 1;
                printf("NEG Eb\n");
                break;
            }
            case 0x20:
            {
                ax = *loc.src8 * al;
                printf("MUL Eb\n");
                break;
            }
            case 0x28:
            {
                ax = (s16)(*loc.src8 * al);
                printf("IMUL Eb\n");
                break;
            }
            case 0x30:
            {
                u8 tmp = ax / *loc.src8;
                u8 tmp1 = ax - (tmp * (*loc.src8));
                al = tmp;
                ah = tmp1;
                printf("DIV Eb\n");
                break;
            }
            case 0x38:
            {
                u8 tmp = ax / *loc.src8;
                u8 tmp1 = ax - (tmp * (*loc.src8));
                al = tmp;
                ah = tmp1;
                printf("IDIV Eb\n");
                break;
            }
            }
            ip+=2;
            break;
        }
        case 0xF7:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,op2,true,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                u16 op3 = RAM::rb(cs,ip+2) | (RAM::rb(cs,ip+3)<<8);
                u16 tmp = *loc.src16 & op3;
                if(tmp == 0) flags |= 0x0040;
                else flags &= 0xFFBF;
                ip+=2;
                printf("TEST Ev,%04x\n",op3);
                break;
            }
            case 0x10:
            {
                *loc.src16 = ~*loc.src16;
                printf("NOT Ev\n");
                break;
            }
            case 0x18:
            {
                *loc.src16 = (~*loc.src16) + 1;
                printf("NEG Ev\n");
                break;
            }
            case 0x20:
            {
                u32 tmp = *loc.src16 * ax;
                ax = tmp & 0xFFFF;
                dx = tmp >> 16;
                printf("MUL Ev\n");
                break;
            }
            case 0x28:
            {
                s32 tmp = *loc.src16 * ax;
                ax = (u32)tmp & 0xFFFF;
                dx = (u32)tmp >> 16;
                printf("IMUL Ev\n");
                break;
            }
            case 0x30:
            {
                u16 tmp = (ax|(dx<<16)) / *loc.src8;
                u16 tmp1 = (ax|(dx<<16)) - (tmp * (*loc.src16));
                ax = tmp;
                dx = tmp1;
                printf("DIV Ev\n");
                break;
            }
            case 0x38:
            {
                u16 tmp = (ax|(dx<<16)) / *loc.src8;
                u16 tmp1 = (ax|(dx<<16)) - (tmp * (*loc.src16));
                ax = tmp;
                dx = tmp1;
                printf("IDIV Ev\n");
                break;
            }
            }
            ip+=2;
            break;
        }
        case 0xF8:
        {
            printf("CLC\n");
            flags &= 0xFFFE;
            ip++;
            break;
        }
        case 0xF9:
        {
            printf("STC\n");
            flags |= 0x0001;
            ip++;
            break;
        }
        case 0xFA:
        {
            printf("CLI\n");
            flags &= 0xFDFF;
            ip++;
            break;
        }
        case 0xFB:
        {
            printf("STI\n");
            flags |= 0x0200;
            ip++;
            break;
        }
        case 0xFC:
        {
            printf("CLD\n");
            flags &= 0xFBFF;
            ip++;
            break;
        }
        case 0xFD:
        {
            printf("STD\n");
            flags |= 0x0400;
            ip++;
            break;
        }
        case 0xFE:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,op2,false,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                printf("INC Eb\n");
                *loc.src8++;
                break;
            }
            case 0x08:
            {
                printf("DEC Eb\n");
                *loc.src8--;
                break;
            }
            }
            ip+=2;
            break;
        }
        case 0xFF:
        {
            u8 op2 = RAM::rb(cs,ip+1);
            locs loc = decodemodrm(seg,op2,true,false);
            switch(op2&0x38)
            {
            case 0x00:
            {
                printf("INC Ev\n");
                *loc.src16++;
                break;
            }
            case 0x08:
            {
                printf("DEC Ev\n");
                *loc.src16--;
                break;
            }
            case 0x10:
            {
                printf("CALL Ev\n");
                sp-=2;
                RAM::wb(ss,sp,(ip+2)&0xFF);
                RAM::wb(ss,sp+1,(ip+2)>>8);
                ip = *loc.src16;
                break;
            }
            case 0x18:
            {
                u16 tmp = RAM::rb(cs,ip+2)|(RAM::rb(cs,ip+3)<<8);
                u16 tmp1 = RAM::rb(cs,ip+4)|(RAM::rb(cs,ip+5)<<8);
                printf("CALL %04x:%04x\n",tmp1,tmp);
                sp-=2;
                RAM::wb(ss,sp,(ip+2)&0xFF);
                RAM::wb(ss,sp+1,(ip+2)>>8);
                sp-=2;
                RAM::wb(ss,sp,cs&0xFF);
                RAM::wb(ss,sp+1,cs>>8);
                ip = tmp;
                cs = tmp1;
                break;
            }
            case 0x20:
            {
                printf("JMP Ev\n");
                ip = *loc.src16;
                break;
            }
            case 0x28:
            {
                u16 tmp = RAM::rb(cs,ip+2)|(RAM::rb(cs,ip+3)<<8);
                u16 tmp1 = RAM::rb(cs,ip+4)|(RAM::rb(cs,ip+5)<<8);
                printf("JMP %04x:%04x\n",tmp1,tmp);
                ip = tmp;
                cs = tmp1;
                break;
            }
            case 0x30:
            {
                printf("PUSH Ev\n");
                sp-=2;
                RAM::wb(ss,sp,*loc.src16 & 0xFF);
                RAM::wb(ss,sp+1,*loc.src16 >> 8);
                break;
            }
            }
            ip+=2;
            break;
        }
        default:
        {
            printf("Unemulated or invalid opcode %02x!\n",op);
            ip++;
            break;
        }
        }
        if(hint)
        {
            u8 tmp = hintnum + PIC::pic[0].offset;
            sp-=2;
            RAM::wb(ss,sp,flags & 0xFF);
            RAM::wb(ss,sp+1,flags >> 8);
            flags &= 0xFCFF;
            sp-=2;
            RAM::wb(ss,sp,cs & 0xFF);
            RAM::wb(ss,sp+1,cs >> 8);
            sp-=2;
            RAM::wb(ss,sp,(ip+2) & 0xFF);
            RAM::wb(ss,sp+1,(ip+2) >> 8);
            cs = RAM::rb(0,(tmp<<2)+3)|(RAM::rb(0,(tmp<<2)+2)<<8);
            ip = RAM::rb(0,(tmp<<2))|(RAM::rb(0,(tmp<<2)+1)<<8);
            printf("Hardware interrupt %02x triggered!\n",tmp);
        }
    }
    else if(!halted && i8080)
    {
        u8 op = RAM::rb(cs,ip); //TODO: I'm guessing that the NEC V20's 8080 mode uses the same stack pointer and instruction pointer.
        switch(op)
        {
        case 0x00:
        case 0x08:
        case 0x10:
        case 0x18:
        case 0x20:
        case 0x28:
        case 0x30:
        case 0x38:
        {
            ip++;
            printf("NOP\n");
            break;
        }
        case 0x01:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            cx = tmp;
            ip+=3;
            printf("LD BC,%04x\n",tmp);
            break;
        }
        case 0x02:
        {
            RAM::wb(cs,cx,al);
            ip++;
            printf("LD (BC),A\n");
            break;
        }
        case 0x03:
        {
            cx++;
            ip++;
            printf("INC BC\n");
            break;
        }
        case 0x04:
        {
            ch++;
            ip++;
            printf("INC B\n");
            break;
        }
        case 0x05:
        {
            ch--;
            ip++;
            printf("DEC B\n");
            break;
        }
        case 0x06:
        {
            ch = RAM::rb(cs,ip+1);
            ip+=2;
            printf("LD B,%02x\n",ch);
            break;
        }
        case 0x07:
        {
            u8 tmp = al;
            u8 tmp1 = flags & 1;
            al <<= 1;
            flags = (flags & 0xFFFE) | (tmp >> 7);
            al = (al & 0xFE) | tmp1;
            ip+=2;
            printf("RLCA\n");
            break;
        }
        case 0x09:
        {
            bx += cx;
            ip++;
            printf("ADD HL,BC\n");
            break;
        }
        case 0x0A:
        {
            al = RAM::rb(cs,cx);
            ip++;
            printf("LD A,(BC)\n");
            break;
        }
        case 0x0B:
        {
            cx--;
            ip++;
            printf("DEC BC\n");
            break;
        }
        case 0x0C:
        {
            cl++;
            ip++;
            printf("INC C\n");
            break;
        }
        case 0x0D:
        {
            cl--;
            ip++;
            printf("DEC C\n");
            break;
        }
        case 0x0E:
        {
            cl = RAM::rb(cs,ip+1);
            ip+=2;
            printf("LD C,%02x\n",cl);
            break;
        }
        case 0x0F:
        {
            u8 tmp = al;
            u8 tmp1 = flags & 1;
            al >>= 1;
            flags = (flags & 0xFFFE) | (tmp & 1);
            al = (al & 0x7F) | (tmp1 << 7);
            ip+=2;
            printf("RRCA\n");
            break;
        }
        case 0x11:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            dx = tmp;
            ip++;
            printf("LD DE,%04x\n",tmp);
            break;
        }
        case 0x12:
        {
            RAM::wb(cs,dx,al);
            ip++;
            printf("LD (DE),A\n");
            break;
        }
        case 0x13:
        {
            dx++;
            ip++;
            printf("INC DE\n");
            break;
        }
        case 0x14:
        {
            dh++;
            ip++;
            printf("INC D\n");
            break;
        }
        case 0x15:
        {
            dh--;
            ip++;
            printf("DEC D\n");
            break;
        }
        case 0x16:
        {
            dh = RAM::rb(cs,ip+1);
            ip+=2;
            printf("LD D,%02x\n",dh);
            break;
        }
        case 0x17:
        {
            u8 tmp = al;
            u8 tmp1 = flags & 1;
            al <<= 1;
            flags = (flags & 0xFE) | (tmp >> 7);
            al = (al & 0xFE) | tmp1;
            ip+=2;
            printf("RLA\n");
            break;
        }
        case 0x19:
        {
            bx += dx;
            ip++;
            printf("ADD HL,DE\n");
            break;
        }
        case 0x1A:
        {
            al = RAM::rb(cs,dx);
            ip++;
            printf("LD A,(DE)\n");
            break;
        }
        case 0x1B:
        {
            dx--;
            ip++;
            printf("DEC DE\n");
            break;
        }
        case 0x1C:
        {
            dl++;
            ip++;
            printf("INC E\n");
            break;
        }
        case 0x1D:
        {
            dl--;
            ip++;
            printf("DEC E\n");
            break;
        }
        case 0x1E:
        {
            dl = RAM::rb(cs,ip+1);
            ip+=2;
            printf("LD E,%02x\n",dl);
            break;
        }
        case 0x1F:
        {
            u8 tmp = al;
            u8 tmp1 = flags & 1;
            al >>= 1;
            flags = (flags & 0xFE) | (tmp & 1);
            al = (al & 0x7F) | (tmp1 << 7);
            ip+=2;
            printf("RRA\n");
            break;
        }
        case 0x21:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            bx = tmp;
            ip++;
            printf("LD HL,%04x\n",tmp);
            break;
        }
        case 0x22:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            RAM::wb(cs,tmp,bl);
            RAM::wb(cs,tmp+1,bh);
            ip++;
            printf("LD (%04x),HL\n",tmp);
            break;
        }
        case 0x23:
        {
            bx++;
            ip++;
            printf("INC HL\n");
            break;
        }
        case 0x24:
        {
            bh++;
            ip++;
            printf("INC H\n");
            break;
        }
        case 0x25:
        {
            bh--;
            ip++;
            printf("DEC H\n");
            break;
        }
        case 0x26:
        {
            bh = RAM::rb(cs,ip+1);
            ip+=2;
            printf("LD H,%02x\n",bh);
            break;
        }
        case 0x27:
        {
            if((al&0x0F)>9||(flags&0x10))
            {
                al += 0x06;
            }
            if((al&0xF0)>0x90||(flags&0x01))
            {
                al += 0x60;
                flags |= 0x01;
            }
            else flags &= 0xFE;
            ip++;
            printf("DAA\n");
            break;
        }
        case 0x29:
        {
            bx <<= 1; // x + x = 2x = x << 1
            ip++;
            printf("ADD HL,HL\n");
            break;
        }
        case 0x2A:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            u16 tmp1 = RAM::rb(cs,tmp)|(RAM::rb(cs,tmp+1)<<8);
            bx = tmp1;
            ip++;
            printf("LD HL,(%04x)\n",tmp);
            break;
        }
        case 0x2B:
        {
            bx--;
            ip++;
            printf("DEC HL\n");
            break;
        }
        case 0x2C:
        {
            bl++;
            ip++;
            printf("INC L\n");
            break;
        }
        case 0x2D:
        {
            bl--;
            ip++;
            printf("DEC L\n");
            break;
        }
        case 0x2E:
        {
            bl = RAM::rb(cs,ip+1);
            ip+=2;
            printf("LD L,%02x\n",bl);
            break;
        }
        case 0x2F:
        {
            al = ~al;
            ip++;
            printf("CPL\n");
            break;
        }
        case 0x31:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            sp = tmp;
            ip+=3;
            printf("LD SP,%04x\n",tmp);
            break;
        }
        case 0x32:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            RAM::wb(cs,tmp,al);
            ip+=3;
            printf("LD (%04x),A\n",tmp);
            break;
        }
        case 0x33:
        {
            sp++;
            ip++;
            printf("INC SP\n");
            break;
        }
        case 0x34:
        {
            u16 tmp = RAM::rb(cs,bx)|(RAM::rb(cs,bx+1)<<8);
            RAM::wb(cs,bx,(tmp+1)&0xFF);
            RAM::wb(cs,bx+1,(tmp+1)>>8);
            ip++;
            printf("INC (HL)\n");
            break;
        }
        case 0x35:
        {
            u16 tmp = RAM::rb(cs,bx)|(RAM::rb(cs,bx+1)<<8);
            RAM::wb(cs,bx,(tmp-1)&0xFF);
            RAM::wb(cs,bx+1,(tmp-1)>>8);
            ip++;
            printf("DEC (HL)\n");
            break;
        }
        case 0x36:
        {
            u8 tmp = RAM::rb(cs,ip+1);
            RAM::wb(cs,bx,tmp);
            ip++;
            printf("LD (HL),%02x\n",tmp);
            break;
        }
        case 0x37:
        {
            flags = (flags & 0xFE) | 1;
            ip++;
            printf("SCF\n");
            break;
        }
        case 0x39:
        {
            bx += sp;
            ip++;
            printf("ADD HL,SP\n");
            break;
        }
        case 0x3A:
        {
            u16 tmp = RAM::rb(cs,ip+1)|(RAM::rb(cs,ip+2)<<8);
            al = tmp;
            ip+=3;
            printf("LD A,(%04x)\n",tmp);
            break;
        }
        case 0x3B:
        {
            sp--;
            ip++;
            printf("DEC SP\n");
            break;
        }
        case 0x3C:
        {
            al++;
            ip++;
            printf("INC A\n");
            break;
        }
        case 0x3D:
        {
            al--;
            ip++;
            printf("DEC A\n");
            break;
        }
        case 0x3E:
        {
            al = RAM::rb(cs,ip+1);
            ip+=2;
            printf("LD A,%02x\n",al);
            break;
        }
        case 0x3F:
        {
            flags ^= 1;
            ip++;
            printf("CCF\n");
            break;
        }
        case 0x40:
        {
            ch = ch;
            ip++;
            printf("LD B,B\n");
            break;
        }
        case 0x41:
        {
            ch = cl;
            ip++;
            printf("LD B,C\n");
            break;
        }
        case 0x42:
        {
            ch = dh;
            ip++;
            printf("LD B,D\n");
            break;
        }
        case 0x43:
        {
            ch = dl;
            ip++;
            printf("LD B,E\n");
            break;
        }
        case 0x44:
        {
            ch = bh;
            ip++;
            printf("LD B,H\n");
            break;
        }
        case 0x45:
        {
            ch = bl;
            ip++;
            printf("LD B,L\n");
            break;
        }
        case 0x46:
        {
            ch = RAM::rb(cs,bx);
            ip++;
            printf("LD B,(HL)\n");
            break;
        }
        case 0x47:
        {
            ch = al;
            ip++;
            printf("LD B,A\n");
            break;
        }
        case 0x48:
        {
            cl = ch;
            ip++;
            printf("LD C,B\n");
            break;
        }
        case 0x49:
        {
            cl = cl;
            ip++;
            printf("LD C,C\n");
            break;
        }
        case 0x4A:
        {
            cl = dh;
            ip++;
            printf("LD C,D\n");
            break;
        }
        case 0x4B:
        {
            cl = dl;
            ip++;
            printf("LD C,E\n");
            break;
        }
        case 0x4C:
        {
            cl = bh;
            ip++;
            printf("LD C,H\n");
            break;
        }
        case 0x4D:
        {
            cl = bl;
            ip++;
            printf("LD C,L\n");
            break;
        }
        case 0x4E:
        {
            cl = RAM::rb(cs,bx);
            ip++;
            printf("LD C,(HL)\n");
            break;
        }
        case 0x4F:
        {
            cl = al;
            ip++;
            printf("LD C,A\n");
            break;
        }
        case 0x50:
        {
            dh = ch;
            ip++;
            printf("LD D,B\n");
            break;
        }
        case 0x51:
        {
            dh = cl;
            ip++;
            printf("LD D,C\n");
            break;
        }
        case 0x52:
        {
            dh = dh;
            ip++;
            printf("LD D,D\n");
            break;
        }
        case 0x53:
        {
            dh = dl;
            ip++;
            printf("LD D,E\n");
            break;
        }
        case 0x54:
        {
            dh = bh;
            ip++;
            printf("LD D,H\n");
            break;
        }
        case 0x55:
        {
            dh = bl;
            ip++;
            printf("LD D,L\n");
            break;
        }
        case 0x56:
        {
            dh = RAM::rb(cs,bx);
            ip++;
            printf("LD D,(HL)\n");
            break;
        }
        case 0x57:
        {
            dh = al;
            ip++;
            printf("LD D,A\n");
            break;
        }
        case 0x58:
        {
            dl = ch;
            ip++;
            printf("LD E,B\n");
            break;
        }
        case 0x59:
        {
            dl = cl;
            ip++;
            printf("LD E,C\n");
            break;
        }
        case 0x5A:
        {
            dl = dh;
            ip++;
            printf("LD E,D\n");
            break;
        }
        case 0x5B:
        {
            dl = dl;
            ip++;
            printf("LD E,E\n");
            break;
        }
        case 0x5C:
        {
            dl = bh;
            ip++;
            printf("LD E,H\n");
            break;
        }
        case 0x5D:
        {
            dl = bl;
            ip++;
            printf("LD E,L\n");
            break;
        }
        case 0x5E:
        {
            dl = RAM::rb(cs,bx);
            ip++;
            printf("LD E,(HL)\n");
            break;
        }
        case 0x5F:
        {
            dl = al;
            ip++;
            printf("LD E,A\n");
            break;
        }
        case 0x60:
        {
            bh = ch;
            ip++;
            printf("LD H,B\n");
            break;
        }
        case 0x61:
        {
            bh = cl;
            ip++;
            printf("LD H,C\n");
            break;
        }
        case 0x62:
        {
            bh = dh;
            ip++;
            printf("LD H,D\n");
            break;
        }
        case 0x63:
        {
            bh = dl;
            ip++;
            printf("LD H,E\n");
            break;
        }
        case 0x64:
        {
            bh = bh;
            ip++;
            printf("LD H,H\n");
            break;
        }
        case 0x65:
        {
            bh = bl;
            ip++;
            printf("LD H,L\n");
            break;
        }
        case 0x66:
        {
            bh = RAM::rb(cs,bx);
            ip++;
            printf("LD H,(HL)\n");
            break;
        }
        case 0x67:
        {
            bh = al;
            ip++;
            printf("LD H,A\n");
            break;
        }
        case 0x68:
        {
            bl = ch;
            ip++;
            printf("LD L,B\n");
            break;
        }
        case 0x69:
        {
            bl = cl;
            ip++;
            printf("LD L,C\n");
            break;
        }
        case 0x6A:
        {
            bl = dh;
            ip++;
            printf("LD L,D\n");
            break;
        }
        case 0x6B:
        {
            bl = dl;
            ip++;
            printf("LD L,E\n");
            break;
        }
        case 0x6C:
        {
            bl = bh;
            ip++;
            printf("LD L,H\n");
            break;
        }
        case 0x6D:
        {
            bl = bl;
            ip++;
            printf("LD L,L\n");
            break;
        }
        case 0x6E:
        {
            bl = RAM::rb(cs,bx);
            ip++;
            printf("LD L,(HL)\n");
            break;
        }
        case 0x6F:
        {
            bl = al;
            ip++;
            printf("LD L,A\n");
            break;
        }
        case 0x70:
        {
            RAM::wb(cs,bx,ch);
            ip++;
            printf("LD (HL),B\n");
            break;
        }
        case 0x71:
        {
            RAM::wb(cs,bx,cl);
            ip++;
            printf("LD (HL),C\n");
            break;
        }
        case 0x72:
        {
            RAM::wb(cs,bx,dh);
            ip++;
            printf("LD (HL),D\n");
            break;
        }
        case 0x73:
        {
            RAM::wb(cs,bx,dl);
            ip++;
            printf("LD (HL),E\n");
            break;
        }
        case 0x74:
        {
            RAM::wb(cs,bx,bh);
            ip++;
            printf("LD (HL),H\n");
            break;
        }
        case 0x75:
        {
            RAM::wb(cs,bx,bl);
            ip++;
            printf("LD (HL),L\n");
            break;
        }
        case 0x76:
        {
            ip++;
            halted = true;
            printf("HLT\n");
            break;
        }
        case 0x77:
        {
            RAM::wb(cs,bx,al);
            ip++;
            printf("LD (HL),A\n");
            break;
        }
        case 0x78:
        {
            al = ch;
            ip++;
            printf("LD A,B\n");
            break;
        }
        case 0x79:
        {
            al = cl;
            ip++;
            printf("LD A,C\n");
            break;
        }
        case 0x7A:
        {
            al = dh;
            ip++;
            printf("LD A,D\n");
            break;
        }
        case 0x7B:
        {
            al = dl;
            ip++;
            printf("LD A,E\n");
            break;
        }
        case 0x7C:
        {
            al = bh;
            ip++;
            printf("LD A,H\n");
            break;
        }
        case 0x7D:
        {
            al = bl;
            ip++;
            printf("LD A,L\n");
            break;
        }
        case 0x7E:
        {
            al = RAM::rb(cs,bx);
            ip++;
            printf("LD A,(HL)\n");
            break;
        }
        case 0x7F:
        {
            al = al;
            ip++;
            printf("LD A,A\n");
            break;
        }
        case 0x80:
        {
            al += ch;
            ip++;
            printf("ADD A,B\n");
            break;
        }
        case 0x81:
        {
            al += cl;
            ip++;
            printf("ADD A,C\n");
            break;
        }
        case 0x82:
        {
            al += dh;
            ip++;
            printf("ADD A,D\n");
            break;
        }
        case 0x83:
        {
            al += dl;
            ip++;
            printf("ADD A,E\n");
            break;
        }
        case 0x84:
        {
            al += bh;
            ip++;
            printf("ADD A,H\n");
            break;
        }
        case 0x85:
        {
            al += bl;
            ip++;
            printf("ADD A,L\n");
            break;
        }
        case 0x86:
        {
            al += RAM::rb(cs,bx);
            ip++;
            printf("ADD A,(HL)\n");
            break;
        }
        case 0x87:
        {
            al <<= 1;
            ip++;
            printf("ADD A,A\n");
            break;
        }
        case 0x88:
        {
            al += ch + (flags & 1);
            ip++;
            printf("ADC A,B\n");
            break;
        }
        case 0x89:
        {
            al += cl + (flags & 1);
            ip++;
            printf("ADC A,C\n");
            break;
        }
        case 0x8A:
        {
            al += dh + (flags & 1);
            ip++;
            printf("ADC A,D\n");
            break;
        }
        case 0x8B:
        {
            al += dl + (flags & 1);
            ip++;
            printf("ADC A,E\n");
            break;
        }
        case 0x8C:
        {
            al += bh + (flags & 1);
            ip++;
            printf("ADC A,H\n");
            break;
        }
        case 0x8D:
        {
            al += bl + (flags & 1);
            ip++;
            printf("ADC A,L\n");
            break;
        }
        case 0x8E:
        {
            al += RAM::rb(cs,bx) + (flags & 1);
            ip++;
            printf("ADC A,(HL)\n");
            break;
        }
        case 0x8F:
        {
            al += al + (flags & 1);
            ip++;
            printf("ADC A,A\n");
            break;
        }
        case 0x90:
        {
            al -= ch;
            ip++;
            printf("SUB A,B\n");
            break;
        }
        case 0x91:
        {
            al -= cl;
            ip++;
            printf("SUB A,C\n");
            break;
        }
        case 0x92:
        {
            al -= dh;
            ip++;
            printf("SUB A,D\n");
            break;
        }
        case 0x93:
        {
            al -= dl;
            ip++;
            printf("SUB A,E\n");
            break;
        }
        case 0x94:
        {
            al -= bh;
            ip++;
            printf("SUB A,H\n");
            break;
        }
        case 0x95:
        {
            al -= bl;
            ip++;
            printf("SUB A,L\n");
            break;
        }
        case 0x96:
        {
            al -= RAM::rb(cs,bx);
            ip++;
            printf("SUB A,(HL)\n");
            break;
        }
        case 0x97:
        {
            al = 0;
            ip++;
            printf("SUB A,A\n");
            break;
        }
        case 0x98:
        {
            al -= ch + (flags & 1);
            ip++;
            printf("SBB A,B\n");
            break;
        }
        case 0x99:
        {
            al -= cl + (flags & 1);
            ip++;
            printf("SBB A,C\n");
            break;
        }
        case 0x9A:
        {
            al -= dh + (flags & 1);
            ip++;
            printf("SBB A,D\n");
            break;
        }
        case 0x9B:
        {
            al -= dl + (flags & 1);
            ip++;
            printf("SBB A,E\n");
            break;
        }
        case 0x9C:
        {
            al -= bh + (flags & 1);
            ip++;
            printf("SBB A,H\n");
            break;
        }
        case 0x9D:
        {
            al -= bl + (flags & 1);
            ip++;
            printf("SBB A,L\n");
            break;
        }
        case 0x9E:
        {
            al -= RAM::rb(cs,bx) + (flags & 1);
            ip++;
            printf("SBB A,(HL)\n");
            break;
        }
        case 0x9F:
        {
            al -= al + (flags & 1);
            ip++;
            printf("SBB A,A\n");
            break;
        }
        case 0xA0:
        {
            al &= ch;
            ip++;
            printf("AND A,B\n");
            break;
        }
        case 0xA1:
        {
            al &= cl;
            ip++;
            printf("AND A,C\n");
            break;
        }
        case 0xA2:
        {
            al &= dh;
            ip++;
            printf("AND A,D\n");
            break;
        }
        case 0xA3:
        {
            al &= dl;
            ip++;
            printf("AND A,E\n");
            break;
        }
        case 0xA4:
        {
            al &= bh;
            ip++;
            printf("AND A,H\n");
            break;
        }
        case 0xA5:
        {
            al &= bl;
            ip++;
            printf("AND A,L\n");
            break;
        }
        case 0xA6:
        {
            al &= RAM::rb(cs,bx);
            ip++;
            printf("AND A,(HL)\n");
            break;
        }
        case 0xA7:
        {
            ip++;
            printf("AND A,A\n");
            break;
        }
        case 0xA8:
        {
            al ^= ch;
            ip++;
            printf("XOR A,B\n");
            break;
        }
        case 0xA9:
        {
            al ^= cl;
            ip++;
            printf("XOR A,C\n");
            break;
        }
        case 0xAA:
        {
            al ^= dh;
            ip++;
            printf("XOR A,D\n");
            break;
        }
        case 0xAB:
        {
            al ^= dl;
            ip++;
            printf("XOR A,E\n");
            break;
        }
        case 0xAC:
        {
            al ^= bh;
            ip++;
            printf("XOR A,H\n");
            break;
        }
        case 0xAD:
        {
            al ^= bl;
            ip++;
            printf("XOR A,L\n");
            break;
        }
        case 0xAE:
        {
            al ^= RAM::rb(cs,bx);
            ip++;
            printf("XOR A,(HL)\n");
            break;
        }
        case 0xAF:
        {
            al = 0;
            ip++;
            printf("XOR A,A\n");
            break;
        }
        case 0xB0:
        {
            al |= ch;
            ip++;
            printf("OR A,B\n");
            break;
        }
        case 0xB1:
        {
            al |= cl;
            ip++;
            printf("OR A,C\n");
            break;
        }
        case 0xB2:
        {
            al |= dh;
            ip++;
            printf("OR A,D\n");
            break;
        }
        case 0xB3:
        {
            al |= dl;
            ip++;
            printf("OR A,E\n");
            break;
        }
        case 0xB4:
        {
            al |= bh;
            ip++;
            printf("OR A,H\n");
            break;
        }
        case 0xB5:
        {
            al |= bl;
            ip++;
            printf("OR A,L\n");
            break;
        }
        case 0xB6:
        {
            al |= RAM::rb(cs,bx);
            ip++;
            printf("OR A,(HL)\n");
            break;
        }
        case 0xB7:
        {
            ip++;
            printf("OR A,A\n");
            break;
        }
        case 0xB8:
        {
            u8 tmp = al - ch;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            if(tmp >= 0x80) flags |= 0x0080;
            else flags &= 0xFF7F;
            ip++;
            printf("CMP A,B\n");
            break;
        }
        case 0xB9:
        {
            u8 tmp = al - cl;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            if(tmp >= 0x80) flags |= 0x0080;
            else flags &= 0xFF7F;
            ip++;
            printf("CMP A,C\n");
            break;
        }
        case 0xBA:
        {
            u8 tmp = al - dh;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            if(tmp >= 0x80) flags |= 0x0080;
            else flags &= 0xFF7F;
            ip++;
            printf("CMP A,D\n");
            break;
        }
        case 0xBB:
        {
            u8 tmp = al - dl;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            if(tmp >= 0x80) flags |= 0x0080;
            else flags &= 0xFF7F;
            ip++;
            printf("CMP A,E\n");
            break;
        }
        case 0xBC:
        {
            u8 tmp = al - bh;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            if(tmp >= 0x80) flags |= 0x0080;
            else flags &= 0xFF7F;
            ip++;
            printf("CMP A,H\n");
            break;
        }
        case 0xBD:
        {
            u8 tmp = al - bl;
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            if(tmp >= 0x80) flags |= 0x0080;
            else flags &= 0xFF7F;
            ip++;
            printf("CMP A,L\n");
            break;
        }
        case 0xBE:
        {
            u8 tmp = al - RAM::rb(cs,bx);
            if(tmp == 0) flags |= 0x0040;
            else flags &= 0xFFBF;
            if(tmp >= 0x80) flags |= 0x0080;
            else flags &= 0xFF7F;
            ip++;
            printf("CMP A,(HL)\n");
            break;
        }
        case 0xBF:
        {
            al = 0;
            flags |= 0x0040;
            flags &= 0xFF7F;
            ip++;
            printf("CMP A,A\n");
            break;
        }
        case 0xC0:
        {
            if(!(flags&0x0040))
            {
                ip = RAM::rb(cs,sp)|(RAM::rb(cs,sp+1)<<8);
                sp+=2;
            }
            else ip++;
            printf("RET NZ\n");
            break;
        }
        case 0xC8:
        {
            if((flags&0x0040))
            {
                ip = RAM::rb(cs,sp)|(RAM::rb(cs,sp+1)<<8);
                sp+=2;
            }
            else ip++;
            printf("RET Z\n");
            break;
        }
        case 0xC9:
        {
            ip = RAM::rb(cs,sp)|(RAM::rb(cs,sp+1)<<8);
            sp+=2;
            printf("RET\n");
            break;
        }
        }
    }
    printf("ax=%04x\n",ax);
    printf("bx=%04x\n",bx);
    printf("cx=%04x\n",cx);
    printf("dx=%04x\n",dx);
    printf("cs=%04x\n",cs);
    printf("ip=%04x\n",ip);
    printf("ds=%04x\n",ds);
    printf("es=%04x\n",es);
    printf("ss=%04x\n",ss);
    printf("si=%04x\n",si);
    printf("di=%04x\n",di);
    printf("sp=%04x\n",sp);
    printf("bp=%04x\n",bp);
    printf("flags=%04x\n",flags);
}

void tick()
{
    if(!halted)
    {
        u8 op = RAM::rb(cs,ip);

        switch(op)
        {
        case 0x26:
        {
            ip++;
            seg = SEG_ES;
            rep = 0;
            break;
        }
        case 0x2E:
        {
            ip++;
            seg = SEG_CS;
            rep = 0;
            break;
        }
        case 0x36:
        {
            ip++;
            seg = SEG_SS;
            rep = 0;
            break;
        }
        case 0x3E:
        {
            ip++;
            seg = SEG_DS;
            rep = 0;
            break;
        }
        case 0xF0:
        {
            ip++;
            seg = SEG_DEFAULT;
            rep = 0;
            break;
        }
        case 0xF2:
        {
            ip++;
            seg = SEG_DEFAULT;
            rep = 2;
            break;
        }
        case 0xF3:
        {
            ip++;
            seg = SEG_DEFAULT;
            rep = 1;
            break;
        }
        case 0xF4:
        {
            ip++;
            printf("HLT\n");
            halted=true;
        }
        default:
            seg = SEG_DEFAULT;
            rep = 0;
        }
        if(type==necv20)
        {
            if(op == 0x65)
            {
                rep = 3;
            }
            if(op == 0x64)
            {
                rep = 4;
            }
        }
        rtick();
    }
}

bool halted = false;
u32 cr0;

bool hint = false; //Hardware interrupts.
u8 hintnum = 0;

} //namespace CPU
