#ifndef KERNELEV_H_
#define KERNELEV_H_


class IVTEntry;
class Event;
class PCB;
typedef unsigned char IVTNo;

class KernelEv{
private:
	IVTEntry *myEntry;
	Event *event;
	PCB *owner;
    IVTNo ivtNum;
	int value;

public:
	KernelEv(Event* ev, IVTNo ivtNo);
	~KernelEv();

	void signal();
	void wait();


};
#endif
