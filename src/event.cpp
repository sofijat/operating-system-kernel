#include "event.h"

#define lock
#ifndef BCC_BLOCK_IGNORE
asm { pushf; cli; }
#endif

#define unlock
#ifndef BCC_BLOCK_IGNORE
asm { popf; }
#endif

Event::Event(IVTNo ivtNo){
	lock
	myImpl=new KernelEv(this, ivtNo);
	unlock
}

Event::~Event(){
	lock
	delete myImpl;
	unlock

}

void Event::wait(){
	lock
	myImpl->wait();
	unlock
}

void Event::signal(){
	lock
	myImpl->signal();
	unlock
}

