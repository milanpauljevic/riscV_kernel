#include "../h/MemoryAllocator.h"

FreeMem* MemAlloc::head=nullptr;
bool MemAlloc::init=false;

int MemAlloc::oslobodiMem(void *adr) {
    if((uint64*)adr<(uint64*)HEAP_START_ADDR || (uint64*)adr>=(uint64*)HEAP_END_ADDR) return -1;
    if(!adr) return 0;
    FreeMem* p;
    if(!head || (uint64*)adr<(uint64*)head) p=nullptr;
    else{
        for(p=head;p->next && (uint64*)(p->next)<(uint64*)adr;p=p->next);
    }

    FreeMem* fm = (FreeMem*)((char*)adr-sizeof(FreeMem));

    if(!p){
        fm->prev=nullptr;
        fm->next=head;
        head->prev=fm;
        head=fm;
    }
    else{
        fm->next=p->next;//1.
        if(fm->next)fm->next->prev=fm;//2.
        fm->prev=p;//3.
        p->next=fm;//4.
    }

    tryToJoin(fm);
    tryToJoin(fm->prev);
    return 0;
}

void *MemAlloc::alocirajMem(size_t sz) {
    if(!init){
        init=true;
        head=(FreeMem*)((uint64*)HEAP_START_ADDR);
        head->prev=0;
        head->next=0;
        head->size=((size_t)((uint64*)HEAP_END_ADDR-(uint64*)HEAP_START_ADDR)-sizeof(MemAlloc))/MEM_BLOCK_SIZE;
    }
    void* adr=nullptr;

    for(FreeMem* cur=head;cur;cur=cur->next){
        if(cur->size>=sz){
            adr=cur;
            if(cur->size==sz){
                if(cur->prev)cur->prev->next=cur->next;
                else head=cur->next;
                if(cur->next)cur->next->prev=cur->prev;
                cur->size=sz;
            }
            else{
                FreeMem* novi=(FreeMem*)((uint64*)cur+sz*MEM_BLOCK_SIZE);
                if(cur->next)cur->next->prev=novi;
                if(cur->prev) cur->prev->next=novi;
                else head=novi;
                novi->prev=cur->prev;
                novi->next=cur->next;
                novi->size=cur->size-sz;
                cur->size=sz;
            }
            break;
        }
    }
    return (char*)adr+sizeof(FreeMem);
}

void MemAlloc::tryToJoin(FreeMem* fm) {
    if(!fm)return;
    if(fm->next && (char*)(fm->next)==((char*)fm+fm->size*MEM_BLOCK_SIZE)){
        fm->size+=fm->next->size;
        fm->next=fm->next->next;
        if(fm->next)fm->next->prev=fm;
    }
}