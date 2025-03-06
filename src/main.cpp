#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/tcb.hpp"
//#include "../h/worker.hpp"

extern void userMain();

void userMainWrapper(){
    userMain();
}

void main(){
    MemoryAllocator::init_mem();
    uint64 base=(uint64) &Riscv::supervisorTrap;
    Riscv::w_stvec(base | 1);

    //naprvi se kernel nit
    TCB *main;
    thread_create(&main, nullptr, nullptr);
    TCB::running = main;

    //prelazak u user rezim
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    changeSysMode();

    thread_t userMainThread;
    thread_create(&userMainThread, reinterpret_cast<void (*)(void *)>(&userMainWrapper), nullptr);
    while (!userMainThread->isFinished())
        thread_dispatch();

    delete userMainThread;
}