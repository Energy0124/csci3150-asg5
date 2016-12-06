#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "manager.h"


uint32_t log2i(uint32_t x) {
    if (x < 0) { return 0; }
    return sizeof(uint32_t) * 8 - __builtin_clz(x) - 1;
}


uint32_t getLastNBits(uint32_t value, uint32_t n) {
    uint32_t mask;
    mask = (uint32_t) ((1 << n) - 1);
    return value & mask;
}


uint32_t getMidNBits(uint32_t value, uint32_t n, uint32_t offset) {
    value>>=offset;
    return getLastNBits(value, n);
}


// Instantiate a manager_t
manager_t *new_memory_manager(uint32_t page_num, uint32_t frame_num, uint32_t frame_size, uint32_t lru_parameter) {
    manager_t *self = malloc(sizeof(manager_t));
    self->_page_num = page_num;
    self->_frame_num = frame_num;
    self->_frame_size = frame_size;
    self->_lru_parameter = lru_parameter;
    // TODO: initiate other members you add
    return self;
}

// Free manager_t
void deconstruct_manager(manager_t *self) {
    free(self);
    // TODO: free other members you add if in need
}

// TODO: return the physical address of the logical address 
uint32_t access(manager_t *self, uint32_t addr) {
    printf("%d\n", log2i(self->_frame_size));
    return 0;
}
