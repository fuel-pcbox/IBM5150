#include "savestate.h"

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
