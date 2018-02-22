#ifndef _event_h_
#define _event_h_

#include "KernelEv.h"
#include "IVTEntry.h"

typedef unsigned char IVTNo;
class KernelEv;

class Event{
public:
	Event (IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;
};

#define PREPAREENTRY(ivtNo,callOld)\
void interrupt interr_##ivtNo(...);\
IVTEntry entry##ivtNo(ivtNo,interr_##ivtNo);\
void interrupt interr_##ivtNo(...){\
if(callOld) table[ivtNo]->oldRoutine();\
table[ivtNo]->myEvent->signal();\
dispatch();\
}

#endif
