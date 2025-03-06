//
// Created by os on 9/3/23.
//
#include "../h/tcb.hpp"

TCB* TCB::running = nullptr;


void TCB::yield() {
    Riscv::pushRegisters();
    dispatch();
    Riscv::popRegisters();
}

void TCB::dispatch() {
    TCB* old = running;
    if(!old->isFinished() && !old->isBlocked()){
        Scheduler::put(old);
    }
    running = Scheduler::get();
    if(old != running) TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->args);
    thread_exit();
}

TCB* TCB::createThread(TCB** handle,TCB::Body body, void *arg) {
    *handle = new TCB(body, arg);
    return *handle;
}

void TCB::threadJoin(TCB* handle) {
    while(!handle->isFinished()){
        TCB::yield();
    }
}


