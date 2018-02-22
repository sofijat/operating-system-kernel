#include "event.h"
#include "FifoQ.h"
#include "SortedQ.h"
#include "Timer.h"
#include "PCB.h"
#include "IdleThr.h"
#include "IVTEntry.h"
#include "semaphor.h"
#include <iostream.h>

int result;

extern int userMain (int argc, char* argv[]);


int main(int argc, char *argv[]){
	PCB::running=new PCB();
	PCB::idle->getPCB()->start();

	inic();
	result= userMain(argc, argv);
	restore();

	delete PCB::idle;
	delete PCB::running;
	delete KernelSem::waitList;
	delete PCB::listOfAllPCB;
	PCB::listOfAllPCB=0;

	return result;

}
