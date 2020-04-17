/**************************************************************
*     Assignment: um
*     Authors: Isaac Hudis, Erena Inoue
*     Date: 04/14/20
*     File: filereader.c
*     Summary: Implementation of filereader module
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "filereader.h"

const int BYTE = 8;
const int OPCODE = 4;
const int OPCODE_LSB = 28;
const unsigned LOAD_VALUE = 13;
const int REGISTER_WIDTH = 3;
const int LV_LSB = 25;

static uint32_t decode_instruction(um_memory mem, uint32_t *instruction);
static void call_instruction(uint32_t opcode, uint32_t local_instruc, 
                             um_memory mem);

 /* 
 *  read_file
 *
 *  Function: Reads an input um file and store the program in segment
 *  zero by bit-packing each instruction into a 32-bit word. 
 *  Input: um_memory mem, FILE *fp
 *  Output: None
 *  Expections: Will rasie a CRE if file pointer is NULL and if
 *  allocating memory is unsuccesful.
 */
void read_file(um_memory mem, FILE *fp)
{
    assert(fp);
    Seq_T seg_zero = Seq_get(mem->identifiers, 0);
    int c;
    uint32_t temp = 0;
    c = getc(fp);
    while (feof(fp) == 0 && c != EOF) {
        uint32_t *word = malloc(sizeof(uint32_t));
        assert(word);
        for (int i = 0; i < 4; i++) {
          if (c == EOF)
            break;
          if (i == 0)
            temp = Bitpack_newu(temp, BYTE, 24, c);
          else if (i == 1)
            temp = Bitpack_newu(temp, BYTE, 16, c);
          else if (i == 2) 
            temp = Bitpack_newu(temp, BYTE, 8, c);
          else if (i == 3) 
            temp = Bitpack_newu(temp, BYTE, 0, c);
        
          c = getc(fp);
        }  
        *word = temp;
        Seq_addhi(seg_zero, word);
    }
}

 /* 
 *  get_next_instruction
 * 
 *  Function: Gets the next instruction word to call from identifers sequence
 *  and calls decode_function which will call a coresponding instruction
 *  function.
 *  Input: um_memory mem
 *  Output: returns opcode in uint32_t format
 *  Expections: Will rasie a CRE if mem is NULL.
 */
uint32_t get_next_instruction(um_memory mem)
{
    assert(mem);
    Seq_T seg = Seq_get(mem->identifiers, mem->program_counter_seg);
    uint32_t *next_instruction = Seq_get(seg, mem->program_counter_index);
    return decode_instruction(mem, next_instruction);
}

 /* 
 *  decode_function (Private Helper Function)
 * 
 *  Function: Unpack the instruction word obtained from get_next_instruction
 *  function and gets the opcode needed. If the opcode turned out to be 13,
 *  or LOAD_VALUE, then unpack the insturction word in this function and
 *  calls the load_value instruction function. Else, call call_instruction
 *  function for all other opcode. 
 *  Input: um_memory mem, uint32_t *instruction
 *  Output: returns opcode in uint32_t format
 *  Expections: Will rasie a CRE if mem and/or instruction are NULL. Also
 *  raises a CRE if the obtained opcode is out of bounds. 
 */
static uint32_t decode_instruction(um_memory mem, uint32_t *instruction)
{
    assert(mem);
    assert(instruction);
    uint32_t local_instruc = *instruction;
    uint32_t opcode = Bitpack_getu(local_instruc, OPCODE, OPCODE_LSB);
    assert(opcode <= 13);
    if (opcode == LOAD_VALUE) {
      uint32_t ra = Bitpack_getu(local_instruc, REGISTER_WIDTH, LV_LSB);
      uint32_t value = Bitpack_getu(local_instruc, LV_LSB, 0);
      load_value(mem, ra, value);
      mem->program_counter_index++;
    } else {
      call_instruction(opcode, local_instruc, mem);
    }
    return opcode;
}

/* 
*  call_instruction (Private Helper Function)
*
*  Function: Unpack the local_instruc word ot obtain the necessary 
*  registers to call instruction functions with. Then, calls an 
*  instruction function that corresponds with the specified opcode.
*  Finally, it increments the program counter, unless it is callling
*  halt or load_program.
*  Input: uint32_t opcode, uint32_t local_instruc, um_memory mem
*  Output: None
*  Expections: Will rasie a CRE if mem is NULL. 
*/
static void call_instruction(uint32_t opcode, uint32_t local_instruc, 
                             um_memory mem)
{
    assert(mem);
    uint32_t ra = Bitpack_getu(local_instruc, REGISTER_WIDTH, 6);
    uint32_t rb = Bitpack_getu(local_instruc, REGISTER_WIDTH, 3);
    uint32_t rc = Bitpack_getu(local_instruc, REGISTER_WIDTH, 0);
    if (opcode == 0) 
      conditional_move(ra, rb, rc, mem);
    else if (opcode == 1)
      segment_load(ra, rb, rc, mem);
    else if (opcode == 2)
      segment_store(ra, rb, rc, mem);
    else if (opcode == 3)
      add(ra, rb, rc, mem);
    else if (opcode == 4)
      multiply(ra, rb, rc, mem);
    else if (opcode == 5) 
      divide(ra, rb, rc, mem);
    else if (opcode == 6)
      bit_nand(ra, rb, rc, mem);
    else if (opcode == 7) {
      halt(mem);
      return ;
    } else if (opcode == 8)
      map(mem, rb, rc);
    else if (opcode == 9)
      unmap(mem, rc);
    else if (opcode == 10)
      output(mem, rc);
    else if (opcode == 11) 
      input(mem, rc);
    else if (opcode == 12) {
      load_program(mem, rb, rc);
      return ;
    }
    mem->program_counter_index++;
}