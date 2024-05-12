.extern _ZN5Riscv20handleSupervisorTrapEv
.extern _ZN5Riscv15handleTimerTrapEv
.extern _ZN5Riscv17handleConsoleTrapEv

.align 4
.global vectorTable
//.type _ZN5Riscv14supervisorTrapEv, @function
vectorTable:
j _ZN5Riscv14supervisorTrapEv
j _ZN5Riscv9timerTrapEv
li x0, 1
li x0, 1
li x0, 1
li x0, 1
li x0, 1
li x0, 1
li x0, 1
li x0, 1
j _ZN5Riscv11consoleTrapEv


.align 4
.global _ZN5Riscv14supervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv, @function
_ZN5Riscv14supervisorTrapEv:
    addi sp, sp, -256
    .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv20handleSupervisorTrapEv

    .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr

    addi sp, sp, 256
    sret

 .align 4
 .global _ZN5Riscv9timerTrapEv
 .type _ZN5Riscv9timerTrapEv, @function
 _ZN5Riscv9timerTrapEv:
     addi sp, sp, -256
     .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     sd x\index, \index * 8(sp)
     .endr

     call _ZN5Riscv15handleTimerTrapEv

     .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     ld x\index, \index * 8(sp)
     .endr

     addi sp, sp, 256
     sret


 .align 4
 .global _ZN5Riscv11consoleTrapEv
 .type _ZN5Riscv11consoleTrapEv, @function
 _ZN5Riscv11consoleTrapEv:
     addi sp, sp, -256
     .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     sd x\index, \index * 8(sp)
     .endr

     call _ZN5Riscv17handleConsoleTrapEv

     .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     ld x\index, \index * 8(sp)
     .endr

     addi sp, sp, 256
     sret