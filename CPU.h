#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include<bits/stdc++.h>
#include<cmath>
#include <ctime>
using namespace std;


class instruction {
public:
	bitset<32> instr;//instruction
	instruction(bitset<32> fetch); // constructor

};

class CPU {
private:
	int dmemory[4096]; //data memory byte addressable in little endian fashion;
//	unsigned long PC; //pc/
	int current_arithmetic;
	int next_arithmetic;
	int a,b,c;
	int x[31];


   //ALU();
public:

	unsigned long PC; //pc/
	CPU();
	unsigned long readPC();
	bitset<32> Fetch(bitset<8> *instmem);
	bool Decode(instruction* instr);
	int Execution();
	int Memory();
	int Writeback();
	int Outputa0();
	int Outputa1();
	int total_number_of_cycle_including_zero=0;
    int r_type_instructuin_count=0;
    int efficient_instruction_count=0;
    int jump_instruction_count=0;
    float IPC=0;

};

class transfer_function
{
private:
string findTwoscomplement(string str);
public:

 int   transfer_register (string);
 int   transfer_immediate(string);
};


