#include "../h/riscv.hpp"
#include "../h/syscall_c.h"

int retFromSys(){
        volatile uint64 ret;
        __asm__ volatile ("mv %0, a0" : "=r"(ret));
        return (int)ret;
};

void* mem_alloc(size_t size){
    if(size<=0)return nullptr;
    size_t blocks = (size % MEM_BLOCK_SIZE == 0) ?
             size / MEM_BLOCK_SIZE :
             size / MEM_BLOCK_SIZE + 1;
    __asm__ volatile("mv a1, %0" : : "r" (blocks));  //size je u blokovima
    __asm__ volatile("mv a0, %0" : : "r" (MEMALLOC));
    __asm__ volatile ("ecall");

     uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return (void*)(ret);
}

int mem_free (void* adr){
    if (!adr) { return -1; }

    __asm__ volatile("mv a1, %0" : : "r" (adr));  //size je u blokovima
    __asm__ volatile("mv a0, %0" : : "r" (MEMFREE));
    __asm__ volatile ("ecall");

    return retFromSys();
}

int thread_create (thread_t* handle,void(*start_routine)(void*),void* arg){
    __asm__ volatile("mv a3, %[arg]": : [arg] "r" (arg));
    __asm__ volatile("mv a2, %[arg]": : [arg] "r" (start_routine));
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (TCREATE));
    __asm__ volatile("ecall");

    return retFromSys();
}

int thread_exit (){
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (TEXIT));
    __asm__ volatile("ecall");

    return retFromSys();
}

void thread_dispatch (){
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (TDISPATCH));
    __asm__ volatile("ecall");
}

void thread_join (thread_t handle){
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (TJOIN));
    __asm__ volatile("ecall");
}

int sem_open (sem_t* handle,unsigned init){
    __asm__ volatile("mv a2, %[arg]": : [arg] "r" (init));
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (SOPEN));
    __asm__ volatile("ecall");

    return retFromSys();
}

int sem_close (sem_t handle){
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (SCLOSE));
    __asm__ volatile("ecall");

    return retFromSys();
}

int sem_wait (sem_t id){
    //if(id == nullptr)return -1;
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (id));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (SWAIT));
    __asm__ volatile("ecall");

    return retFromSys();
}

int sem_signal (sem_t id){
    if(id == nullptr)return -1;
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (id));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (SSIGNAL));
    __asm__ volatile("ecall");

    return retFromSys();
}

int time_sleep (time_t){

    return 0;
}

char getc (){
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (GETC));
    __asm__ volatile("ecall");

    return retFromSys();
}

void putc(char c){
    char ch=c;
    __asm__ volatile("mv a1,%0"::"r"((uint64)ch));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (PUTC));
    __asm__ volatile("ecall");
}

void changeSysMode(){
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (CHANGEMODE));
    __asm__ volatile("ecall");
}