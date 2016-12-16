// //////////////////////////////////////////////////////////////////
// File: xlibrary.h
// Author: ZDHull
// Date: 2016/10/26
// //////////////////////////////////////////////////////////////////

#ifndef _xLibrary_
#define _xLibrary_

// Define size of memory
#define MEM_SIZE 65536

#include <cstring>
#include <getopt.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <bitset>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

// Uncomment for more output to terminal
//#define DEBUG

// Create enumerated types for instructions
enum Latency {ADD, SUB, AND, NOR, DIV, MUL, MOD, EXP};
enum Instruction_Name {N_ADD, N_SUB, N_AND, N_NOR, N_DIV, N_MUL, N_MOD, N_EXP, N_LW, N_SW, N_LIZ, N_LIS, N_LUI, N_BP, N_BN, N_BX, N_BZ, N_JR, N_JAL, N_J, N_HALT, N_PUT};

// Public Functions
void get_opcode(unsigned short int inst, unsigned short int * op);

short int x_add(short int inst);
short int x_sub(short int inst);
short int x_and(short int inst);
short int x_nor(short int inst);
short int x_div(short int inst);
short int x_mul(short int inst);
short int x_mod(short int inst);
short int x_exp(short int inst);
short int x_lw(short int inst);
short int x_sw(short int inst);
short int x_liz(short int inst);
short int x_lis(short int inst);
short int x_lui(short int inst);
short int x_bp(short int inst);
short int x_bn(short int inst);
short int x_bx(short int inst);
short int x_bz(short int inst);
short int x_jr(short int inst);
short int x_jalr(short int inst);
short int x_j(short int inst);
short int x_halt(short int inst);
short int x_put(short int inst);

#endif
