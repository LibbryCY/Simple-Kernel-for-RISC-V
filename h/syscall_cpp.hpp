
#ifndef PROJECTBASE_SYSCALL_CPP_HPP
#define PROJECTBASE_SYSCALL_CPP_HPP

#include "syscall_c.h"

void* operator new (size_t size);
void operator delete(void *adr) noexcept;
void* operator new[] (size_t size);
void operator delete[](void *adr) noexcept;

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);     //ne treba
    thread_t getHandle(){ return myHandle; }

    static void threadWrapper(void *);

protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
};

class Semaphore {
public:
    explicit Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    time_t period;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};
#endif //PROJECTBASE_SYSCALL_CPP_HPP
