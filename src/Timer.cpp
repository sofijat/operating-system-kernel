#include "Timer.h"
#include "IdleThr.h"

volatile unsigned int cnt=30;
volatile int switchRequested=0;
extern void tick();

unsigned tsp, tss, tbp;

void interrupt timer(){
	if (switchRequested == 0) {
	cnt--;
	tick();
    KernelSem::waitList->update();
}
if (cnt == 0 || switchRequested != 0) {

		#ifndef BCC_BLOCK_IGNORE
				asm mov tsp, sp
				asm	mov tss, ss
				asm	mov tbp, bp
		#endif
				PCB::running->sp = tsp;
				PCB::running->ss = tss;
				PCB::running->bp = tbp;


				if(switchRequested == 1 || cnt == 0)
					if (PCB::running != PCB::idle->getPCB() && PCB::running->state != PCB::BLOCKED && PCB::running->state != PCB::DONE)
						Scheduler::put(PCB::running);

				PCB::running = Scheduler::get();
				if (!PCB::running)
					PCB::running = PCB::idle->getPCB();

				tsp = PCB::running->sp;
				tss = PCB::running->ss;
				tbp = PCB::running->bp;

				cnt = PCB::running->timeSlice;

		#ifndef BCC_BLOCK_IGNORE
				asm	mov sp, tsp
				asm	mov ss, tss
				asm	mov bp, tbp
		#endif
			}

			if(switchRequested == 0)
		#ifndef BCC_BLOCK_IGNORE
				asm int 60h;
		#endif
			switchRequested = 0;



}


unsigned oldTimerOFF, oldTimerSEG;

void inic(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0 // ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG // postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif
}

void restore(){
#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif
}

