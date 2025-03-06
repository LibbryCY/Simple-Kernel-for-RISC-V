
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/sem.hpp"


enum Interrupts: uint64 {
    ECALL_SUPER = 0x0000000000000009UL,
    ECALL_USER  = 0x0000000000000008UL
};
void Riscv::popSppSpie()
{
     //ms_sstatus(SSTATUS_SPP);
    __asm__ volatile ("csrw sepc, ra");  //ra je povratna adresa funkcije popSppSpie unutar threadWrappera
    __asm__ volatile ("sret");
}


void Riscv::handleTime(){
    Riscv::mc_sip(SIP_SSIP);
}

void Riscv::handleIntr() {
    uint64 scause=r_scause();
    if (scause == ECALL_SUPER || scause == ECALL_USER) //(ecall)
    {
        uint64 volatile sstatus = r_sstatus();
        uint64 volatile sepc = r_sepc() + 4;
        uint64 syscode;
        __asm__ volatile ("mv %[code], a0" : [code]"=r"(syscode));

        if(syscode==MEMALLOC){
            uint64 a1;              //broj blokova
            __asm__ volatile("ld %0, 8*11(fp)":"=r"(a1));
            size_t size = a1 * MEM_BLOCK_SIZE;     //u bajtovima

            void* ret = MemoryAllocator::m_alloc(size);
            __asm__ volatile("sd %0,10*8(fp)"::"r"(ret));  //vraca adresu
        }else if(syscode==MEMFREE){
            void* a1;           //adresa
            __asm__ volatile("ld %0, 8*11(fp)":"=r"(a1));

            uint64 ret=MemoryAllocator::m_free(a1);
            __asm__ volatile("sd %0,10*8(fp)"::"r"(ret));   //vraca 0 za uspeh
        }else if(syscode==TCREATE){
            uint64 thandle;
            __asm__ volatile ("ld %[handle], 11*8(fp)" : [handle]"=r"(thandle));
            uint64 startR;
            __asm__ volatile ("ld %[rs], 12*8(fp)" : [rs]"=r"(startR));
            TCB::Body funct=(TCB::Body)startR;
            void* arg;
            __asm__ volatile("ld %[arg], 13*8(fp)": [arg] "=r"(arg));
            TCB** threadHandle=(TCB**) thandle;
            *threadHandle=TCB::createThread(threadHandle,funct,arg);
            uint64 ret=0;
            if(threadHandle== nullptr)ret=-1;
            __asm__ volatile ("sd %0, 10*8(fp)"::"r"(ret));

        }else if(syscode == TDISPATCH){
            TCB::dispatch();
        }else if(syscode == TEXIT){
            TCB::running->setFinished(true);
            TCB::dispatch();
        }else if(syscode == TJOIN){
            thread_t handle;
            __asm__ volatile("ld t2, 8*11(fp)");
            __asm__ volatile("mv %0, t2" : "=r" (handle));
            TCB::threadJoin(handle);

        }else if(syscode == SOPEN){
            sem_t* handle;
            int init;
            uint64 ret;

            __asm__ volatile("ld %[arg], 11*8(fp)": [arg] "=r"(handle));

            __asm__ volatile("ld %[arg], 12*8(fp)": [arg] "=r"(init));

            *handle = Sem::createSem(init);

            if (*handle == nullptr){
                ret = -1;
            }
            else {
                ret = 0;
            }
            __asm__ volatile("sd %0,10*8(fp)"::"r"(ret));

         }else if(syscode == SCLOSE){
            sem_t handle;
            uint64 ret;

            __asm__ volatile("ld %[arg], 11*8(fp)": [arg] "=r"(handle));
            ret = (handle)->close();

            __asm__ volatile("sd %0,10*8(fp)"::"r"(ret));

        }else if(syscode==SWAIT) {
            sem_t id;
            uint64 ret;

            __asm__ volatile("ld %[arg], 11*8(fp)": [arg] "=r"(id));
            ret = (id)->wait();

            __asm__ volatile("sd %0,10*8(fp)"::"r"(ret));
        }else if(syscode==SSIGNAL) {
            sem_t id;
            uint64 ret;

            __asm__ volatile("ld %[arg], 11*8(fp)": [arg] "=r"(id));
            ret = (id)->signal();

            __asm__ volatile("sd %0,10*8(fp)"::"r"(ret));
        }else if(syscode==CHANGEMODE) {
            w_sstatus(sstatus);
            mc_sstatus(SSTATUS_SPP);
            w_sepc(sepc); //povratni PC
            return;
        }else if (syscode == GETC) {
            //getc
            char c=__getc();
            __asm__ volatile("sd %0,10*8(fp)"::"r"(c));
        } else if (syscode == PUTC) {
            uint64 ch;
            __asm__ volatile("ld t2, 8*11(fp)");
            __asm__ volatile("mv %0, t2" : "=r" (ch));
            __putc((char)ch);
        }

        w_sstatus(sstatus);
        w_sepc(sepc); //povratni PC

    } else{
        console_handler();
    }

}

void Riscv::handleConsole() {
    console_handler();
}


void Riscv::printString(const char *string) {
    while(*string!='\0'){
        __putc(*string);
        string++;
    }
}

void Riscv::printInt(uint64 integer)
{
    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if (integer < 0)
    {
        neg = 1;
        x = -integer;
    } else
    {
        x = integer;
    }

    i = 0;
    do
    {
        buf[i++] = digits[x % 10];
    } while ((x /= 10) != 0);
    if (neg)
        buf[i++] = '-';

    while (--i >= 0) { putc(buf[i]); }
}

void Riscv::printHexa(uint64 xx)
{
    char buffer[16];
    for (int i = 7; i >= 0; i--)
    {
        uint8 byte = (xx >> (8 * i)) & 0xFF; // i-ti bajt
        buffer[15 - (i*2 + 1)] = ("0123456789ABCDEF"[byte >> 4]);
        buffer[15 - (i*2)] = ("0123456789ABCDEF"[byte & 0x0F]);
    }

    __putc('0'); __putc('x');

    int i = 0;
    while (i < 16) __putc(buffer[i++]);

}
