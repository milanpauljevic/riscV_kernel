#ifndef _SCHEDULER_
#define _SCHEDULER_

#include "../lib/hw.h"
#include "tcb.hpp"
#include "MemoryAllocator.h"

struct Node{
    Node* next;
    TCB* tcb;
};

class Scheduler{
public:
    static void put(TCB*);
    static TCB* get();
private:
    static Node *head, *tail;

    friend class TCB;
};

#endif