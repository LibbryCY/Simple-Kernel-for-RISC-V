//
// Created by os on 9/9/23.
//

#include "../h/sem.hpp"

Sem *Sem::createSem(int init = 0) {
    return new Sem(init);
}

Sem::~Sem() {
    this->close();
}

int Sem::close() {
    if (closed)
        return -1;
    closed = true;
    if (blocked.peekFirst() != nullptr) {
        while (blocked.peekFirst()) {
            blocked.peekFirst()->setBlocked(false);
            Scheduler::put(blocked.removeFirst());
        }
    }

    return 0;
}

void Sem::block() {
    TCB::running->setBlocked(true);
    blocked.addLast(TCB::running);
    thread_dispatch();
}

void Sem::deblock() {
    TCB* temp = blocked.removeFirst();
    temp->setBlocked(false);
    Scheduler::put(temp);
}

int Sem::wait() {
    if(closed)return -1;
    val--;
    if(val < 0)block();
    return 0;
}

int Sem::signal() {
    if(closed)return -1;
    val++;
    if(val <= 0)deblock();
    return 0;
}
