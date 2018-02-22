#include "IVTEntry.h"
#include <dos.h>

IVTEntry *table[256];



#define lock
#ifndef BCC_BLOCK_IGNORE
asm { pushf; cli; }
#endif

#define unlock
#ifndef BCC_BLOCK_IGNORE
asm { popf; }
#endif

void IVTEntry::init(){
#ifndef BCC_BLOCK_IGNORE
	oldRoutine=getvect(ivtNo);
	setvect(ivtNo, newRoutine);
#endif
}

void IVTEntry::rest(){
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRoutine);
#endif
}

IVTEntry::IVTEntry(IVTNo IVTno, pInterrupt newRout): ivtNo(IVTno){
	lock
    newRoutine=newRout;
    table[ivtNo]=this;
    init();
    unlock
}

IVTEntry::~IVTEntry(){
	lock
    table[ivtNo]=0;
    rest();
    unlock
}

IVTEntry* IVTEntry::getIVTEntry(IVTNo ivt){
	return table[ivt];
}


