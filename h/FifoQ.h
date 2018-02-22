#ifndef FIFOQ_H_
#define FIFOQ_H_
#include "PCB.h"
#include "SCHEDULE.H"


typedef int ID;


class FifoQ {
private:
	struct Node{
		PCB* pcb;
		Node* next;
		Node (PCB* ppcb): pcb(ppcb), next(0){}
	};
public:
	Node* head;
	Node* tail;

	PCB* get();
	void put(PCB* p);
	FifoQ();
	~FifoQ();
	int number();
	Thread* find(ID id);
	void removeById(ID id);
};

#endif
