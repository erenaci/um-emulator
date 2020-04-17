/**************************************************************
*     Assignment: um
*     Authors: Isaac Hudis, Erena Inoue
*     Date: 04/14/20
*     File: instructions.h
*     Summary: Interface of instructions module
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "seq.h"
#include "stack.h"
#include <assert.h>
#include <stdint.h>
#include "segmem.h"

/* conditional move instruction */
void conditional_move(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem);

/* segmented load instruction */
void segment_load(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem);

/* segmented store instruction */
void segment_store(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem);

/* add instruction */
void add(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem);

/* multiply instruction */
void multiply(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem);

/* divide instruction */
void divide(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem);

/* bitwise nand instruction */
void bit_nand(uint32_t ra, uint32_t rb, uint32_t rc, um_memory mem);

/* halt instruction */
void halt(um_memory mem);

/* map instruction */
void map(um_memory mem, uint32_t rb, uint32_t rc);

/* unmap instruction */
void unmap(um_memory mem, uint32_t rc);

/* output instruction */
void output(um_memory mem, uint32_t rc);

/* input instruction */
void input(um_memory mem, uint32_t rc);

/* load_porgram instruction */
void load_program(um_memory mem, uint32_t rb, uint32_t rc);

/* load_value instruction */
void load_value(um_memory mem, uint32_t ra, uint32_t value);