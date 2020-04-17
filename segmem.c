/**************************************************************
 *     Assignment: um
 *     Authors: Isaac Hudis, Erena Inoue
 *     Date: 04/14/20
 *     File: segmem.c
 *     Summary: Implementation of segmem module 
 **************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "segmem.h"
#include <stdint.h>

const int REGISTERS = 8;

/* 
*  initialize_memory
* 
*  Function: Initialize all elements in the um_memory struct
*  Input: None
*  Output: initialized um_memory struct
*  Expections: none, but will raise CRE when allocating a memory 
*  is unsuccessful.
*/
um_memory initialize_memory()
{
   um_memory memory = malloc(sizeof(struct um_memory));
   assert(memory);
   memory->identifiers = Seq_new(8);
   memory->registers = malloc(REGISTERS * sizeof(uint32_t));
   assert(memory->registers);
   memory->reusable_mem = Stack_new();
   memory->program_counter_seg = 0;
   memory->program_counter_index = 0;

   uint32_t initial_value = 0;
   for (int i = 0; i < REGISTERS; i++) {
     memory->registers[i] = initial_value;
   }
   
   Seq_addhi(memory->identifiers, Seq_new(8));
   return memory;
}

 /* 
 *  map_segment
 * 
 *  Function: creates a new segment with a given number of words. Each
 *  word in the new segment is initialized to zero and reuse identifiers 
 *  from the stack if it is not emepty. Else, it will add the newly
 *  mapped segments to the back of identifers sequence. 
 *  Input: uint32_t words, uint32_t register_index, um_memory mem
 *  Output: None
 *  Expectations: Will raise CRE if mem is NULL and if register_index
 *  is out of bounds. Also will raise CRE if the allocating memory is
 *  unsuccessful.
 */
void map_segment(uint32_t words, uint32_t register_index, um_memory mem) 
{
    assert(register_index <= 7);
    assert(mem);
    Seq_T toAdd = Seq_new(words);
    for(uint32_t i = 0; i < words; i++) {
      uint32_t *initial_value = malloc(sizeof(uint32_t));
      assert(initial_value);
      *initial_value = 0;
      Seq_addhi(toAdd, initial_value); 
    }  
    if (Stack_empty(mem->reusable_mem) == 0) {
      uint32_t *index_ptr = (uint32_t *)Stack_pop(mem->reusable_mem);
      uint32_t index = *index_ptr;
      free(index_ptr);
      Seq_T old_seq = Seq_get(mem->identifiers, index);
      for (int i = 0; i < Seq_length(old_seq); i++) {
        uint32_t *temp = Seq_get(old_seq, i);
        free(temp);
      }
      Seq_free(&old_seq);
      Seq_put(mem->identifiers, index, toAdd);
      mem->registers[register_index] = index;
    } else {
      Seq_addhi(mem->identifiers, toAdd);
      uint32_t new_index = Seq_length(mem->identifiers) - 1;
      mem->registers[register_index] = new_index;
    }
}

 /* 
 *  unmap_segment
 * 
 *  Function: The specified segment is unmapped and that segment 
 *  identifier will be pushed to the stack so that it can be 
 *  reused in map later on. Does not immediately free the memory, 
 *  this occurs in mapped later if the segment identifier is reused.
 *  Input: uint32_t register_index, um_memory mem
 *  Output: None
 *  Expectations: Will raise CRE if mem is NULL and if register_index
 *  is out of bounds. Also will raise CRE if the allocating memory is
 *  unsuccessful.
 */
void unmap_segment(uint32_t register_index, um_memory mem)
{
    assert(register_index <= 7);
    assert(mem);
    uint32_t value = mem->registers[register_index];
    uint32_t *ptr = malloc(sizeof(uint32_t));
    assert(ptr);
    *ptr = value;
    Stack_push(mem->reusable_mem, ptr);
}

 /* 
 *  free_memory
 * 
 *  Function: Frees all allocated memory
 *  Input: um_memory mem
 *  Output: None
 *  Expectations: Will raise CRE if mem is NULL. 
 */
void free_memory(um_memory mem)
{
    assert(mem);
    free(mem->registers);
    for (int i = 0; i < Seq_length(mem->identifiers); i++) {
      Seq_T toFree = Seq_get(mem->identifiers, i);
      for (int j = 0; j < Seq_length(toFree); j++) {
        uint32_t *temp = Seq_get(toFree, j);
        free(temp);
      }
      Seq_free(&toFree);
    }
    Seq_free(&(mem->identifiers));
    while (Stack_empty(mem->reusable_mem) != 1) {
      uint32_t *temp = Stack_pop(mem->reusable_mem);
      free(temp);
    }
    Stack_free(&(mem->reusable_mem));
    free(mem);
}