#include "../h/Riscv.hpp"

void Riscv::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap() {
    uint64 volatile scauseV;
    __asm__ volatile("csrr %[retV], scause":[retV]"=r" (scauseV));
    uint64 volatile par1, par2, par3, par4;
    __asm__ volatile("mv %[par], a1": [par]"=r"(par1));
    __asm__ volatile("mv %[par], a2": [par]"=r"(par2));
    __asm__ volatile("mv %[par], a3": [par]"=r"(par3));
    __asm__ volatile("mv %[par], a4": [par]"=r"(par4));

    uint64 volatile sepcV;
    __asm__ volatile ("csrr %0, sepc" : "=r" (sepcV));

    if(scauseV==0x09 || scauseV==0x08) {
        uint64 volatile code;
        __asm__ volatile("mv %[code], a0": [code]"=r"(code));

        uint64 volatile sstatusV;
        __asm__ volatile("csrr %[retV], sstatus":[retV]"=r" (sstatusV));

        //mem_alloc
        if (code == 0x01) {
            size_t sz=(size_t)par1;
            void *adr = MemAlloc::alocirajMem(sz);
            __asm__ volatile("mv a0, %[retV]"::[retV]"r"(adr));
        }

            //mem_free
        else if (code == 0x02) {
            void *adr = (void *) par1;
            uint64 ret = MemAlloc::oslobodiMem(adr);
            __asm__ volatile("mv a0, %[retV]"::[retV]"r"(ret));
        }

            //thread_create and thread_cpp_create
        else if (code == 0x11 || code==0x15) {
            TCB** handle=(TCB**)par1;
            TCB::Body b=(TCB::Body)par2;
            void *arg = (void *) par3;
            uint64 *stack = (uint64 *) par4;
            TCB *t;
            if(code==0x15)t=TCB::create_CPP_Thread(handle, b, arg, stack);
            else  t = TCB::createThread(handle, b, arg, stack);
            int ret = 0;
            if (!t)ret = -1;
            __asm__ volatile("mv a0, %[retV]"::[retV]"r"(ret));
        }

            //thread_exit
        else if (code == 0x12) {
            TCB::running->setFinished(true);
            TCB*t=TCB::running;
            TCB::dispatch();
            MemAlloc::oslobodiMem(t);
        }

            //thread_dispatch
        else if (code == 0x13) {
            TCB::dispatch();
        }

            //thread_join
        else if (code == 0x14) {
            TCB *t = (TCB *) par1;
            TCB::running->joinThread(t);
        }

            //sem_open
        else if (code == 0x21) {
            Sem **semh = (Sem **) par1;
            unsigned init = (unsigned) par2;
            uint64 ret = Sem::open(semh, init);
            __asm__ volatile("mv a0, %[retV]"::[retV]"r"(ret));
        }

            //sem_close
        else if (code == 0x22) {
            Sem *semh = (Sem *) par1;
            uint64 ret = Sem::close(semh);
            __asm__ volatile("mv a0, %[retV]"::[retV]"r"(ret));
        }

            //sem_wait
        else if (code == 0x23) {
            Sem *semh = (Sem *) par1;
            uint64 ret = Sem::wait(semh);
            __asm__ volatile("mv a0, %[retV]"::[retV]"r"(ret));
        }

            //sem_signal
        else if (code == 0x24) {
            Sem *semh = (Sem *) par1;
            uint64 ret = Sem::signal(semh);
            __asm__ volatile("mv a0, %[retV]"::[retV]"r"(ret));
        }

        //getc
        else if (code == 0x41) {
            char ret = __getc();
            __asm__ volatile("mv a0, %[retV]"::[retV]"r"((uint64)ret));
        }

            //putc
        else if (code == 0x42) {
            char c = (char) par1;
            __putc(c);
        }

        //changeUser
        else if(code==0x75){
            w_sstatus(sstatusV);
            uint64 x=1<<8;
            __asm__ volatile("csrc sstatus, %[x]":[x]"=r"(x));
            __asm__ volatile("csrw sepc, %[sepc]"::[sepc]"r" (sepcV+4));
            mc_sip(SIP_SSIP);
            return;
        }


        __asm__ volatile("csrw sstatus, %[sstatus]"::[sstatus]"r" (sstatusV));
        mc_sip(BitMaskSkip::SIP_SSIP);
        __asm__ volatile("csrw sepc, %[sepc]"::[sepc]"r" (sepcV+4));
    }
    else{
        __putc(scauseV+'0');
        __putc('\n');
        while(true);
    }
}

void Riscv::handleConsoleTrap() {
    console_handler();
}

void Riscv::handleTimerTrap() {
    mc_sip(SIP_SSIP);
}

