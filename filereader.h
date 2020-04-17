/**************************************************************
*     Assignment: um
*     Authors: Isaac Hudis, Erena Inoue
*     Date: 04/14/20
*     File: filereader.h
*     Summary: Interface of filereader module
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "seq.h"
#include "stack.h"
#include "instructions.h"
#include "bitpack.h"
#include <assert.h>
#include <stdint.h>

/* initial file reading */
void read_file(um_memory mem, FILE *fp);

/* obtains next instruction for decode and execute */
uint32_t get_next_instruction(um_memory mem);