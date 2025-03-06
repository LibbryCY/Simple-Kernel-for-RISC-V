//
// Created by os on 9/9/23.
//
#ifndef PROJECTBASE_SEM_HPP
#define PROJECTBASE_SEM_HPP

#include "../h/tcb.hpp"
#include "../h/syscall_c.h"


class Sem{
public:
    ~Sem();
    int wait ();
    int signal ();
    int close ();
    int getVal () const { return val; };
    static Sem* createSem(int init);
private:
    List<TCB> blocked;

    explicit Sem (int v) : val(v)  {}

    int val;
    bool closed = false;

    void block();
    void deblock();

    friend Riscv;
};


#endif //PROJECTBASE_SEM_HPP
