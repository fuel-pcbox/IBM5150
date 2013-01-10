namespace CPU
{
enum cputype
{
    intel8086 = 0,
    intel286,
    intel386
}

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
union reg16
{
    struct
    {
        u8 lo,hi;
    } parts;
    u16 w;
} a,b,c,d;
#define ax a.w
#define al a.parts.lo
#define ah a.parts.hi
#define bx b.w
#define bl b.parts.lo
#define bh b.parts.hi
#define cx c.w
#define cl c.parts.lo
#define ch c.parts.hi

#define dx d.w
#define dl d.parts.lo
#define dh d.parts.hi

u16 ds,es,ss;
u16 sp,bp,si,di;
u16 cs = 0xF000;
u16 ip = 0xFFF0;
u16 flags = 0xF002;

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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_SS:
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
                case SEG_DEFAULT: case SEG_SS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_SS:
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
                case SEG_DEFAULT: case SEG_SS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_SS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_SS:
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
                case SEG_DEFAULT: case SEG_SS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_DS:
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
                case SEG_DEFAULT: case SEG_SS:
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
                case SEG_DEFAULT: case SEG_DS:
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
    return res;
}
int seg = SEG_DEFAULT;
void rtick()
{
    u8 op = RAM::rb(cs,ip);
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
        RAM::wb(ss,sp) = es & 0xFF;
        RAM::wb(ss,sp+1) = es >> 8;
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
        RAM::wb(ss,sp) = cs & 0xFF;
        RAM::wb(ss,sp+1) = cs >> 8;
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
        RAM::wb(ss,sp) = ss & 0xFF;
        RAM::wb(ss,sp+1) = ss >> 8;
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
        RAM::wb(ss,sp) = ds & 0xFF;
        RAM::wb(ss,sp+1) = ds >> 8;
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
        *loc.src8 &= *loc.dst8;
        u8 tmp = *loc.src8;
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
        *loc.dst8 &= *loc.dst8;
        u8 tmp = *loc.dst8;
        if(tmp == 0) flags |= 0x0040;
        else flags &= 0xFFBF;
        ip+=2;
        printf("AND Eb,Gb modrm=%02x\n",modrm);
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
		printf("INC AX\n");
		ax++;
		if(ax == 0) flags |= 0x0040;
		else flags &= 0xFFBF;
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
		printf("INC CX\n");
		cx++;
		if(cx == 0) flags |= 0x0040;
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
    case 0x8B:
    {
        u8 modrm = RAM::rb(cs,ip+1);
        locs loc = decodemodrm(seg,modrm,true,false);
        *loc.src16 = *loc.dst16;
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
    case 0x8E:
    {
        u8 modrm = RAM::rb(cs,ip+1);
        locs loc = decodemodrm(seg,modrm,true,true);
        *loc.dst16 = *loc.src16;
        ip+=2;
        printf("MOV Sw,Ew modrm=%02x\n",modrm);
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
    case 0xB4:
    {
        ah = RAM::rb(cs,ip+1);
        printf("MOV AH,%02x\n",ah);
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
    case 0xD0:
    {
        u8 op2 = RAM::rb(cs,ip+1);
        switch(op2&0xC0)
        {
        case 0xC0:
        {
            switch(op2&0x38)
            {
            case 0x20:
            {
                switch(op2&0x07)
                {
                case 0x00:
                {
                    u8 tmp = al;
                    printf("SHL AL,1\n");
                    al <<= 1;
                    if(tmp&0x80) flags |= 0x0001;
                    else flags &= 0xFFFE;
                    if(al&0x80) flags |= 0x0800;
                    else flags &= 0xF7FF;
                    if(al==0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    break;
                }
                case 0x04:
                {
                    u8 tmp = ah;
                    printf("SHL AH,1\n");
                    ah <<= 1;
                    if(tmp&0x80) flags |= 0x0001;
                    else flags &= 0xFFFE;
                    if(ah&0x80) flags |= 0x0800;
                    else flags &= 0xF7FF;
                    if(ah==0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    break;
                }
                }
                break;
            }
            }
            break;
        }
        }
        ip+=2;
        break;
    }
    case 0xD2:
    {
        u8 op2 = RAM::rb(cs,ip+1);
        switch(op2&0xC0)
        {
        case 0xC0:
        {
            switch(op2&0x38)
            {
            case 0x28:
            {
                switch(op2&0x07)
                {
                case 0x04:
                {
                    u8 tmp = ah;
                    printf("SHR AH,CL\n");
                    ah >>= cl;
                    if(tmp&1) flags |= 0x0001;
                    else flags &= 0xFFFE;
                    if(ah==0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    break;
                }
                }
                break;
            }
            }
            break;
        }
        }
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
    case 0xEE:
    {
        printf("OUT DX,AL\n");
        IO_XT::wb(dx,al);
        ip++;
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
    case 0xFE:
    {
        u8 op2 = RAM::rb(cs,ip+1);
        switch(op2&0xC0)
        {
        case 0xC0:
        {
            switch(op2&0x38)
            {
            case 0x00:
            {
                switch(op2&0x07)
                {
                case 0x00:
                {
                    printf("INC AL\n");
                    al++;
                    if(al==0) flags |= 0x0040;
                    else flags &= 0xFFBF;
                    break;
                }
                }
                break;
            }
            }
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
    u8 op = RAM::rb(cs,ip+1);
    
    switch(op)
    {
        case 0x26:
        {
            ip++;
            seg = SEG_ES;
            break;
        }
        case 0x2E:
        {
            ip++;
            seg = SEG_CS;
            break;
        }
        case 0x36:
        {
            ip++;
            seg = SEG_SS;
            break;
        }
        case 0x3E:
        {
            ip++;
            seg = SEG_DS;
            break;
        }
        defaukt: seg = SEG_DEFAULT;
    }
    rtick();
}

};
