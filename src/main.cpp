#include "../h/syscall_cpp.hpp"

void userMain();

void userMainWrapper(void * t){
    userMain();
}

extern "C" void vectorTable();

int main(){
    __asm__ volatile("csrw stvec, %[vektor]"::[vektor]"r" (&vectorTable));
    __asm__ volatile("csrr t1, stvec");
    __asm__ volatile("addi t1, t1, 1");
    __asm__ volatile("csrw stvec, t1");

    thread_t nit;
    thread_create_cpp(&nit, nullptr, nullptr);
    TCB::running=(TCB*)nit;

    changeUser();

    Thread t=Thread(userMainWrapper, nullptr);
    t.start();

    t.join();

    return 0;
}