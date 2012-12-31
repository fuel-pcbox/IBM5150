#include <cstdint>
#include <cstdio>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;

namespace RAM
{
	u8 RAM[0x100000];
	u8 rb(u16 seg, u16 off)
	{
		return RAM[(seg<<4)+off];
	}
};

namespace CPU
{
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
		switch(modrm)
		{
			case 0xC0:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &al;
						res.dst8 = &al;
						break;
					}
					case 1:
					{
						res.src16 = &ax;
						res.dst16 = &ax;
						break;
					}
					//This case skip may look incorrect, but it's here because segment registers are 16-bits. How are you going to move 8-bits into 16-bits?
					case 3:
					{
						res.src16 = &ax;
						res.dst16 = &es;
						break;
					}
				}
				break;
			}
			case 0xC1:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &cl;
						res.dst8 = &al;
						break;
					}
					case 1:
					{
						res.src16 = &cx;
						res.dst16 = &ax;
						break;
					}
					case 3:
					{
						res.src16 = &cx;
						res.dst16 = &es;
						break;
					}
				}
				break;
			}
			case 0xC2:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &dl;
						res.dst8 = &al;
						break;
					}
					case 1:
					{
						res.src16 = &dx;
						res.dst16 = &ax;
						break;
					}
					case 3:
					{
						res.src16 = &dx;
						res.dst16 = &es;
						break;
					}
				}
				break;
			}
			case 0xC3:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &bl;
						res.dst8 = &al;
						break;
					}
					case 1:
					{
						res.src16 = &bx;
						res.dst16 = &ax;
						break;
					}
					case 3:
					{
						res.src16 = &bx;
						res.dst16 = &es;
						break;
					}
				}
				break;
			}
			case 0xC4:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &ah;
						res.dst8 = &al;
						break;
					}
					case 1:
					{
						res.src16 = &sp;
						res.dst16 = &ax;
						break;
					}
					case 3:
					{
						res.src16 = &sp;
						res.dst16 = &es;
						break;
					}
				}
				break;
			}
			case 0xC5:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &ch;
						res.dst8 = &al;
						break;
					}
					case 1:
					{
						res.src16 = &bp;
						res.dst16 = &ax;
						break;
					}
					case 3:
					{
						res.src16 = &bp;
						res.dst16 = &es;
						break;
					}
				}
				break;
			}
			case 0xC6:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &dh;
						res.dst8 = &al;
						break;
					}
					case 1:
					{
						res.src16 = &si;
						res.dst16 = &ax;
						break;
					}
					case 3:
					{
						res.src16 = &si;
						res.dst16 = &es;
						break;
					}
				}
				break;
			}
			case 0xC7:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &bh;
						res.dst8 = &al;
						break;
					}
					case 1:
					{
						res.src16 = &di;
						res.dst16 = &ax;
						break;
					}
					case 3:
					{
						res.src16 = &di;
						res.dst16 = &es;
						break;
					}
				}
				break;
			}
			case 0xC8:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &al;
						res.dst8 = &cl;
						break;
					}
					case 1:
					{
						res.src16 = &ax;
						res.dst16 = &cx;
						break;
					}
					//This case skip may look incorrect, but it's here because segment registers are 16-bits. How are you going to move 8-bits into 16-bits?
					case 3:
					{
						res.src16 = &ax;
						res.dst16 = &cs;
						break;
					}
				}
				break;
			}
			case 0xC9:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &cl;
						res.dst8 = &cl;
						break;
					}
					case 1:
					{
						res.src16 = &cx;
						res.dst16 = &cx;
						break;
					}
					case 3:
					{
						res.src16 = &cx;
						res.dst16 = &cs;
						break;
					}
				}
				break;
			}
			case 0xCA:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &dl;
						res.dst8 = &cl;
						break;
					}
					case 1:
					{
						res.src16 = &dx;
						res.dst16 = &cx;
						break;
					}
					case 3:
					{
						res.src16 = &dx;
						res.dst16 = &cs;
						break;
					}
				}
				break;
			}
			case 0xCB:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &bl;
						res.dst8 = &cl;
						break;
					}
					case 1:
					{
						res.src16 = &bx;
						res.dst16 = &cx;
						break;
					}
					case 3:
					{
						res.src16 = &bx;
						res.dst16 = &cs;
						break;
					}
				}
				break;
			}
			case 0xCC:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &ah;
						res.dst8 = &cl;
						break;
					}
					case 1:
					{
						res.src16 = &sp;
						res.dst16 = &cx;
						break;
					}
					case 3:
					{
						res.src16 = &sp;
						res.dst16 = &cs;
						break;
					}
				}
				break;
			}
			case 0xCD:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &ch;
						res.dst8 = &cl;
						break;
					}
					case 1:
					{
						res.src16 = &bp;
						res.dst16 = &cx;
						break;
					}
					case 3:
					{
						res.src16 = &bp;
						res.dst16 = &cs;
						break;
					}
				}
				break;
			}
			case 0xCE:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &dh;
						res.dst8 = &cl;
						break;
					}
					case 1:
					{
						res.src16 = &si;
						res.dst16 = &cx;
						break;
					}
					case 3:
					{
						res.src16 = &si;
						res.dst16 = &cs;
						break;
					}
				}
				break;
			}
			case 0xCF:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &bh;
						res.dst8 = &cl;
						break;
					}
					case 1:
					{
						res.src16 = &di;
						res.dst16 = &cx;
						break;
					}
					case 3:
					{
						res.src16 = &di;
						res.dst16 = &cs;
						break;
					}
				}
				break;
			}
			case 0xE4:
			{
				switch(tmp)
				{
					case 0:
					{
						res.src8 = &ah;
						res.dst8 = &ah;
						break;
					}
					case 1:
					{
						res.src16 = &sp;
						res.dst16 = &sp;
						break;
					}
					case 3:
					{
						res.src16 = &sp;
						res.dst16 = &es;
						break;
					}
				}
				break;
			}
		}
		return res;
	}
	void tick()
	{
		u8 op = RAM::rb(cs,ip);
		switch(op)
		{
			case 0x32:
			{
				u8 modrm = RAM::rb(cs,ip+1);
				locs loc = decodemodrm(SEG_DEFAULT,modrm,false,false);
				*loc.src8 ^= *loc.dst8;
				u8 tmp = *loc.src8;
				if(tmp == 0) flags |= 0x0040;
				else flags &= 0xFFBF;
				ip+=2;
				printf("XOR Gb,Eb modrm=%02x\n",modrm);
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
			case 0xEA:
			{
				u16 tmp = cs;
				u16 tmp1 = ip;
				cs = (RAM::rb(tmp,tmp1+4)<<8)|RAM::rb(tmp,tmp1+3);
				ip = (RAM::rb(tmp,tmp1+2)<<8)|RAM::rb(tmp,tmp1+1);
				printf("JMP %04x:%04x\n",cs,ip);
				break;
			}
			case 0xFA:
			{
				printf("CLI\n");
				flags &= 0xFDFF;
				ip++;
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
};

int main()
{
	FILE* bios = fopen("pcxt.rom","rb");
	fseek(bios,0,SEEK_END);
	long size = ftell(bios);
	fseek(bios,0,SEEK_SET);
	fread(RAM::RAM + (0x100000 - size),1,size,bios);
	fclose(bios);
	for(int i = 0;i<40;i++)
	{
		CPU::tick();
	} 
}