#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "manager.h"

#define get_2d(a, w, row, column) ((a)[(row) * (w) + (column)])

uint32_t log2i(uint32_t x) {
    if (x < 0) { return 0; }
    return sizeof(uint32_t) * 8 - __builtin_clz(x) - 1;
}


uint32_t get_last_n_bits(uint32_t value, uint32_t n) {
    uint32_t mask;
    mask = (uint32_t) ((1 << n) - 1);
    return value & mask;
}


uint32_t get_mid_n_bits(uint32_t value, uint32_t n, uint32_t offset) {
    value >>= offset;
    return get_last_n_bits(value, n);
}

int32_t get_lru_time(manager_t *self, uint32_t page) {
    if ((self->_access_table_row_size)[page] < self->_lru_parameter) {
        return get_2d(self->_access_table, self->_lru_parameter, page, 0);
    } else {
        return get_2d(self->_access_table, self->_lru_parameter, page, self->_lru_parameter - 1);
    }
}

void add_access(manager_t *self, uint32_t page) {
    int i;
    for (i = self->_lru_parameter - 1; i > 0; --i) {
        get_2d(self->_access_table, self->_lru_parameter, page, i) = get_2d(self->_access_table,
                                                                            self->_lru_parameter, page, i - 1);
    }
    get_2d(self->_access_table, self->_lru_parameter, page, 0) = ++(self->_current_timestamp);
    if ((self->_access_table_row_size)[page] < self->_lru_parameter)(self->_access_table_row_size)[page] += 1;
}

int32_t get_free_frame(manager_t *self) {
    int i = 0;
    for (i = 0; i < self->_frame_num; ++i) {
        if ((self->_frame_table)[i] == -1) return i;
    }
    return -1;
}

// Instantiate a manager_t
manager_t *new_memory_manager(uint32_t page_num, uint32_t frame_num, uint32_t frame_size, uint32_t lru_parameter) {
    manager_t *self = malloc(sizeof(manager_t));
    self->_page_num = page_num;
    self->_frame_num = frame_num;
    self->_frame_size = frame_size;
    self->_lru_parameter = lru_parameter;
    // TODO: initiate other members you add
    self->_page_table = malloc(sizeof(int32_t) * page_num);
    int i;
    for (i = 0; i < page_num; ++i) {
        (self->_page_table)[i] = -1;
    }
    // memset(self->_page_table, -1, page_num);
    self->_frame_table = malloc(sizeof(int32_t) * frame_num);
    for (i = 0; i < frame_num; ++i) {
        (self->_frame_table)[i] = -1;
    }
    //memset(self->_frame_table, -1, frame_num);
    self->_access_table = malloc(sizeof(int32_t) * page_num * lru_parameter);
    for (i = 0; i < page_num * lru_parameter; ++i) {
        (self->_access_table)[i] = -1;
    }
    //memset(self->_page_table, -1, page_num * lru_parameter);
    self->_access_table_row_size = malloc(sizeof(int32_t) * page_num);
    for (i = 0; i < page_num; ++i) {
        (self->_access_table_row_size)[i] = 0;
    }
    //memset(self->_access_table_row_size, 0, page_num);
    self->_current_timestamp = -1;
    return self;
}

// Free manager_t
void deconstruct_manager(manager_t *self) {
    // TODO: free other members you add if in need
    free(self->_page_table);
    free(self->_access_table);
    free(self->_access_table_row_size);
    free(self);
}

// TODO: return the physical address of the logical address 
uint32_t access(manager_t *self, uint32_t addr) {
    uint32_t offset = get_last_n_bits(addr, log2i(self->_frame_size));
    uint32_t page = get_mid_n_bits(addr, log2i(self->_page_num), log2i(self->_frame_size));
    if ((self->_page_table)[page] != -1) { //page already have a frame
        add_access(self, page);
        return (uint32_t) (self->_page_table)[page];
    }
    //page don't have a frame yet, try find a frame
    uint32_t address;
    int32_t frame = get_free_frame(self);
    if (frame != -1) {      //found free frame, great!
        (self->_frame_table)[frame] = page;
        (self->_page_table)[page] = frame;
        add_access(self, page);
        address = offset | (frame<<log2i(self->_frame_size));
    } else {     //no free frame, damn it I have to code many more lines
        uint32_t lru_page = get_lru_page(self);  //find a lru page
        uint32_t lru_frame = (uint32_t) (self->_page_table)[lru_page];
        (self->_page_table)[lru_page] = -1; //remove the page from frame
        (self->_page_table)[page] = lru_frame; //swap the frame with the new accessed page
        (self->_frame_table)[lru_frame] = page;
        add_access(self, page);
        address = offset | (lru_frame<<log2i(self->_frame_size));
    }
    return address;

}

uint32_t get_lru_page(manager_t *self) {
    uint32_t i;
    int32_t lru_time = -1;
    uint32_t lru_page = 0;
    for (i = 0; i < self->_frame_num; ++i) {
        int32_t time = get_lru_time(self, (uint32_t) (self->_frame_table)[i]);
        if (lru_time == -1 || time < lru_time) {
            lru_time = time;
            lru_page = i;
        }
    }
    return lru_page;
}



