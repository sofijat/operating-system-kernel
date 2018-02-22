#include "KernelEv.h"
#include "PCB.h"
#include "IVTEntry.h"
#include "IdleThr.h"

#define lock
#ifndef BCC_BLOCK_IGNORE
asm { pushf; cli; }
#endif

#define unlock
#ifndef BCC_BLOCK_IGNORE
asm { popf; }
#endif


KernelEv::KernelEv(Event* ev, IVTNo ivtNo): event(ev), ivtNum(ivtNo){
	owner=PCB::running;
	myEntry=IVTEntry::getIVTEntry(ivtNum);
	value=0;
	table[ivtNo]->myEvent=this;
}

KernelEv::~KernelEv(){
	event=0;
    owner=0;
    myEntry=0;
	table[ivtNum]->myEvent=0;
}

void KernelEv::wait(){
	lock
	if (owner==PCB::running && value==0){
		PCB::running->state=PCB::BLOCKED;
		value=1;
		unlock
		dispatch();
	}
	else {
		unlock
	}
}

void KernelEv::signal(){
	lock
	if (value==1) {
		PCB::running->state=PCB::READY;
		value=0;
		if (owner!=PCB::idle->getPCB()) Scheduler::put(owner);
	}
	unlock
}

