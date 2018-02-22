#ifndef TIMER_H_
#define TIMER_H_

#include "SCHEDULE.H"
#include "KernelSe.h"
#include "PCB.h"

extern volatile int switchRequested;

void inic();
void interrupt timer();
void restore();

#endif
