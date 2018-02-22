#ifndef KERNELSE_H_
#define KERNELSE_H_
#include "FifoQ.h"
#include "SortedQ.h"
#include "PCB.h";
typedef unsigned int Time;

class FifoQ;
class SortedQ;

class KernelSem {
public:
	KernelSem(int init=1);
	~KernelSem();

	int wait(Time maxTimeToWait);
	void signal();

	int val();

	static SortedQ* waitList;

	int value;
	FifoQ* listOfBlockedPCB;
};

#endif
