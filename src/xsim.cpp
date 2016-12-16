// ////////////////////////////////////////////////////////
// File: xsim.cpp
// Author: ZDHull
// Date 2016/10/26
// ////////////////////////////////////////////////////////

#include "xlibrary.h"

using namespace std;

#define FILE_STRING_SIZE 200

// ////////////////////////////////////////////////////////
// Function Prototypes
// ////////////////////////////////////////////////////////
void hex2bin (string line, unsigned char * instruction);
void read_data_mem();
void write_data_mem();
void read_config(char * filename);
void write_output(char * filename);
// ///////////////////////////////////////////////////////

// ///////////////////////////////////////////////////////
// Global Variables
// ///////////////////////////////////////////////////////
int latency_vals[8];			// Latency values of arithmetic instructions
int clock_cycles[22];			// Number of cycles per instruction
unsigned char inst_memory[MEM_SIZE];	// Instruction Memory
unsigned char data_memory[MEM_SIZE];	// Data Memory
short int reg_file[8];			// Register File
unsigned short int program_counter;	// Program Counter
// ///////////////////////////////////////////////////////

int main (int argc, char *argv[]) {

    char inputfile[FILE_STRING_SIZE];		// Char String for input file
    char configfile[FILE_STRING_SIZE];		// Char string for configuration file
    char outputstatfile[FILE_STRING_SIZE];	// Char string for output file

    int i;					// Count variable

    short int halt_all;				// Halting Flag

    ifstream infile;				// Input File
    string line;				// String for instruction line

    short int instruction;			// 16-Bit value of instruction
    unsigned short int opcode;			// Opcode Value

    // Check for valid execution parameters
    if (argc != 4) {
	cout << "Invalid Usage...\n\t" << argv[0] << " input_file configuration_file output_file" << endl;
	return -1;
    }

    // copy parameters to strings
    strcpy(inputfile, argv[1]);
    strcpy(configfile, argv[2]);
    strcpy(outputstatfile, argv[3]);

#ifdef DEBUG

    cout << "Input: " << inputfile << endl;
    cout << "Config: " << configfile << endl;
    cout << "Output: " << outputstatfile << endl;

#endif

    // Open the input file
    infile.open(inputfile);
    // Make sure file is open and exists
    if (!infile.is_open()) {
	cout << "Input File Does Not Exist...Terminating" << endl;
	return 0;
    }

    // Read configuration file
    read_config(configfile);

    // set data and clock cycles to 0
    memset(data_memory, 0, sizeof(data_memory));
    memset(clock_cycles, 0, sizeof(clock_cycles));

    // Set halt flag to 0
    halt_all = (short int) 0;
    // Set program counter to address 0
    program_counter = 0;

    // Set count variable to 0
    i = 0;

    // Read the input file
    while(getline(infile, line)) {
	// Ignore Comments
	if (line[0] != '#') {
	    // Convert hex to binary representation
	    hex2bin(line.substr(0,2), &inst_memory[i++]);
	    hex2bin(line.substr(2,2), &inst_memory[i]);
	    i++;
	}
    }

    // Close the input file
    infile.close();

#ifdef DEBUG

    cout << "Num Instructions: "<< (i/2) << endl;

#endif

    // Loop until halt flag is set or error occurs
    while ((!halt_all) && (program_counter != (unsigned short int)-1)) {

#ifdef DEBUG
	    cout << "PC: " << program_counter << endl;
#endif

	    // Get instruction from instruction memory
	    instruction = (unsigned short int)(inst_memory[program_counter] << 8) | (unsigned short int)(inst_memory[program_counter + 1]);

	    // Get the opcode of instruction
	    get_opcode(instruction, &opcode);

#if 1
	    cout << hex << instruction << "\t" << dec;
#endif

#if 1
	    // Perform appropriate operation based on opcode		
	    switch (opcode){
		case (0x00):
		    program_counter = x_add(instruction);
		    break;
		case (0x01):
		    program_counter = x_sub(instruction);
		    break;
		case (0x02):
		    program_counter = x_and(instruction);
		    break;
		case (0x03):
		    program_counter = x_nor(instruction);
		    break;
		case (0x04):
		    program_counter = x_div(instruction);
		    break;
		case (0x05):
		    program_counter = x_mul(instruction);
		    break;
		case (0x06):
		    program_counter = x_mod(instruction);
		    break;
		case (0x07):
		    program_counter = x_exp(instruction);
		    break;
		case (0x08):
		    program_counter = x_lw(instruction);
		    break;
		case (0x09):
		    program_counter = x_sw(instruction);
		    break;
		case (0x10):
		    program_counter = x_liz(instruction);
		    break;
		case (0x11):
		    program_counter = x_lis(instruction);
		    break;
		case (0x12):
		    program_counter = x_lui(instruction);
		    break;
		case (0x14):
		    program_counter = x_bp(instruction);
		    break;
		case (0x15):
		    program_counter = x_bn(instruction);
		    break;
		case (0x16):
		    program_counter = x_bx(instruction);
		    break;
		case (0x17):
		    program_counter = x_bz(instruction);
		    break;
		case (0x0C):
		    program_counter = x_jr(instruction);
		    break;
		case (0x13):
		    program_counter = x_jalr(instruction);
		    break;
		case (0x18):
		    program_counter = x_j(instruction);
		    break;
		case (0x0D):
		    halt_all = x_halt(instruction);
		    break;
		case (0x0E):
		    program_counter = x_put(instruction);
		    break;
		default:
		    cout << "Invalid Opcode: " << opcode << endl;
		    program_counter += 2;
		    break;
	    }
#endif

#ifdef DEBUG
	    cout << endl;
#endif
    }

    // Write output stats after program terminates
    write_output(outputstatfile);

#ifdef DEBUG

    write_data_mem();

#endif

    return 0;
}

// ////////////////////////////////////////////////////////////////
// Local Procedures
// ////////////////////////////////////////////////////////////////

void hex2bin (string line, unsigned char * instruction) {
    int i;			// Counting variable
    unsigned short int temp;	// temporary value

    // Initialize values
    *instruction = 0;
    temp = 0;

    i = 0;

    // Loop while line is a character or digit
    while (isalnum(line[i])) {
	// Shift temp
	temp = temp << 4;
	
	// Convert hex to number
	if ((line[i] >= 'A') && (line[i] <= 'F')) {
	    temp=(temp|((line[i]-'A'+10)));
	}
	else if ((line[i] >= 'a') && (line[i] <= 'f')) {
	    temp=(temp|((line[i]-'a'+10)));
	}
	else {
	    temp=(temp|((line[i]-'0')));
	}

	i++;
    }

    // Copy temp
    memcpy(instruction, &temp, sizeof(char));

    return;
}

// /////////////////////////////////////////////////////////////////
// This function is for debugging purposes
// /////////////////////////////////////////////////////////////////
void read_data_mem() {
    ifstream mem_file;
    string line1, line2;
    unsigned char temp1, temp2;
    int i;

    mem_file.open("data_mem.txt");

    if (!mem_file.is_open()) {
	cout << "File Does Not Exist" << endl;
    }

    i = 0;

    while (mem_file >> line1) {
	mem_file >> line2;
	temp1 = 0;
	temp2 = 0;

	hex2bin(line1, &temp1);
	hex2bin(line2, &temp2);

	data_memory[i++] = temp1;
	data_memory[i++] = temp2;
    }

    mem_file.close();

    return;
}

// Convert a binary value back to hex
char bin2hex (short int onebyte) {
    char hex_val;		// hex value

    // Convert
    switch (onebyte) {
	case 0:
	    hex_val = '0';
	    break;
	case 1:
	    hex_val = '1';
	    break;
	case 2:
	    hex_val = '2';
	    break;
	case 3:
	    hex_val = '3';
	    break;
	case 4:
	    hex_val = '4';
	    break;
	case 5:
	    hex_val = '5';
	    break;
	case 6:
	    hex_val = '6';
	    break;
	case 7:
	    hex_val = '7';
	    break;
	case 8:
	    hex_val = '8';
	    break;
	case 9:
	    hex_val = '9';
	    break;
	case 10:
	    hex_val = 'A';
	    break;
	case 11:
	    hex_val = 'B';
	    break;
	case 12:
	    hex_val = 'C';
	    break;
	case 13:
	    hex_val = 'D';
	    break;
	case 14:
	    hex_val = 'E';
	    break;
	case 15:
	    hex_val = 'F';
	    break;
	default:
	    break;
    }

    return hex_val;
}

// ///////////////////////////////////////////////////////////////////////
// This function is for debugging purposes
// ///////////////////////////////////////////////////////////////////////
void write_data_mem() {
    ofstream outfile;
    int i;
    int j;
    short int temp;
    char line[2];

    outfile.open("data_mem.txt", ios::trunc);

    j = 0;

    if (outfile.is_open()) {
	for (i = 0; i < (MEM_SIZE/2); i++) {
	    temp = data_memory[i];
	    outfile << bin2hex(((temp >> 4) & 0x000F)) << bin2hex(((temp) & 0x000F)) << "\n";
	    temp = data_memory[++i];
	    outfile << bin2hex(((temp >> 4) & 0x000F)) << bin2hex(((temp) & 0x000F)) << "\n";
	    outfile.flush();
	}
    }

    outfile.close();
}

// Read the configuration file
void read_config(char * filename) {
    Json::Value root;		// JSON variable
    ifstream test(filename);	// Input file

    // Copy configuration to root
    test >> root;

    // Pull out values
    latency_vals[ADD] = root.get("add", 1).asInt();
    latency_vals[SUB] = root.get("sub", 1).asInt();
    latency_vals[AND] = root.get("and", 1).asInt();
    latency_vals[NOR] = root.get("nor", 1).asInt();
    latency_vals[DIV] = root.get("div", 1).asInt();
    latency_vals[MUL] = root.get("mul", 1).asInt();
    latency_vals[MOD] = root.get("mod", 1).asInt();
    latency_vals[EXP] = root.get("exp", 1).asInt();

#ifdef DEBUG

    cout << "Add: " << latency_vals[ADD] << endl;
    cout << "Sub: " << latency_vals[SUB] << endl;
    cout << "And: " << latency_vals[AND] << endl;
    cout << "NOR: " << latency_vals[NOR] << endl;
    cout << "DIV: " << latency_vals[DIV] << endl;
    cout << "MUL: " << latency_vals[MUL] << endl;
    cout << "Mod: " << latency_vals[MOD] << endl;
    cout << "EXP: " << latency_vals[EXP] << endl;

#endif

    return;
}

// Write the output stats
void write_output (char * filename) {
    ofstream outfile;				// Output file
    Json::Value stat_obj;			// JSON objects
    Json::Value stat_array(Json::arrayValue);
    Json::Value array;
    Json::Value reg_obj;
    Json::Value reg_array(Json::arrayValue);
    Json::StyledWriter styledWriter;

    // Initialize values
    int inst_count = 0;
    int num_cycles = 0;
    int i;

    // Copy values in the register
    reg_obj["r0"] = reg_file[0];
    reg_obj["r1"] = reg_file[1];
    reg_obj["r2"] = reg_file[2];
    reg_obj["r3"] = reg_file[3];
    reg_obj["r4"] = reg_file[4];
    reg_obj["r5"] = reg_file[5];
    reg_obj["r6"] = reg_file[6];
    reg_obj["r7"] = reg_file[7];

    // Append as array
    reg_array.append(reg_obj);

    // Copy instruction stats
    stat_obj["add"] = clock_cycles[N_ADD];
    stat_obj["sub"] = clock_cycles[N_SUB];
    stat_obj["and"] = clock_cycles[N_AND];
    stat_obj["nor"] = clock_cycles[N_NOR];
    stat_obj["div"] = clock_cycles[N_DIV];
    stat_obj["mul"] = clock_cycles[N_MUL];
    stat_obj["mod"] = clock_cycles[N_MOD];
    stat_obj["exp"] = clock_cycles[N_EXP];
    stat_obj["lw"] = clock_cycles[N_LW];
    stat_obj["sw"] = clock_cycles[N_SW];
    stat_obj["liz"] = clock_cycles[N_LIZ];
    stat_obj["lis"] = clock_cycles[N_LIS];
    stat_obj["lui"] = clock_cycles[N_LUI];
    stat_obj["bp"] = clock_cycles[N_BP];
    stat_obj["bn"] = clock_cycles[N_BN];
    stat_obj["bx"] = clock_cycles[N_BX];
    stat_obj["bz"] = clock_cycles[N_BZ];
    stat_obj["jr"] = clock_cycles[N_JR];
    stat_obj["jal"] = clock_cycles[N_JAL];
    stat_obj["j"] = clock_cycles[N_J];
    stat_obj["halt"] = clock_cycles[N_HALT];
    stat_obj["put"] = clock_cycles[N_PUT];

    // Calculate number of cycles and instruction count
    for (i = 0; i < 22; i++) {
	inst_count += clock_cycles[i];
	if (i < 8) {
	    num_cycles += (clock_cycles[i] * latency_vals[i]);
	}
	else {
	    num_cycles += clock_cycles[i];
	}
    }

    // Copy counts
    stat_obj["instructions"] = inst_count;
    stat_obj["cycles"] = num_cycles;

    // Append as array
    stat_array.append(stat_obj);
 
    // Combine as 1 object
    array["registers"] = reg_array;  
    array["stats"] = stat_array;  

#ifdef DEBUG

    cout << endl << endl << array << endl;

#endif

    // Open output
    outfile.open(filename);

    // Write stats
    outfile << styledWriter.write(array);

    // Close
    outfile.close();
}
