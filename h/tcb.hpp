#ifndef _TCB_
#define _TCB_

#include "MemoryAllocator.h"
//#include "Riscv.hpp"

class TCB{
public:
    bool isFinished(){return finished;}
    void setFinished(bool fin){
        finished=fin;
        if(fin)signalJoinedThreads();
    }

    using Body=void(*)(void*);

    static TCB* createTCB(Body body, void* arg, uint64* stack);

    static TCB* createThread(TCB** handle, Body body, void* arg, uint64* stack);
    static TCB* create_CPP_Thread(TCB** handle, Body body, void* arg, uint64* stack);

    static TCB* running;
    static void yield();
    static void joinThread(TCB* t);

private:
    struct Context{
        uint64 ra, sp;
    };

    struct Elem{
        TCB* tcb;
        Elem* next;
    };

    void* arg;
    Elem *head, *tail;
    bool finished, blocked, semClosed;
    uint64* stack;
    Body body;
    Context context;

    static void dispatch();
    static void contextSwitch(TCB::Context *old, TCB::Context *running);

    static void wrapper();

    void signalJoinedThreads();


    friend class Sem;
    friend class Riscv;
};

#endif


