#include "thread.h"
#include "PCB.h"
#include <IOSTREAM.H>

extern volatile int switchRequested;

#define lock
#ifndef BCC_BLOCK_IGNORE
asm { pushf; cli; }
#endif

#define unlock
#ifndef BCC_BLOCK_IGNORE
asm { popf; }
#endif


void dispatch(){
lock
	switchRequested=1;
#ifndef BCC_BLOCK_IGNORE
    asm int 8h;
#endif
unlock

}

void Thread::start(){
	lock
	myPCB->start();
	unlock
}
void Thread::waitToComplete(){
	lock
	myPCB->waitToComplete();
	unlock
}

Thread::~Thread(){
	waitToComplete();
	lock
	delete myPCB;
	unlock
}

Thread::Thread(StackSize stackSize , Time timeSlice){
	lock
	myPCB=new PCB(this, stackSize, timeSlice);
	unlock

}
ID Thread::getId() {
	return myPCB->getId();
}

ID Thread::getRunningId() {
	return PCB::running->getId();

}

Thread *Thread::getThreadById(ID id) {
	return PCB::listOfAllPCB->find(id);
}



