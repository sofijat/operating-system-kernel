#include "KernelSe.h"
#include "IdleThr.h"

SortedQ* KernelSem::waitList=new SortedQ();


#define lock
#ifndef BCC_BLOCK_IGNORE
asm { pushf; cli; }
#endif

#define unlock
#ifndef BCC_BLOCK_IGNORE
asm { popf; }
#endif

KernelSem::KernelSem(int init): value(init){
	lock
	listOfBlockedPCB=new FifoQ();
	unlock
}

KernelSem::~KernelSem(){
	lock
	delete listOfBlockedPCB;
	unlock
}

int KernelSem::wait(Time maxTimeToWait){
	lock
	if (--value<0 && PCB::running->state!=PCB::BLOCKED && PCB::running->state!=PCB::DONE){
		PCB::running->mySemaphore=this;
		PCB::running->state=PCB::BLOCKED;
		listOfBlockedPCB->put(PCB::running);
		if (maxTimeToWait>0){
		  waitList->put(PCB::running, maxTimeToWait);
		}
		unlock
		dispatch();
	}
	else {
		PCB::running->waitValue=2;
		unlock
	}
	return PCB::running->waitValue;
}

void KernelSem::signal(){
	lock
	if (value++<0){
	        PCB* pcb=listOfBlockedPCB->get();
			if (pcb!=0){
				waitList->removeById(pcb->getId());
				pcb->state=PCB::READY;
				pcb->mySemaphore=0;
				pcb->waitValue=1;
				if (pcb!=PCB::idle->getPCB()) {Scheduler::put(pcb);}
			}
		}
	unlock
}

int KernelSem::val(){
	return value;
}

