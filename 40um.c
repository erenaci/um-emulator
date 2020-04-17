/**************************************************************
*     Assignment: um
*     Authors: Isaac Hudis, Erena Inoue
*     Date: 04/14/20
*     File: 40um.c
*     Summary:
*     This file contains the main function that emulates the 
*     UM.
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitpack.h"
#include "filereader.h"
#include <assert.h>
#include <stdint.h>

const unsigned HALT = 7;

int main(int argc, char *argv[])
{
   FILE *src;
   if(argc != 2) {
     fprintf(stderr, "Program called incorrectly, usage: ./um [input_file]");
     exit(EXIT_FAILURE);
   }

   um_memory memory = initialize_memory();
   
   src = fopen(argv[1], "r");
   assert(src);
   read_file(memory, src);
   fclose(src);

   uint32_t opcode = 0;
   while (opcode != HALT) {
     opcode = get_next_instruction(memory);
   }
   
   exit(EXIT_SUCCESS);
}
 
 