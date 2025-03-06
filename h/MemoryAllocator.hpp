
#ifndef PROJECTBASE_MEMORYALLOCATOR_HPP
#define PROJECTBASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

typedef struct MemDescr {
    size_t size;
    struct MemDescr *next;
} MemDescr;

class MemoryAllocator{
public:
    static struct MemDescr *free_head;
    static uint8* end;
    static uint8* base;

public:
    static void init_mem();
    static void* m_alloc(size_t size);
    static int m_free(void* adr);

    static void tryToJoin(MemDescr *cur);
};


#endif //PROJECTBASE_MEMORYALLOCATOR_HPP
