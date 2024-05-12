#ifndef _SEM_HPP
#define _SEM_HPP

#include "tcb.hpp"
#include "MemoryAllocator.h"
#include "Scheduler.h"

class Sem{
public:
    static int open(Sem** handle, int i);
    static int close(Sem* handle);
    static int wait(Sem* handle);
    static int signal(Sem* handle);


private:
    int val;

    struct Elem{
        TCB* tcb;
        Elem* next;
    };

    Elem *head,*tail;
    //Sem()=delete;
    TCB* getTCB();
    int putTCB(TCB* tcb);






};

#endif