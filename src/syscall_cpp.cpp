#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"

void* operator new (size_t size){
    return mem_alloc(size);
}
void operator delete(void *adr) noexcept{
    mem_free(adr);
}
void* operator new[] (size_t size) {
    return mem_alloc(size);
}
void operator delete[](void *adr) noexcept{
    mem_free(adr);
}

void Console::putc(char c) {
    ::putc(c);
}

char Console::getc() {
    return ::getc();
}

Thread::Thread() {
    myHandle= nullptr;
    thread_create(&myHandle,Thread::threadWrapper ,(void*)this);
}

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle= nullptr;
    thread_create(&myHandle,body,arg);
}

void Thread::join() {
    thread_join(this->myHandle);
}

int Thread::start() {
    Scheduler::put(myHandle);
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::threadWrapper(void* thread) {
    ((Thread*)thread)->run();
}

Thread::~Thread() {
    myHandle->setFinished(true);
    delete myHandle;
}

int Thread::sleep(time_t) {
    return 0;   //nisam radio
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

Semaphore::Semaphore(unsigned int init) {
    myHandle= nullptr;
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
    //delete myHandle;
}

void PeriodicThread::terminate() {
    // nisam radio
}

PeriodicThread::PeriodicThread(time_t period) {
    this->period=period;
}
