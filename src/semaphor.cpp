#include "semaphor.h"


#define lock
#ifndef BCC_BLOCK_IGNORE
asm { pushf; cli; }
#endif

#define unlock
#ifndef BCC_BLOCK_IGNORE
asm { popf; }
#endif

Semaphore::Semaphore(int init){
	lock
	myImpl=new KernelSem(init);
	unlock
}

Semaphore::~Semaphore(){
	lock
	delete myImpl;
	unlock
}

int Semaphore::wait(Time maxTimeToWait){
	lock
	int v=myImpl->wait(maxTimeToWait);
	unlock
	return v;

}

void Semaphore::signal(){
	lock
	myImpl->signal();
	unlock
}

int Semaphore::val() const{
	return myImpl->val();
}
