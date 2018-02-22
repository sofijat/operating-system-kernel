#include "SortedQ.h"
#include "IdleThr.h"

void SortedQ::put(PCB* p, Time t){
	Node *node = new Node(p, t);
	Node *curr=head, *prev=0;
	volatile int sum=0;

	if(curr==0) head=node;
	else{
		sum=curr->waiting;
		while (t>sum && curr!=0){
			prev=curr;
			curr=curr->next;
			if (curr!=0)
			sum+=curr->waiting;
		}
		if (prev==0){
			head=node;
			node->next=curr;
			curr->waiting-=node->waiting;
		}
		else{
			if (curr!=0){
				prev->next=node;
				node->next=curr;
				node->waiting-=(sum-curr->waiting);
				curr->waiting-=node->waiting;
			}
			else {
				prev->next=node;
				node->waiting-=sum;
			}
		}

	}
}

SortedQ::SortedQ(): head(0) {}

SortedQ::~SortedQ(){
	if (head==0) return;
	while(head){
		Node* old=head;
		head=head->next;
		old->next=0;
		delete old;
	}
	head=0;
}

void SortedQ::removeById(ID id){
	Node* sof=head;
	Node* prev=0;
	if (sof==0) return;
	while (sof!=0 && sof->pcb->getId()!=id){
				prev=sof;
				sof=sof->next;
	}
	if (sof==0) return;

	if (sof==head){
			head=head->next;
			if (head!=0) head->waiting+=sof->waiting;
	}
	else {
			prev->next=sof->next;
			if (sof->next!=0){
				sof->next->waiting+=sof->waiting;
			}
	}
	sof->next=0;
	delete sof;
}

void SortedQ::update(){
	if (head==0) return;
	head->waiting--;
	if (head->waiting!=0) {return;}
	while (head!=0 && head->waiting==0) {
			PCB* ppp=head->pcb;
			ppp->state=PCB::READY;
			if (ppp->mySemaphore!=0){
			ppp->mySemaphore->listOfBlockedPCB->removeById(ppp->getId());
			ppp->mySemaphore->value++;
			ppp->mySemaphore=0;
			}
			ppp->waitValue=0;
			if (ppp!=PCB::idle->getPCB()) Scheduler::put(ppp);
			this->removeById(ppp->getId());
		}

}
