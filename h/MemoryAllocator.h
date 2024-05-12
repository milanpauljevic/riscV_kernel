#ifndef __MemoryAllocator__cpp__
#define __MemoryAllocator__cpp__

#include "../lib/hw.h"

struct FreeMem{
    FreeMem *next, *prev;
    size_t size;
};

class MemAlloc{
public:
    static void* alocirajMem(size_t sz);
    static int oslobodiMem(void* adr);

private:
    static FreeMem* head;
    static void tryToJoin(FreeMem* fm);
    static bool init;
};

#endif