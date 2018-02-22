#ifndef IVTENTRY_H_
#define IVTENTRY_H_

class KernelEv;
class Event;
class IVTEntry;

extern IVTEntry *table[256];


typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:
	KernelEv* myEvent;
	pInterrupt oldRoutine;
	pInterrupt newRoutine;

	IVTEntry(IVTNo IVTno, pInterrupt newRout);
	~IVTEntry();

	static IVTEntry* getIVTEntry(IVTNo ivtNo);


private:
	IVTNo ivtNo;
    void init();
	void rest();



};

#endif
