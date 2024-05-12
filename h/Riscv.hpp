#ifndef __riscv__
#define __riscv__

#include "../lib/hw.h"
#include "MemoryAllocator.h"
#include "tcb.hpp"
#include "Sem.hpp"
#include "Scheduler.h"
#include "../lib/console.h"
#include "../test/printing.hpp"

class Riscv{
public:
    //pop sstatus.spp and sstatus.spie bits
    static void popSppSpie();

    static void pushRegisters();
    static void popRegisters();

    static uint64 r_scause();// read scause
    static void w_scause(uint64 scause);// write scause

    static uint64 r_sepc();// read sepc
    static void w_sepc(uint64 sepc);// write sepc

    static uint64 r_stvec();// read stvec
    static void w_stvec(uint64 stvec);// write stvec

    enum BitMaskSkip{
        SIP_SSIP=(1<<1),
        SIP_STIP=(1<<5),
        SIP_SEIP=(1<<9),
    };

    static void ms_sip(uint64 mask);// set sip
    static void mc_sip(uint64 mask);// clear sip

    static uint64 r_sstatus();//read sstatus
    static void w_sstatus(uint64 sstatus);// write sstatus

    static void supervisorTrap();
    static void timerTrap();
    static void consoleTrap();


private:
    static void handleSupervisorTrap();
    static void handleTimerTrap();
    static void handleConsoleTrap();
};

inline uint64 Riscv::r_scause() {
    uint64 ret;
    __asm__ volatile("csrr %[retV], scause":[retV]"=r" (ret));
    return ret;
}

inline void Riscv::w_scause(uint64 scause) {
    __asm__ volatile("csrw %[scause], scause"::[scause]"r" (scause));
}

inline uint64 Riscv::r_sepc() {
    uint64 volatile ret;
    __asm__ volatile("csrr %[retV], sepc":[retV]"=r" (ret));
    return ret;
}

inline void Riscv::w_sepc(uint64 sepc) {
    __asm__ volatile("csrw %[sepc], sepc"::[sepc]"r" (sepc));
}

inline uint64 Riscv::r_stvec() {
    uint64 ret;
    __asm__ volatile("csrr %[retV], stvec":[retV]"=r" (ret));
    return ret;
}

inline void Riscv::w_stvec(uint64 stvec) {
    __asm__ volatile("csrw %[stvec], stvec"::[stvec]"r" (stvec));
}

inline void Riscv::mc_sip(uint64 mask)
{
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::ms_sip(uint64 mask)
{
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sstatus() {
    uint64 volatile ret;
    __asm__ volatile("csrr %[retV], sstatus":[retV]"=r" (ret));
    return ret;
}

inline void Riscv::w_sstatus(uint64 sstatus) {
    __asm__ volatile("csrw sstatus, %[sstatus] "::[sstatus]"r" (sstatus));
}

#endif