#pragma once

#include <stdint.h>
#include <math.h>

// Define a structure named 'manager_t'  
// !!! Do NOT modify the name of 'manager_t'
// The suffix '_t' identifies that this is a structure
typedef struct {
    uint32_t _page_num; // page number
    uint32_t _frame_size; // frame size
    uint32_t _frame_num; // frame number
    uint32_t _lru_parameter; // value of parameter in LRU-parameter
    // TODO: add other members if you need to
    int32_t *_page_table;
    int32_t *_frame_table;
    int32_t *_access_table;
    int32_t *_access_table_row_size;
    int32_t _current_timestamp;
} manager_t;

// Instantiate a manager_t
manager_t *new_memory_manager(uint32_t page_num, uint32_t frame_num, uint32_t frame_size, uint32_t lru_parameter);

// Free manager_t
void deconstruct_manager(manager_t *self);

// TODO: return the physical address of the logical address 
uint32_t access(manager_t *self, uint32_t addr);

static inline uint32_t log2i(uint32_t x);

uint32_t get_last_n_bits(uint32_t value, uint32_t n);

uint32_t get_mid_n_bits(uint32_t value, uint32_t n, uint32_t offset);

void add_access(manager_t *self, uint32_t page);

int32_t get_lru_time(manager_t *self, uint32_t page);

int32_t get_free_frame(manager_t *self);

uint32_t get_lru_page(manager_t *self);




