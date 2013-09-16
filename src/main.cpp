#include <cstdio>
#include <vector>
#include <string>
#include <functional>
#include <chrono>
#include <thread>

#include "interface.h"
#include "misc.h"

enum
{
    update_frame = 1,
    update_scanline = 2,
    update_pixel = 4,
    update_clock = 8
};

#include "cpu.h"
#include "mda.h"
#include "cga.h"
#include "savestate.h"

int main(int ac, char** av)
{
    if(ac < 2)
    {
        printf("Usage:\n\tibm5150 configfile\n");
        return 1;
    }

    PIC::pic[0].init1 = false;
    PIC::pic[0].init2 = false;
    PIC::pic[0].enabled = false;
    DMA_XT::chan[0].access_flip_flop = false;
    CPU::cr0 = 0;

    char* isa1 = new char[10];
    char* biosrom = new char[256];
    char* mdarom = new char[256];

    FILE* config = fopen(av[1],"r");
    fscanf(config,"isa1=%s\n",isa1);
    fscanf(config,"biosrom=%s\n",biosrom);
    fscanf(config,"mdarom=%s\n",mdarom);
    fclose(config);
    
    FILE* bios = fopen(biosrom,"rb");
    fseek(bios,0,SEEK_END);
    long size = ftell(bios);
    fseek(bios,0,SEEK_SET);
    fread(RAM::RAM + (0x100000 - size),1,size,bios);
    
    FILE* mda = fopen(mdarom,"rb");
    fread(MDA::ROM,1,0x2000,mda);
    fseek(mda,0,SEEK_SET);
    fread(CGA::ROM,1,0x2000,mda);
    fclose(mda);
    fclose(bios);

    std::string isa1slot = isa1;
    delete[] isa1;

    INTERFACE::init();

    if(isa1slot == "mda")
    {
        IO_XT::handlers.push_back(MDA::mdacrtc);
        INTERFACE::window_caption("IBM5150:  CPU: 8086 SYSTEM: IBM PC 5150 ISA1: MDA");
    }
    if(isa1slot == "cga")
    {
        IO_XT::handlers.push_back(CGA::cgacrtc);
        INTERFACE::window_caption("IBM5150:  CPU: 8086 SYSTEM: IBM PC 5150 ISA1: CGA");
    }

    IO_XT::handlers.push_back(DMA_XT::handler);
    IO_XT::handlers.push_back(DMA_XT::handler2);
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
    
    fp = fopen("save/mda.dump","rb");
    if(fp != NULL)
    {
        fread(&MDA::hdisp,1,1,fp);
        fread(&MDA::vdisp,1,1,fp);
        fread(&MDA::maxscan,1,1,fp);
        fread(&MDA::dispmode,1,1,fp); 
        fclose(fp);
    }
    
    fp = fopen("save/cga.dump","rb");
    if(fp != NULL)
    {
        fread(&CGA::hdisp,1,1,fp);
        fread(&CGA::vdisp,1,1,fp);
        fread(&CGA::maxscan,1,1,fp);
        fread(&CGA::dispmode,1,1,fp); 
        fclose(fp);
    }
    
    fp = fopen("save/pic.dump","rb");
    if(fp != NULL)
    {
        fread(&PIC::pic[0].intrmask,1,1,fp);
        fread(&PIC::pic[0].offset,1,1,fp);
        fread(&PIC::pic[0].enabled,sizeof(bool),1,fp);
        fclose(fp);
    }
    
    bool debugsaved = false;
    
    std::thread pitthread([]()
    {
        PIT::tick();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    });

    while(INTERFACE::quitflag == false)
    { 
        if(i==100)
        {
            i = 0;
            if(isa1slot == "mda") MDA::tick_frame();
            if(isa1slot == "cga") CGA::tick_frame();
            INTERFACE::update_screen();
        }


        //TODO: remove SDL_* prefix
        INTERFACE::handle_events();
        
        CPU::tick();
        
        if(CPU::hint == true) CPU::hint = false;
        
        i++; 
    }
    
    pitthread.join();

    INTERFACE::quit();

    return 0;
}
