#include "FifoQ.h"


FifoQ::FifoQ(): head(0), tail(0){}

void FifoQ::put(PCB* p){
		 Node* t = tail;
         tail = new Node(p);
         if (head==0) head = tail;
         else t->next = tail;
}

PCB* FifoQ::get(){
	if (head==0) return 0;
	PCB* p=head->pcb;
	Node* t=head->next;
	delete head;
	if (t!=0) head=t;
	else {head=0; tail=0;}
	return p;
}

FifoQ::~FifoQ(){
	if (head==0) return;
	while (head){
				Node *old=head;
				head=head->next;
				delete old;
			}
	head=0;
	tail=0;
}

int FifoQ::number(){
	if (head==0) return 0;
	int n=0;
	for (Node *curr=head; curr!=0; curr=curr->next) n++;
	return n;
}

Thread* FifoQ::find(ID id){
	if (head==0) return 0;
	for (Node* help=head; help!=0; help=help->next){
		if (help->pcb->id==id) return help->pcb->myThread;
	}
	return 0;

}

void FifoQ::removeById(ID id){
	Node* current=head;
	Node* prev=0;
	if (current==0) return;
	while (current!=0 && current->pcb->getId()!=id){
			prev=current;
			current=current->next;
	}
	if (current==0) {prev=0; return;}

	if (prev==0){
		PCB* p=get();
		p=0;
		return;
	}
	if (current==tail) tail=prev;
	prev->next=current->next;
	current->next=0;
	delete current;

}
