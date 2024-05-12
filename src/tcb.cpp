#include "../h/tcb.hpp"
#include "../h/Riscv.hpp"

TCB* TCB::running=nullptr;

void TCB::dispatch() {
    TCB* old=TCB::running;
    TCB::running=Scheduler::get();
    if(!old->finished && !old->blocked){Scheduler::put(old);}
    contextSwitch(&old->context, &running->context);
    //if(old->isFinished()){MemAlloc::oslobodiMem(old);}
}

TCB *TCB::createTCB(TCB::Body body, void *arg, uint64 *stack) {
    TCB* t=(TCB*)MemAlloc::alocirajMem((sizeof(TCB)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    t->finished=false;
    t->body=body;
    t->arg=arg;
    t->stack=body?stack:nullptr;
    t->context={(uint64)&wrapper, (uint64)(stack?&stack[DEFAULT_STACK_SIZE]:0)};
    t->blocked=false;
    t->head=t->tail=nullptr;
    t->semClosed=false;
    return t;
}

void TCB::wrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    thread_dispatch();
}

TCB *TCB::createThread(TCB** handle, TCB::Body body, void *arg, uint64 *stack) {
    TCB* thread= createTCB(body, arg, stack);
    *handle=thread;
    if(*handle)Scheduler::put(*handle);
    return thread;
}

void TCB::signalJoinedThreads() {
    Elem* old;
    while(this->head){
        old=this->head;
        this->head=this->head->next;
        Scheduler::put(old->tcb);
        MemAlloc::oslobodiMem(old);
    }
    this->tail=nullptr;
}

void TCB::joinThread(TCB *t) {
    running->blocked=true;
    Elem* e=(Elem*)MemAlloc::alocirajMem((sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    e->next=nullptr;
    e->tcb=running;
    if(t->head)t->tail->next=e;
    else t->head=e;
    t->tail=e;
    dispatch();
}

TCB *TCB::create_CPP_Thread(TCB **handle, TCB::Body body, void *arg, uint64 *stack) {
    TCB* thread= createTCB(body, arg, stack);
    *handle=thread;
    return thread;
}
