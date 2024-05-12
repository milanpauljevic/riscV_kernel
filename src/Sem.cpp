#include "../h/Sem.hpp"

int Sem::open(Sem **handle, int i) {
    Sem* s=(Sem*)MemAlloc::alocirajMem((sizeof(Sem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    if(!s)return -1;//neuspela alokacija
    s->val=i;
    s->head=s->tail=nullptr;
    *handle=s;
    return 0;
}

int Sem::close(Sem *handle) {
    if(!handle)return -1;//ne postoji semafor
    TCB* tmp=handle->getTCB();
    while(tmp){
        tmp->blocked=false;
        tmp->semClosed=true;
        Scheduler::put(tmp);
        tmp=handle->getTCB();
    }
    int ret=MemAlloc::oslobodiMem(handle);
    handle=nullptr;
    return ret;
}

int Sem::wait(Sem *handle) {
    if(!handle)return -1;//ne postoji semafor
    /*int init=handle->i;
    handle->val=--init;*/
    if(--handle->val<0){
        if(TCB::running->semClosed)return -1;
        handle->putTCB(TCB::running);
        TCB::running->blocked=true;
        TCB::dispatch();
    }
    return 0;
}

int Sem::signal(Sem *handle) {
    if(!handle)return -1;//ne postoji semafor
    if(++handle->val<=0){
        TCB* tmp=handle->getTCB();
        //if(!tmp)return -2;//prazan queue
        tmp->blocked=false;
        Scheduler::put(tmp);
    }
    return 0;
}

TCB* Sem::getTCB() {
    if(!head)return nullptr;
    Elem* tmp=head;
    head=head->next;
    if(!head)tail=nullptr;
    TCB* t=tmp->tcb;
    MemAlloc::oslobodiMem(tmp);
    return t;
}

int Sem::putTCB(TCB *tcb) {
    if(!tcb)return -1;//ne postoji tcb
    Elem* e=(Elem*)MemAlloc::alocirajMem((sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    if(!e)return -2;//neuspela alokacija
    e->tcb=tcb;
    e->next=nullptr;
    if(head){
        tail->next=e;
        tail=e;
    }
    else{
        head=tail=e;
    }
    return 0;
}
