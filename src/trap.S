
.extern _ZN5Riscv10handleTimeEv
.extern _ZN5Riscv13handleConsoleEv
.extern _ZN5Riscv10handleIntrEv
.global _ZN5Riscv14supervisorTrapEv
.align 4
_ZN5Riscv14supervisorTrapEv:
    j handleIntrT
    j handleTimeT
    ld x0,0*8(sp)
    ld x0,0*8(sp)
    ld x0,0*8(sp)
    ld x0,0*8(sp)
    ld x0,0*8(sp)
    ld x0,0*8(sp)
    ld x0,0*8(sp)
    j handleConsoleT

handleIntrT:
     addi sp, sp, -256
     .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     sd x\index, \index * 8(sp)
     .endr

     call _ZN5Riscv10handleIntrEv

     .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     ld x\index, \index * 8(sp)
     .endr
     addi sp, sp, 256

     sret

handleTimeT:
     addi sp, sp, -256
     .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     sd x\index, \index * 8(sp)
     .endr

     call _ZN5Riscv10handleTimeEv

     .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     ld x\index, \index * 8(sp)
     .endr
     addi sp, sp, 256

     sret

handleConsoleT:
     addi sp, sp, -256
     .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     sd x\index, \index * 8(sp)
     .endr

     call _ZN5Riscv13handleConsoleEv

     .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     ld x\index, \index * 8(sp)
     .endr
     addi sp, sp, 256

     sret