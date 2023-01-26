#include "CPU.h"





int alu_out;

int memory[31];

//itype except lw
int temporary_number_x_rs1;
int temporary_number_x_rs2;
int temporary_number_x_rd;
int temporary_number_immediate;
//sw
int offset;


bool ADDI=false;
bool ADD=false;
bool XOR=false;
bool SUB=false;
bool ANDI=false;
bool SRA=false;
bool LW=false;
bool SW=false;
bool BLT=false;
bool JALR=false;




bool writeback_on=false;
bool writeback_lw = false;
bool memory_write=false;
bool memory_read = false;

instruction::instruction(bitset<32> fetch)
{
	//cout << fetch << endl;
	instr = fetch;
	//cout << instr << endl;
}

CPU::CPU()
{
	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		dmemory[i] = (0);
	}


}

bitset<32> CPU::Fetch(bitset<8> *instmem) {
	bitset<32> instr = ((((instmem[PC + 3].to_ulong()) << 24)) + ((instmem[PC + 2].to_ulong()) << 16) + ((instmem[PC + 1].to_ulong()) << 8) + (instmem[PC + 0].to_ulong()));  //get 32 bit instruction
	PC += 4;//increment PC
	total_number_of_cycle_including_zero=PC/4-jump_instruction_count;
	efficient_instruction_count = total_number_of_cycle_including_zero-1;//Delete "zero instruction
	IPC=1.0*efficient_instruction_count/total_number_of_cycle_including_zero;
	return instr;
}


bool CPU::Decode(instruction* curr)
{
bitset<32> initial_insturction;
initial_insturction = curr->instr;
string temporary1;


string opcode;
string func3;
string func7;
string rs1;
string rs2;
string rd;

temporary1 = initial_insturction.to_string();
opcode = temporary1.substr(25 , 7);
rd     = temporary1.substr(20 , 5);
func3  = temporary1.substr(17 , 3);
rs1    = temporary1.substr(12 , 5);
rs2    = temporary1.substr(7  , 5);
func7  = temporary1.substr(0  , 7);
string immediate = func7 + rs2;
string sw_immediate = func7+rd;
string branch_immediate = temporary1.substr(0  , 1)+temporary1.substr(24,1)+temporary1.substr(2  , 6)+temporary1.substr(20  , 4)+"0";

//cout<<temporary1<<endl;

string instructiontype_R = "0110011";
string insturctiontype_I = "0010011";
string insturctiontype_S = "0100011";
string instruction_Load =  "0000011";
string instructiontype_B = "1100011";
string instruction_jalr =  "1100111";
string ending = "00000000000000000000000000000000";

transfer_function transfer_stringtovirable;

if(temporary1==ending)
{

return false;
}



if(opcode == instructiontype_R ){
        if(func3 == "000" && func7=="0000000")
        {
            temporary_number_x_rd= transfer_stringtovirable.transfer_register(rd);
            temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
            temporary_number_x_rs2 = transfer_stringtovirable.transfer_register(rs2);
            ADD = true;
            r_type_instructuin_count+=1;

        }
        else if(func3 == "000" && func7 == "0100000")
        {
            temporary_number_x_rd= transfer_stringtovirable.transfer_register(rd);
            temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
            temporary_number_x_rs2 = transfer_stringtovirable.transfer_register(rs2);
            SUB = true;
            r_type_instructuin_count+=1;
        }
        else if(func3 == "100" )
        {
            temporary_number_x_rd= transfer_stringtovirable.transfer_register(rd);
            temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
            temporary_number_x_rs2 = transfer_stringtovirable.transfer_register(rs2);
            XOR = true;
            r_type_instructuin_count+=1;
        }
        else if(func3 == "101" && func7 == "0100000")
        {
            temporary_number_x_rd= transfer_stringtovirable.transfer_register(rd);
            temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
            temporary_number_x_rs2 = transfer_stringtovirable.transfer_register(rs2);
            SRA = true;
            r_type_instructuin_count+=1;
        }



}
else if(opcode == insturctiontype_I){
  // cout<<immediate<<endl;
   if(func3 == "000"){
   temporary_number_x_rd= transfer_stringtovirable.transfer_register(rd);
   temporary_number_immediate = transfer_stringtovirable.transfer_immediate(immediate);
   temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
   ADDI = true;
   }
   else if(func3 == "111")
   {
   temporary_number_x_rd= transfer_stringtovirable.transfer_register(rd);
   temporary_number_immediate = transfer_stringtovirable.transfer_immediate(immediate);
   temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
   ANDI = true;
   }

}

else if(opcode == insturctiontype_S)
{


        offset= transfer_stringtovirable.transfer_immediate(sw_immediate);
        temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
        temporary_number_x_rs2 = transfer_stringtovirable.transfer_register(rs2);
        SW = true;

}

else if(opcode == instruction_Load)
{
    if(func3 == "010")
    {
       temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
       temporary_number_x_rd= transfer_stringtovirable.transfer_register(rd);
       temporary_number_immediate = transfer_stringtovirable.transfer_immediate(immediate);
       LW = true;
    }
}
else if(opcode == instructiontype_B)
{
    if(func3 == "100")
    {
        temporary_number_immediate = transfer_stringtovirable.transfer_immediate(branch_immediate);
        temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
        temporary_number_x_rs2 = transfer_stringtovirable.transfer_register(rs2);
        BLT = true;

    }
}
else if(opcode == instruction_jalr)
{
    if(func3 == "000")
    {
   temporary_number_x_rd= transfer_stringtovirable.transfer_register(rd);
   temporary_number_immediate = transfer_stringtovirable.transfer_immediate(immediate);
   temporary_number_x_rs1 = transfer_stringtovirable.transfer_register(rs1);
   JALR = true;

    }
}
//cout<<opcode<<endl;//find the opcode of every instruction


return true;
}





unsigned long CPU::readPC()
{

	return PC;
}





int transfer_function::transfer_register(string x)
{
    bitset<5>bitset1(x);
    int y = bitset1.to_ulong();
    return y;

}

string transfer_function::findTwoscomplement(string str)
{
	int n = str.length();
	int i;
	for (i = n-1 ; i >= 0 ; i--)
		if (str[i] == '1')
			break;
	if (i == -1)
		return '1' + str;

	for (int k = i-1 ; k >= 0; k--)
	{
		if (str[k] == '1')
			str[k] = '0';
		else
			str[k] = '1';
	}
	return str;
}







int transfer_function::transfer_immediate(string str)
{
     string c;
    c=str.substr(0,1);
    if(c=="1"){
	string k = findTwoscomplement(str);
	bitset<14>bitset1(k);
	int y = -bitset1.to_ulong();
   return y;
    }
    else{
        bitset<12>bitset1(str);
      int j =  bitset1.to_ulong();
return j;

}
}



int CPU::Execution()
{

if(ADDI==true)
{
    alu_out = temporary_number_immediate+temporary_number_x_rs1;
    writeback_on = true;
    memory_write = false;
    ADDI = false;
    return x[temporary_number_x_rs2];
}
else if(SW == true)
{
    alu_out = offset + temporary_number_x_rs1;
 //   cout<<alu_out<<endl;
    memory_write = true;
    SW = false;
}
else if(ADD == true)
{
    alu_out = x[temporary_number_x_rs1]+ x[temporary_number_x_rs2];
    ADD = false;
    memory_write = false;
    writeback_on = true;
}
else if(LW == true)
{
    alu_out = temporary_number_immediate+temporary_number_x_rs1;
    LW = false;
    memory_write = false;
    memory_read = true;
}
else if(SUB == true)
{
    alu_out = x[temporary_number_x_rs1]-x[temporary_number_x_rs2];
    SUB = false;
    writeback_on = true;
    memory_write = false;
    memory_read = false;
}
else if(XOR == true)
{
    alu_out = x[temporary_number_x_rs1]^x[temporary_number_x_rs2];
    XOR = false;
    writeback_on = true;
    memory_write = false;
    memory_read = false;
}
else if(SRA== true)
{
    alu_out = x[temporary_number_x_rs1]/pow(2,x[temporary_number_x_rs2]);
    SRA = false;
    writeback_on = true;
    memory_write = false;
    memory_read = false;
}
else if(ANDI== true)
{
    alu_out = temporary_number_immediate & x[temporary_number_x_rs1];
    ANDI = false;
    writeback_on = true;
    memory_write = false;
    memory_read = false;
}
else if(BLT== true)
{
    alu_out = x[temporary_number_x_rs1]-x[temporary_number_x_rs2];
    if(alu_out < 0 )
    {
    BLT = false;
    writeback_on = false;
    memory_write = false;
    memory_read = false;
    PC+=temporary_number_immediate;
    jump_instruction_count +=(temporary_number_immediate-4)/4;
  //  cout<<PC<<endl;
    }
    else{return 0;}

}
else if(JALR== true)
{

    x[temporary_number_x_rd]=PC;
 //      cout<<"PC = "<<PC<<"    jumps to +"<<temporary_number_immediate<<endl;
    PC = x[temporary_number_x_rs1]+temporary_number_immediate;
    jump_instruction_count+= (temporary_number_immediate-4)/4;
   // cout<<PC<<endl;
    JALR = false;
    writeback_on = false;
    memory_write = false;
    memory_read = false;
}
return 0;

}



int CPU::Memory()
{
if(memory_write==true)
{
  //  return 0;
    memory[alu_out] = x[temporary_number_x_rs2];
 //  cout<<"PC = "<<PC<<"    "<<"memory["<<alu_out<<"] ="<<memory[alu_out]<<endl;
    memory_write = false;
}
else if(memory_read == true)
{
//cout<<"PC = "<<PC<<"   "<<"read data memory"<<alu_out<<" = "<<memory[alu_out]<<endl;
memory_read = false;
writeback_lw = true;
}
return 0;
}


int CPU::Writeback()
{
if(writeback_on == true){
    x[temporary_number_x_rd]=alu_out;
 //   cout<<"PC = "<<PC<<"     "<< "x["<<temporary_number_x_rd<< "] = "<<x[temporary_number_x_rd]<<endl;
 //   cout<<PC<<endl;
    writeback_on = false;

}
else if(writeback_lw == true)
{

    x[temporary_number_x_rd]=memory[alu_out];
//cout<<"PC = "<<PC<<"    "<< "x["<<temporary_number_x_rd<< "] = "<<x[temporary_number_x_rd]<<endl;
writeback_lw == false;

}
return 0;
}

int CPU::Outputa0()
{
    return x[10];
}

int CPU::Outputa1()
{
    return x[11];

}
