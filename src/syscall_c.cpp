
#include "../h/syscall_c.h"

void *mem_alloc(size_t size) {
    size=(size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    uint64 code=0x01;
    __asm__ volatile("mv a1, %[size]"::[size]"r" (size));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
    uint64 ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));
    return (void*)ret;
}

int mem_free(void *adr) {
    uint64 code=0x02;
    __asm__ volatile("mv a1, %[adr]"::[adr]"r" (adr));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
    uint64 ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));
    return ret;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    volatile uint64 code=0x11;
    volatile thread_t* h=handle;
    void (*startRout)(void *)=start_routine;
    volatile void* a=arg;
    //alokacija steka
    uint64* stack=(uint64*)MemAlloc::alocirajMem((DEFAULT_STACK_SIZE+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);

    //pakovanje parametara
    __asm__ volatile("mv a4, %[stack]"::[stack]"r" (stack));
    __asm__ volatile("mv a3, %[arg]"::[arg]"r" (a));
    __asm__ volatile("mv a2, %[start_routine]"::[start_routine]"r" (startRout));
    __asm__ volatile("mv a1, %[handle]"::[handle]"r" (h));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));

    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));

    return ret;
}

int thread_create_cpp(thread_t *handle, void (*start_routine)(void *), void *arg) {
    uint64 code=0x15;
    thread_t* h=handle;
    void (*startRout)(void *)=start_routine;
    void* a=arg;
    //alokacija steka
    uint64* stack=(uint64*)MemAlloc::alocirajMem((DEFAULT_STACK_SIZE+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);

    //pakovanje parametara
    __asm__ volatile("mv a4, %[stack]"::[stack]"r" (stack));
    __asm__ volatile("mv a3, %[arg]"::[arg]"r" (a));
    __asm__ volatile("mv a2, %[start_routine]"::[start_routine]"r" (startRout));
    __asm__ volatile("mv a1, %[handle]"::[handle]"r" (h));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));

    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));

    return ret;
}

int thread_exit() {
    uint64 code=0x12;
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    int ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));
    return ret;
}

void thread_dispatch() {
    uint64 code=0x13;
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
}

void thread_join(thread_t handle) {
    uint64 code=0x14;
    __asm__ volatile("mv a1, %[handle]"::[handle]"r" (handle));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
}

int sem_open(sem_t *handle, unsigned int init) {
    uint64 code=0x21;
    __asm__ volatile("mv a2, %[init]"::[init]"r" (init));
    __asm__ volatile("mv a1, %[handle]"::[handle]"r" (handle));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));
    return ret;
}

int sem_close(sem_t handle) {
    uint64 code=0x22;
    __asm__ volatile("mv a1, %[handle]"::[handle]"r" (handle));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));
    return ret;
}

int sem_wait(sem_t id) {
    uint64 code=0x23;
    __asm__ volatile("mv a1, %[id]"::[id]"r" (id));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));
    return ret;
}

int sem_signal(sem_t id) {
    uint64 code=0x24;
    __asm__ volatile("mv a1, %[id]"::[id]"r" (id));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));
    return ret;
}

int time_sleep(time_t) {
    return 0;
}

char getc() {
    uint64 code=0x41;
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
    uint64 ret;
    __asm__ volatile("mv %[ret], a0":[ret]"=r"(ret));
    return (char)ret;
}

void putc(char c) {
    uint64 code=0x42;
    __asm__ volatile("mv a1, %[c]"::[c]"r" (c));
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
}

void changeUser(){
    uint64 code=0x75;
    __asm__ volatile("mv a0, %[code]"::[code]"r" (code));
    __asm__ volatile("ecall");
    return;
}
