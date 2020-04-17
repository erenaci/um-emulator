/**************************************************************
*     Assignment: um
*     Authors: Isaac Hudis, Erena Inoue
*     Date: 04/14/20
*     File: instructions.c
*     Summary: Implementation of instructions class
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "seq.h"
#include "instructions.h"
#include "stack.h"

/*
 *  conditional_move
 *
 *  Function: implements the conditional move instruction. If $r[rc] is not 0 it
 *  moves contents of $r[rb] into $r[rc].
 *  Input: registers ra, rb, rc and um_memory struct
 *  Output: none 
 *  Expectations: it is a checked runtime error to pass in a null um_memory 
 *  struct, as well as register values that are not between 0 and 7. 
 */
void conditional_move(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem)
{
    assert(mem);
    assert(ra <= 7);
    assert(rb <= 7);
    assert(rc <= 7);
    if(mem->registers[rc] != 0) 
      mem->registers[ra] = mem->registers[rb];
}

/*
 *  segment_load
 *
 *  Function: implements the segmented load instruction for UM. This gets the 
 *  value stored at $m[$r[rb]]$r[rc]] and stores it in $r[ra]. 
 *  Input: register values ra, rb and rc, as well as um_memory struct um.
 *  Output: none 
 *  Expectations: it is a checked runtime error to pass a null um_memory struct,
 *  as well as register values that are not within the correct bounds (0 - 7).
 *  It will not give a checked runtime error in the case that an unmapped 
 *  segment is being requested, or if the index is not in bounds inside the 
 *  given segment, and so this will result in failure and undefined behaviour.
 */
void segment_load(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem)
{
    assert(mem);
    assert(ra <= 7);
    assert(rb <= 7);
    assert(rc <= 7);
    Seq_T segment = Seq_get(mem->identifiers, mem->registers[rb]);
    uint32_t value_at_index = *(uint32_t *)Seq_get(segment, mem->registers[rc]);
    mem->registers[ra] = value_at_index;
}

/*
 *  segment_store
 *
 *  Function: this implements the segmented store instruction for UM. It places
 *  the value currently in $r[rc] in the location $m[$r[rb][$r[rc]] in memory.
 *  Input: register values ra, rb, rc, as well as um_memory struct mem.
 *  Output: none 
 *  Expectations: is a checked runtime error to pass in a null um_memory struct,
 *  as well as register values that are outside the correct bounds of 0-7. 
 *  Requesting an unmapped segment or an index that is outside of the bounds
 *  of a segment will result in failure and undefined behaviour, similar to 
 *  segmented load.
 */
void segment_store(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem)
{
    assert(mem);
    assert(ra <= 7);
    assert(rb <= 7);
    assert(rc <= 7);
    uint32_t value_to_add = mem->registers[rc];
    uint32_t *ptr = malloc(sizeof(uint32_t));
    assert(ptr);
    *ptr = value_to_add;
    Seq_T segment = Seq_get(mem->identifiers, mem->registers[ra]);
    uint32_t *to_free = Seq_get(segment, mem->registers[rb]);
    free(to_free);
    Seq_put(segment, mem->registers[rb], ptr);
  
}

/*
 *  add
 *
 *  Function: this implements the add instruction for UM. It adds the value in 
 *  $r[rb] to the value in $r[rc] and places the result in $r[ra].
 *  Input: register values ra, rb, rc, as well as um_memory struct mem.
 *  Output: none 
 *  Expectations: it is a checked runtime error to pass in a null um_memory 
 *  struct, as well as register values that are outside the correct bounds of 
 *  0-7. 
 */
void add(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem)
{
    assert(mem);
    assert(ra <= 7);
    assert(rb <= 7);
    assert(rc <= 7);
    mem->registers[ra] = mem->registers[rb] + mem->registers[rc];
}

/*
 *  mulitply
 *
 *  Function: this implements the multiply instruction for UM. It multiplies
 *  the value in $r[rb] to $r[rc] and places the result in $r[ra].
 *  Input: register values ra, rb, rc, as well as um_memory struct mem.
 *  Output: none 
 *  Expectations: it is a checked runtime error to pass in a null um_memory 
 *  struct, as well as register values that are outside the correct bounds of 
 *  0-7. 
 */
void multiply(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem)
{
    assert(mem);
    assert(ra <= 7);
    assert(rb <= 7);
    assert(rc <= 7);
    mem->registers[ra] = mem->registers[rb] * mem->registers[rc];
}

/*
 *  divide
 *
 *  Function: this implements the divide instruction for UM. It divides the
 *  value in $r[rb] by the value in $r[rc] and places result in $r[ra].
 *  Input: register values ra, rb, rc, as well as um_memory struct mem.
 *  Output: none 
 *  Expections: it is a checked runtime error to pass in a null um_memory 
 *  struct, as well as register values that are outside the correct bounds of 
 *  0-7. It will be a checked runtime error to divide by zero.
 */
void divide(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem)
{
    assert(mem);
    assert(ra <= 7);
    assert(rb <= 7);
    assert(rc <= 7);
    assert(mem->registers[rc] != 0);
    mem->registers[ra] = mem->registers[rb] / mem->registers[rc];
}

/*
 *  bit_nand
 *
 *  Function: this implements the bitwise NAND instruction for UM. It does a
 *  nand on the values in $r[rb] and $r[rc] and places result in $r[ra].
 *  Input: register values ra, rb, rc, as well as um_memory struct mem.
 *  Output: none 
 *  Expections: it is a checked runtime error to pass in a null um_memory 
 *  struct, as well as register values that are outside the correct bounds of 
 *  0-7. 
 */
void bit_nand(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem)
{
    assert(mem);
    assert(ra <= 7);
    assert(rb <= 7);
    assert(rc <= 7);
    mem->registers[ra] = ~(mem->registers[rb] & mem->registers[rc]);
}

/*
 *  halt
 *
 *  Function: this implements the halt instruction for the UM. It calls a 
 *  free_memory function that frees all memory associated with the program.
 *  Input: um_memory struct mem.
 *  Output: none
 *  Expections: it is a checked runtime error to pass in a null um_memory 
 *  struct.
 */
void halt(um_memory mem)
{
    assert(mem);
    free_memory(mem);
}

/*
 *  map
 *
 *  Function: this implements the map instruction for UM. It maps a new 
 *  segment with a number of words equal to value in $r[rc]. It then places
 *  the new identifier in $r[rb]. Calls a helper function to implement the 
 *  details of mapping.
 *  Input: register values rb, rc, as well as um_memory struct mem.
 *  Output: none 
 *  it is a checked runtime error to pass in a null um_memory struct, as well 
 *  as register values that are outside the correct bounds of 0-7. 
 */
void map(um_memory mem, uint32_t rb, uint32_t rc)
{
    assert(mem);
    assert(rb <= 7);
    assert(rc <= 7);
    map_segment(mem->registers[rc], rb, mem);
}

/*
 *  unmap
 *
 *  Function: this implements the unmap instruction for UM. It unmaps the 
 *  segment given by the value in $r[rc], by callling a helper function to
 *  implement the details.
 *  Input: register value rc, as well as um_memory struct mem.
 *  Output: none 
 *  Expections: it is a checked runtime error to pass in a null um_memory 
 *  struct, as well as register values that are outside the correct bounds of 
 *  0-7. 
 */
void unmap(um_memory mem, uint32_t rc)
{
    assert(mem);
    assert(rc <= 7);
    unmap_segment(rc, mem);
}

/*
 *  output
 *
 *  Function: this implements the output instruction for UM. It prints the 
 *  value in $r[rc] to the I/O device.
 *  Input: register value rc, as well as um_memory struct mem.
 *  Output: none
 *  Expections: it is a checked runtime error to pass in a null um_memory 
 *  struct, as well as register values that are outside the correct bounds 
 *  of 0-7. It is also a checked runtime error to try and output a value that 
 *  is greater than 255 (outside the range of ASCII).
 */
void output(um_memory mem, uint32_t rc)
{
    assert(mem->registers[rc] < 256);
    assert(mem);
    assert(rc <= 7);
    putchar(mem->registers[rc]); 
}

/*
 *  input
 *
 *  Function: this implements the input instruction for the UM. It reads one 
 *  character from the I/O device and checks if it is the end of stream
 *  indicator, in which case it places the bit value of all 1s in $r[rc] and
 *  ends. Otherwise, it stores the character read in in $r[rc].
 *  Input: register value rc, as well as um_memory struct mem.
 *  Output: it is a checked runtime error to pass in a null um_memory struct,
 *  as well as register values that are outside the correct bounds of 0-7. It
 *  is also a checked runtime error to try and read in a character that is 
 *  outisde the range of ASCII.
 */
void input(um_memory mem, uint32_t rc)
{
    assert(mem);
    assert(rc <= 7);
    int input = getchar();
    if (input == ~0) {
      uint32_t end_value = ~0;
      mem->registers[rc] = end_value;
    } else {
      assert(input >= 0 && input <= 255);
      mem->registers[rc] = input;
  }
}

/*
 *  load_program
 *
 *  Function: this implements the load_program instruction for UM. It takes 
 *  the memory in segment $m[$r[rb]], duplicates it, and places it in segment
 *  0, replacing the old program that was stored. It then sets the program 
 *  counter to $m[0][$r[rc]].
 *  Input: register values rb, rc, as well as um_memory struct mem.
 *  Output: none 
 *  Expections: it is a checked runtime error to pass in a null um_memory 
 *  struct, as well as register values that are outside the correct bounds of 
 *  0-7. If value in $r[rb] is 0, this function will be very quick as it will 
 *  only update the program counter and then end. 
 */
void load_program(um_memory mem, uint32_t rb, uint32_t rc)
{
    assert(mem);
    assert(rb <= 7);
    assert(rc <= 7);
    if (mem->registers[rb] == 0) {
      mem->program_counter_seg = 0;
      mem->program_counter_index = mem->registers[rc];
      return ;
    } else {
      Seq_T seq_zero = Seq_get(mem->identifiers, 0);
      for (int i = 0; i < Seq_length(seq_zero); i++) {
        uint32_t *to_free = Seq_get(seq_zero, i);
        free(to_free);
      }
      Seq_free(&seq_zero);
      Seq_T new_program = Seq_get(mem->identifiers, mem->registers[rb]);
      Seq_T duplicate = Seq_new(Seq_length(new_program));
      for (int i = 0; i < Seq_length(new_program); i++) {
        uint32_t *temp = malloc(sizeof(uint32_t));
        assert(temp);
        *temp = *(uint32_t *)Seq_get(new_program, i);
        Seq_addhi(duplicate, (void *)temp);
      }
      Seq_put(mem->identifiers, 0, duplicate);
      mem->program_counter_seg = 0;
      mem->program_counter_index = mem->registers[rc];
  }
}


/*
 *  load_value
 *
 *  Function: this implements the load_value instruction for UM. It places the 
 *  provided value into $r[ra].
 *  Input: register value ra, as well as um_memory struct mem and uin32_t 
 *  value.
 *  Output: none 
 *  Expections: it is a checked runtime error to pass in a null um_memory 
 *  struct, as well as register values that are outside the correct bounds of 
 *  0-7. 
 */
void load_value(um_memory mem, uint32_t ra, uint32_t value)
{
    assert(mem);
    assert(ra <= 7);
    mem->registers[ra] = value;
}
