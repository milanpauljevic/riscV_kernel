#include "../h/Scheduler.h"


Node* Scheduler::head=nullptr;
Node* Scheduler::tail=nullptr;

void Scheduler::put(TCB *t) {
    size_t sz=(sizeof(Node)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    Node* n=(Node*)MemAlloc::alocirajMem(sz);
    n->tcb=t;

    if(!Scheduler::tail){
        Scheduler::tail=Scheduler::head=n;
    }
    else{
        Scheduler::tail->next=n;
        Scheduler::tail=n;
    }
    n->next=nullptr;
}

TCB* Scheduler::get() {
    if(!Scheduler::head)return nullptr;
    Node* n=Scheduler::head;
    Scheduler::head=Scheduler::head->next;
    if(!Scheduler::head) Scheduler::tail=nullptr;
    TCB* tcb=n->tcb;
    MemAlloc::oslobodiMem(n);
    return tcb;
}
