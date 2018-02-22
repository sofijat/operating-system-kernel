#ifndef IDLETHR_H_
#define IDLETHR_H_
#include "PCB.h"
#include "thread.h"
#include <IOSTREAM.H>

class IdleThread: public Thread {
public:
	IdleThread();
	virtual void run();
	friend class PCB;
	PCB* getPCB();

};

#endif /* IDLETHR_H_ */
