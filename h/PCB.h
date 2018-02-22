#ifndef PCB_H_
#define PCB_H_
#include "thread.h"
#include "KernelSe.h"
#include "SCHEDULE.H"
#include "FifoQ.h"
#include <dos.h>

class FifoQ;
class KernelSem;
class Thread;
const StackSize MAX_STACK_SIZE=65536;

class PCB {

public:
	enum State { NEW, READY, BLOCKED, DONE };

	static PCB* running;
	static ID globalId;
	static IdleThread* idle;
	ID id;

	Time timeSlice;
	State state;
	int waitValue;

	unsigned ss,sp,bp;
	unsigned *stack;
	Thread* myThread;
	KernelSem* mySemaphore;

	PCB( Thread* t, StackSize stSize, Time tiSlice);
	PCB();
	//PCB(Time tiSlce, void (*body));
    ~PCB();

	void start();
	void waitToComplete();
	ID getId();

	static void wrapper();
    void initStack();

	FifoQ *waitingForMe;
    static FifoQ* listOfAllPCB;


private:
	StackSize stackSize;
	void wakeThemUp();


};

#endif
