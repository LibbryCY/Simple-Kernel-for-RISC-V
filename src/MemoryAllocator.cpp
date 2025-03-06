
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"

MemDescr *MemoryAllocator::free_head = nullptr;

uint8 *MemoryAllocator::base = nullptr;

uint8 *MemoryAllocator::end = nullptr;


void MemoryAllocator::init_mem() {
    //poravnanje na cele blokove
    base=(uint8*)HEAP_START_ADDR ;
    end = (uint8*)HEAP_END_ADDR;

    free_head = (struct MemDescr *)base;
    struct MemDescr * node = free_head;

    node->next= nullptr;
    node->size = ((size_t)end-(size_t)base - MEM_BLOCK_SIZE);
}


void *MemoryAllocator::m_alloc(size_t size) {
    if(!free_head) return nullptr;
    struct MemDescr* tmp,*prev= nullptr;
    for(tmp=free_head;tmp;prev=tmp,tmp=tmp->next){
        if(tmp->size>=size)break;
    }
    if(!tmp) {
        Riscv::printString("tmp je null u mallocu");
        return nullptr; }
    size_t  rem = tmp->size - size;
    if(MEM_BLOCK_SIZE <= rem){    //ima jos memorije iza potrebne velicine za zaglavlj
        tmp->size = size;
        size_t offs = size + MEM_BLOCK_SIZE;   //pocetak novog free bloka
        MemDescr* newNode = ( struct MemDescr *)((char*)tmp + offs);
        if (prev) {
            prev->next = newNode;
        }
        else free_head = newNode;
        newNode->next = tmp->next;
        newNode->size = rem - MEM_BLOCK_SIZE;
    }else {                                          //nema ostatka taman se uklapa
        if (prev) prev->next = tmp->next;
        else free_head = tmp->next;
    }
    tmp->next = nullptr;

    return (char*)((uint64)tmp + MEM_BLOCK_SIZE);

}

void MemoryAllocator::tryToJoin(MemDescr *cur) {

    if(cur->next && (char*)(cur->next) == ((char*)cur+cur->size+MEM_BLOCK_SIZE)){
        cur->size+=cur->next->size + MEM_BLOCK_SIZE;
        cur->next=cur->next->next;
    }

}
int MemoryAllocator::m_free(void *adr) {
    if((uint64 )adr > (uint64)HEAP_END_ADDR-MEM_BLOCK_SIZE || (uint64 *)adr<(uint64 *)HEAP_START_ADDR)return -1;
    MemDescr * prev= nullptr;
    if(free_head==nullptr || (char*)adr<(char*)free_head) prev = nullptr;
    else{
        for(prev=free_head;prev->next!= nullptr && (char*)adr>(char*)(prev->next);prev=prev->next);
    }
    MemDescr * seg = (MemDescr *) ((uint64)adr - MEM_BLOCK_SIZE);

    if(prev)seg->next=prev->next;
    else {
        seg->next = free_head;
    }

    if(prev)prev->next=seg;
    else free_head=seg;
    if(seg!=nullptr)tryToJoin(seg);
    if(prev!=nullptr)tryToJoin(prev);

    return 0;
}