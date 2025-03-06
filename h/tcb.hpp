
#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/riscv.hpp"

// Thread Control Block
class TCB
{
public:
    ~TCB() {
       delete[] stack;
    }

    void setBlocked(bool value) { blocked = value; }

    bool isBlocked() const { return blocked; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    using Body = void (*)(void*);

    static TCB *createThread(TCB** handle,Body body,void* arg);

    static void yield();

    static TCB *running;
    static void dispatch();

    static void threadJoin(TCB*);

private:

    TCB(Body body, void* arg) :
            body(body),
            args(arg),
            stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
            context({(uint64)&threadWrapper,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0  //sp pokayuje na lokaciju iza poslednje adrese niza
                    }),
            finished(false),
            blocked(false)
            {
                if (body != nullptr && body != Thread::threadWrapper ) { Scheduler::put(this); }
            }
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* args;
    uint64 *stack;
    Context context;
    bool finished;
    bool blocked;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP