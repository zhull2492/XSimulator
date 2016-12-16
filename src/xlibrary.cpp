// //////////////////////////////////////////////////////////////////
// File: xlibrary.cpp
// Description: Library of instruction implemented for the XSim
//              instruction set
// Author: ZDHull
// Date: 2016/10/26
// //////////////////////////////////////////////////////////////////

#include "xlibrary.h"

using namespace std;

// //////////////////////////////////////////
// Extern variables shared amoung files
extern short int inst_memory[MEM_SIZE/2];
extern char data_memory[MEM_SIZE];
extern short int reg_file[8];
extern short int program_counter;
extern int clock_cycles[22];
extern int latency_vals[8];
// //////////////////////////////////////////

// /////////////////////////////////////////////////////////////////
// Inputs: One 16-bit value
// Outputs: First 5 bits of input
// Description: This function parses out the opcode bits in an instruction
// /////////////////////////////////////////////////////////////////
void get_opcode(unsigned short int inst, unsigned short int * op) {

    *op = (inst >> 11) & 0x001F;
   
    return;
}

// //////////////////////////////////////////////////////////////////
// Inputs: One 16-Bit value
// Outputs: Three values corresponding to register numbers
// Description: This function parses out the three register vlaues from
//              an R-Type instruction. This function is private.
// //////////////////////////////////////////////////////////////////
void r_type_field(short int inst, short int * rd, short int * rs, short int * rt) {

    *rd = (inst >> 8) & 0x0007; 
    *rs = (inst >> 5) & 0x0007;
    *rt = (inst >> 2) & 0x0007;

    return;
}

// //////////////////////////////////////////////////////////////////
// Inputs: One 16-Bit value
// Outputs: Two values corresponding to register number and 8-Bit immediate
// Description: This function parses out the two values from
//              an I-Type instruction. This function is private.
// //////////////////////////////////////////////////////////////////
void i_type_field(short int inst, short int * rd, short int * imm8) {

    *rd = (inst >> 8) & 0x0007;
    *imm8 = inst & 0x00FF;

    return;
}

// //////////////////////////////////////////////////////////////////
// Inputs: One 16-Bit value
// Outputs: One value corresponding an 11-Bit immediate
// Description: This function parses out the one value from
//              an IX-Type instruction. This function is private.
// //////////////////////////////////////////////////////////////////
void ix_type_field(short int inst, int * imm11) {

    *imm11 = inst & 0x07FF;

    return;
}

// //////////////////////////////////////////////////////////////////
// XSim Library Functions
// //////////////////////////////////////////////////////////////////

short int x_add(short int inst) {
    short int rs, rt, rd;	// Integer values for registers

    // Get register numbers
    r_type_field(inst, &rd, &rs, &rt);

    // Perform addition
    reg_file[rd] = reg_file[rs] + reg_file[rt];

    // Increment Frequency count
    clock_cycles[N_ADD] += (1);
    cout << "ADD" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "RS + RT = RD" << endl << reg_file[rs] << " + " << reg_file[rt] << " = " << reg_file[rd] << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_sub(short int inst) {
    short int rs, rt, rd;	// Integer values for registers

    // Get register numbers
    r_type_field(inst, &rd, &rs, &rt);

    // Perfrom subtraction
    reg_file[rd] = reg_file[rs] - reg_file[rt];

    // Increment Frequency count
    clock_cycles[N_SUB] += (1);;
    cout << "SUB" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "RS - RT = RD" << endl << reg_file[rs] << " - " << reg_file[rt] << " = " << reg_file[rd] << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}
short int x_and(short int inst) {
    short int rs, rt, rd;	// Integer values for registers

    // Get register numbers
    r_type_field(inst, &rd, &rs, &rt);

    // Perfrom Bit-Wise ANDing
    reg_file[rd] = reg_file[rs] & reg_file[rt];

    // Increment Frequency Count
    clock_cycles[N_AND] += (1);
    cout << "AND" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "RS & RT = RD" << endl << bitset<16>(reg_file[rs]) << " & " << bitset<16>(reg_file[rt]) << " = " << bitset<16>(reg_file[rd]) << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_nor(short int inst) {
    short int rs, rt, rd;	// Integer values for registers

    // Get register numbers
    r_type_field(inst, &rd, &rs, &rt);

    // Perfrom Bit-Wise NORing (OR then NOT)
    reg_file[rd] = ~(reg_file[rs] | reg_file[rt]);

    // Increment Frequency Count
    clock_cycles[N_NOR] += (1);
    cout << "NOR" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "RS ~| RT = RD" << endl << bitset<16>(reg_file[rs]) << " ~| " << bitset<16>(reg_file[rt]) << " = " << bitset<16>(reg_file[rd]) << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_div(short int inst) {
    short int rs, rt, rd;	// Integer values for registers

    // Get register numbers
    r_type_field(inst, &rd, &rs, &rt);

    // Check for DIVIDE BY ZERO
    if (reg_file[rt] == 0) {
	cout << "Divide by 0 Error...Terminating\n";
	return (unsigned short int) -1;
    }

    // Perfrom Division
    reg_file[rd] = reg_file[rs] / reg_file[rt];

    // Increment Frequency count
    clock_cycles[N_DIV] += (1);
    cout << "DIV" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "RS / RT = RD" << endl << reg_file[rs] << " / " << reg_file[rt] << " = " << reg_file[rd] << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_mul(short int inst) {
    short int rs, rt, rd;	// Integer values for registers

    // Get register numbers
    r_type_field(inst, &rd, &rs, &rt);

    // Perform multiplication
    reg_file[rd] = reg_file[rs] * reg_file[rt];

    // Increment frequency count
    clock_cycles[N_MUL] += (1);
    cout << "MUL" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "RS * RT = RD" << endl << reg_file[rs] << " * " << reg_file[rt] << " = " << reg_file[rd] << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_mod(short int inst) {
    short int rs, rt, rd;	// Integer values for registers

    // Get register numbers
    r_type_field(inst, &rd, &rs, &rt);

    // Check for MOD BY ZERO error
    if (reg_file[rt] == 0){
	cout << "Cannot MOD by 0...terminating\n";
	return (unsigned short int) -1;
    }

    // Perform MODULUS division
    reg_file[rd] = reg_file[rs] % reg_file[rt];

    // Increment frequency count
    clock_cycles[N_MOD] += (1);
    cout << "MOD" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "RS mod RT = RD" << endl << reg_file[rs] << " mod " << reg_file[rt] << " = " << reg_file[rd] << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_exp(short int inst) {
    short int rs, rt, rd;	// Integer values for registers

    // Get register numbers
    r_type_field(inst, &rd, &rs, &rt);

    // perform exponentiation
    reg_file[rd] = (short int)pow(reg_file[rs], reg_file[rt]);

    // Increment Frequency count
    clock_cycles[N_EXP] += (1);
    cout << "EXP" << endl;

#ifdef DEBUG	
    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "RS ^ RT = RD" << endl << reg_file[rs] << " ^ " << reg_file[rt] << " = " << reg_file[rd] << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_lw(short int inst) {
    short int rs, rt, rd;		// Integer values for registers
    unsigned short int temp1, temp2; 	// temporary holders for half-words

    // Get register values
    r_type_field(inst, &rd, &rs, &rt);

    // Check for word-aligned address
    if ((unsigned short int)reg_file[rs] & 0x0001) {
	cout << "Address not word aligned...terminating" << endl;
	return (unsigned short int) -1;
    } 

    temp1 = 0;
    temp2 = 0;

    // Copy data from memory to temp variables
    memcpy(&temp1, &data_memory[((unsigned short int)reg_file[rs])], sizeof(char));
    memcpy(&temp2, &data_memory[((unsigned short int)reg_file[rs]+1)], sizeof(char));

    // Store memory value in register
    reg_file[rd] = (temp1 << 8) | temp2;
    
    // Increment frequency count
    clock_cycles[N_LW] += 1;
    cout << "LW" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "RD <- MEM[RS]" << endl << hex << reg_file[rd] << " <- " << (unsigned short int)data_memory[rs] << (unsigned short int)data_memory[rs+1] << dec << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}
short int x_sw(short int inst) {
    short int rs, rt, rd;	// Integer values for registers
    unsigned short int temp; 	// temporary value 

    // Get register numbers
    r_type_field(inst, &rd, &rs, &rt);

    // Check for word aligned address
    if ((unsigned short int)reg_file[rs] & 0x0001) {
	cout << "Address not word aligned...terminating" << endl;
#ifdef DEBUG
	cout << "RS: " << rs << endl << (unsigned short int)reg_file[rs] << endl;
#endif
	return (unsigned short int) -1;
    } 

    // Copy 8-Bits to temp and store to memory
    temp = (reg_file[rt] >> 8) & 0x00FF;
    memcpy(&data_memory[(unsigned short int)reg_file[rs]], &temp, sizeof(char));
    // Copy Next 8-Bits to temp and store to memory
    temp = (reg_file[rt]) & 0x00FF;
    memcpy(&data_memory[((unsigned short int)reg_file[rs])+1], &temp, sizeof(char));

    // Increment frequency count
    clock_cycles[N_SW] += 1;
    cout << "SW" << endl;

#ifdef DEBUG
    cout << (rs >> 1) << endl;

    cout << "RD: " << (rd) << endl;
    cout << "RS: " << (rs) << endl;
    cout << "RT: " << (rt) << endl;

    cout << "Reg1: " << hex << ((unsigned short int)(reg_file[rt] >> 8) & 0x00FF) << "\t" << (unsigned short int)data_memory[(unsigned short int)reg_file[rs]] << dec << endl;
    cout << "Reg2: " << hex << ((unsigned short int)reg_file[rt] & 0x00FF) << "\t" << (unsigned short int)data_memory[(unsigned short int)reg_file[rs]+1] << dec << endl;

    cout << "MEM[RS] <- RT" << endl << hex << (unsigned short int)data_memory[rs] << (unsigned short int)data_memory[rs+1] << " <- " << reg_file[rt] << dec << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_liz(short int inst) {
    short int rd, imm8;		// Integers values for a register and immediate

    // Get values
    i_type_field(inst, &rd, &imm8);

    // Zero-Extend immediate to 16-Bits
    reg_file[rd] = 0x00FF & imm8;

    // Increment frequency count
    clock_cycles[N_LIZ] += 1;
    cout << "LIZ" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "IMM8: " << (imm8) << endl;

    cout << "RD <- Z_EXT(IMM8)" << endl << (signed short int)reg_file[rd] << " <- " << (unsigned short int)imm8 << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_lis(short int inst) {
    short int rd;	// Integer value for register
    short int imm8;	// register value for immediate

    // Get register and immediate value
    i_type_field(inst, &rd, &imm8);

    // Check sign bit and extend it to 16-Bits
    if (imm8 >> 7) {
	reg_file[rd] = 0xFF00 | imm8;
    }
    else {
	reg_file[rd] = imm8 & 0x00FF;
    }

    // Increment frequency count
    clock_cycles[N_LIS] += 1;
    cout << "LIS" << endl;

#ifdef DEBUG
    cout << "RD: " << (rd) << endl;
    cout << "IMM8: " << (imm8) << endl;

    cout << "RD <- Z_EXT(IMM8)" << endl << (signed)reg_file[rd] << " <- " << (signed)imm8 << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_lui(short int inst) {
    short int rd, imm8;		// Value for register and immediate

    // Get register and immediate value
    i_type_field(inst, &rd, &imm8);

    // Increment frequency count
    clock_cycles[N_LUI] += 1;
    cout << "LUI" << endl;

#ifdef DEBUG
    cout << "RD: " << rd << endl;
    cout << "IMM8: " << bitset<8>(imm8) << endl;
    cout << "RD[7:0]: " << (bitset<8>(reg_file[rd] & 0x00FF)) << endl;
#endif

    // Concatenate immediate and register
    reg_file[rd] = (0xFF00 & (imm8 << 8)) | (0x00FF & reg_file[rd]);

#ifdef DEBUG
    cout << "RD <- IMM8:RD" << endl << bitset<16>(reg_file[rd]) << " <- " << bitset<8>(imm8) << bitset<8>(reg_file[rd] & 0x00FF) << endl;
#endif

    return (unsigned short int) (program_counter + 2);
}

short int x_bp(short int inst) {
    short int rd, imm8;			// Values for register and immediate
    unsigned short int next_addr;	// Value for next instruction address

    // Get register and immediate value
    i_type_field(inst, &rd, &imm8);

    // Check if value is positive
    if (reg_file[rd] > 0) {
	next_addr = 0x01FF & (imm8 << 1);
    }
    else {
	next_addr = program_counter + 2;
    }

    // Increment frequency count
    clock_cycles[N_BP] += 1;
    cout << "BP" << endl;

#ifdef DEBUG
    cout << "RD: " << rd << endl;
    cout << "IMM8: " << imm8 << endl;

    cout << "PC <- ($RD > 0) ? Z_EXT(imm8<<1) : PC + 2" << endl << hex << next_addr << " <- " << dec << reg_file[rd] << " ? " << hex << (unsigned short int)(imm8 << 1) << " : " << (program_counter + 2) << dec << endl;
#endif

    return (unsigned short int) next_addr;
}

short int x_bn(short int inst) {
    short int rd, imm8;			// Value for register and immediate
    unsigned short int next_addr;	// Value for next instruction address

    // Get register and immediate value
    i_type_field(inst, &rd, &imm8);

    // Check if value is negative
    if (reg_file[rd] < 0) {
	next_addr = 0x01FF & (imm8 << 1);
    }
    else {
	next_addr = program_counter + 2;
    }

    // Increment frequency count
    clock_cycles[N_BN] += 1;
    cout << "BN" << endl;

#ifdef DEBUG
    cout << "RD: " << rd << endl;
    cout << "IMM8: " << imm8 << endl;

    cout << "PC <- ($RD < 0) ? Z_EXT(imm8<<1) : PC + 2" << endl << hex << next_addr << " <- " << dec << reg_file[rd] << " ? " << hex << (unsigned short int)(imm8 << 1) << " : " << (program_counter + 2) << dec << endl;
#endif

    return (unsigned short int) next_addr;
}

short int x_bx(short int inst) {
    short int rd, imm8;			// Value of register and immediate
    unsigned short int next_addr;	// Value to next instruction address

    // Get register and immediate value
    i_type_field(inst, &rd, &imm8);

    // Check if register value is NOT EQUAL to zero
    if (reg_file[rd] != 0) {
	next_addr = 0x01FF & (imm8 << 1);
    }
    else {
	next_addr = program_counter + 2;
    }

    // Increment Frequency count
    clock_cycles[N_BX] += 1;
    cout << "BX" << endl;

#ifdef DEBUG
    cout << "RD: " << rd << endl;
    cout << "IMM8: " << imm8 << endl;

    cout << "PC <- ($RD ~= 0) ? Z_EXT(imm8<<1) : PC + 2" << endl << hex << next_addr << " <- " << dec << reg_file[rd] << " ? " << hex << (unsigned short int)(imm8 << 1) << " : " << (program_counter + 2) << dec << endl;
#endif

    return (unsigned short int) next_addr;
}

short int x_bz(short int inst) {
    unsigned short int next_addr;	// Value of next address instruction
    short int rd, imm8;			// Value of register and immediate

    // Get register and immediate value
    i_type_field(inst, &rd, &imm8);

    // Check if register value is EQUAL TO zero
    if (reg_file[rd] == 0) {
	next_addr = 0x01FF & (imm8 << 1);
    }
    else {
	next_addr = program_counter + 2;
    }

    // Increment Frequency count
    clock_cycles[N_BZ] += 1;
    cout << "BZ" << endl;

#ifdef DEBUG
    cout << "RD: " << rd << endl;
    cout << "IMM8: " << imm8 << endl;

    cout << "PC <- ($RD == 0) ? Z_EXT(imm8<<1) : PC + 2" << endl << hex << next_addr << " <- " << dec << reg_file[rd] << " ? " << hex << (unsigned short int)(imm8 << 1) << " : " << (program_counter + 2) << dec << endl;
#endif

    return (unsigned short int) next_addr;
}

short int x_jr(short int inst) {
    short int rs, rt, rd;		// Value for registers
    unsigned short int next_addr;	// Value of next instruction address

    // Get register values
    r_type_field(inst, &rd, &rs, &rt);

    // Get next instruction address for return
    next_addr = (unsigned short int)reg_file[rs];

    // Increment frequency count
    clock_cycles[N_JR] += 1;
    cout << "JR" << endl;

#ifdef DEBUG
    cout << "RS: " << rs << endl;
    
    cout << "PC <- RS" << endl << hex << next_addr << " <- " << (unsigned short int)reg_file[rs] << dec << endl;
#endif

    return (unsigned short int) next_addr;
}

short int x_jalr(short int inst) {
    short int rs, rt, rd;		// Value for registers
    unsigned short int next_addr;	// Value fo next instruction address

    // Get register values
    r_type_field(inst, &rd, &rs, &rt);

    // Save next instruciton address
    reg_file[rd] = program_counter + 2;
    // Get address to jump to
    next_addr = (unsigned short int)reg_file[rs];

    // Increment frequency count
    clock_cycles[N_JAL] += 1;
    cout << "JALR" << endl;

#ifdef DEBUG
    cout << "RD: " << rd << endl;
    cout << "RS: " << rs << endl;

    cout << "RD <- PC + 2 ; PC <- RS" << endl << hex << reg_file[rd] << " <- " << (program_counter + 2) << " ; " << next_addr << " <- " << reg_file[rs] << dec << endl;
#endif

    return (unsigned short int) next_addr;
}

short int x_j(short int inst) {
    int imm11;				// Value of immediate
    unsigned short int next_addr;	// Value of next address

    // Get value of immediate
    ix_type_field(inst, &imm11);

    // Get value of address to jump to by concatenating top bits of PC and immediate
    next_addr = (unsigned short int)(program_counter & 0xF000) | (unsigned short int)(imm11 << 1);

    // Increment Frequency Count
    clock_cycles[N_J] += 1;
    cout << "J" << endl;

#ifdef DEBUG
    cout << "IMM11: " << hex << imm11 << dec << endl;

    cout << "PC <- PC[15:12]..(IMM11<<1)" << endl << hex << ((program_counter >> 12) & 0x000F) << ".." << (imm11 << 1) << dec << endl;
#endif

    return (unsigned short int) next_addr;
}

short int x_halt(short int inst) {
    short int rs, rt, rd;		// Value for registers

    // Get values for registers
    r_type_field(inst, &rd, &rs, &rt);

    // Increment frequency count
    clock_cycles[N_HALT] = 1;
#if 1
    cout << "HALT" << endl;
#endif

    return (short int) 1;
}

short int x_put(short int inst) {
    short int rs, rt, rd;	// Value for registers

    // Get register values
    r_type_field(inst, &rd, &rs, &rt);

    // Increment frequency count
    clock_cycles[N_PUT] += 1;
    cout << "PUT" << endl;

#ifdef DEBUG
    cout << "RS: " << rs << endl;
#endif

    // Print value in register to STDOUT
    fprintf(stdout, "\t$R%d: %d\n", rs, reg_file[rs]);
    return (unsigned short int) (program_counter + 2);
}
