#include "IdleThr.h"

IdleThread::IdleThread(): Thread(1024, 1) {}

void IdleThread::run(){
	while(1);
}

PCB* IdleThread::getPCB(){
	return myPCB;
}
