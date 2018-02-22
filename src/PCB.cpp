#include "PCB.h"
#include "IdleThr.h"

#define lock
#ifndef BCC_BLOCK_IGNORE
asm { pushf; cli; }
#endif

#define unlock
#ifndef BCC_BLOCK_IGNORE
asm { popf; }
#endif


ID PCB::globalId=0;
PCB* PCB::running = 0;
IdleThread* PCB::idle=new IdleThread();
FifoQ* PCB::listOfAllPCB=new FifoQ();

PCB::PCB(Thread* t, StackSize stSize, Time tiSlice): state(NEW), timeSlice(tiSlice), stack(0), myThread(t), mySemaphore(0) {
	lock
	if(stSize>MAX_STACK_SIZE) stackSize=MAX_STACK_SIZE;
	else stackSize=stSize;
	waitingForMe=new FifoQ();
	id=++globalId;
	ss=0;  //nisam sigurna sme li ovo da se stavlja u konstruktor
	sp=0;
	bp=0;
	waitValue=0;
    listOfAllPCB->put(this);
    unlock

}

PCB::PCB(){
	lock
	state=NEW;
	stack=0;
	stackSize=0;
	id=++globalId;
	waitingForMe=new FifoQ();
	timeSlice=defaultTimeSlice;
	myThread=0;
	mySemaphore=0;
	ss=0; sp=0; bp=0;
	waitValue=0;
	listOfAllPCB->put(this);
	unlock

}


void PCB::wrapper(){
	PCB::running->myThread->run();
	PCB::running->wakeThemUp();
}

void PCB::wakeThemUp(){
	state=DONE;
	int num=waitingForMe->number();
	if (num!=0){
		for (int i=num; i>0; i--){
			PCB* help=waitingForMe->get();
			help->state=READY;
			if (help!=0 && help!=idle->myPCB) Scheduler::put(help);
		}
	}
	dispatch();
}

void PCB::start(){
	if (state==NEW){
		this->state=READY;
		initStack();
	    if (this!=idle->myPCB)  Scheduler::put(this);
		}
}

PCB::~PCB(){
	lock
	PCB::listOfAllPCB->removeById(id);
	mySemaphore=0;
    delete waitingForMe;
    delete [] stack;
    unlock
}


void PCB::initStack(){
	unsigned off=stackSize/sizeof(unsigned);
	stack = new unsigned [off];
	stack[off - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[off - 2] = FP_SEG(wrapper);
	stack[off - 3] = FP_OFF(wrapper);

	sp = FP_OFF(stack + off - 12);
	ss = FP_SEG(stack + off - 12);
	bp = FP_OFF(stack + off - 12);
#endif

}


ID PCB::getId(){
	return id;
}


void PCB::waitToComplete(){
	if (this!=running && this!=idle->myPCB && state!=DONE && running!=0){
		running->state=BLOCKED;
		waitingForMe->put(running);
		dispatch();
	}
}


