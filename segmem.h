/**************************************************************
*     Assignment: um
*     Authors: Isaac Hudis, Erena Inoue
*     Date: 04/14/20
*     File: segmem.h
*     Summary: Interface of segmem module
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "seq.h"
#include "stack.h"
#include <assert.h>
#include <stdint.h>

struct um_memory {
  Seq_T identifiers;
  uint32_t *registers;
  Stack_T reusable_mem;
  uint32_t program_counter_seg;
  uint32_t program_counter_index;
};

typedef struct um_memory *um_memory;

um_memory initialize_memory();
void free_memory(um_memory mem);


void map_segment(uint32_t words, uint32_t register_index, um_memory mem);
void unmap_segment(uint32_t register_index, um_memory mem);




 