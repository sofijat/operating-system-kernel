#ifndef SORTEDQ_H_
#define SORTEDQ_H_
#include "SCHEDULE.H"
#include "PCB.h"

class PCB;
typedef unsigned int Time;

class SortedQ {
private:
	struct Node{
		PCB* pcb;
		Node* next;
		Time waiting;
		Node (PCB* ppcb, Time t): pcb(ppcb), waiting(t), next(0) {}
	};
public:
	Node *head;
	void put(PCB *p, Time t);
	SortedQ();
	~SortedQ();
	void update();
	void removeById(ID id);

};

#endif
