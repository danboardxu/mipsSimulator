 /* On my honor, I have neither given nor received unauthorized aid on this assignment */

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

#define AUTHOR "Vikrant Sagar"
#define COURSE "Computer Architecture"
#define PROJECT "Project 1"

//class to hold 32 bit data structure and functions operated on it
class instr32{
	public:
		bool bits[32];	//32 bit bool array to hold 32 bit number

		instr32(){
			for(int i=0;i<32;i++)
				bits[i] = 0;
		}

		//constructor to convert 32 char binary string to 32 bit bool array
		instr32(string str){
			int i = 0;
			for ( std::string::iterator it=str.begin(); it!=str.end(); ++it){
				if(*it == '1')
					bits[i] = 1;
				else
					bits[i] = 0;

				i++;
			}
		}
		
		//function to print the bool array as on the screen
		void print(){
			for(int i=0;i< 32;i++){
				cout << bits[i];
			}
			//cout << endl;
		}
		
		//operator overloading to assign 32 char string to 32 bit bool array 
		void operator=(string str){
			int i = 0;
			for ( std::string::iterator it=str.begin(); it!=str.end(); ++it){
				if(*it == '1')
					bits[i] = 1;
				else
					bits[i] = 0;

				i++;
			}

		}

		//function to convert instr32 data structure to long num
		instr32 num_2_twos_complement(long num){
			instr32 temp;
			if(num < 0){
				temp = num;
				temp = ~temp;
				temp = temp + instr32("00000000000000000000000000000001");
				return temp;

			}else{
				temp = num;
				return temp;
			}
		}
		
		//operator overloading to assign number to 32 bit bool array
		void operator=(long num){

			instr32 ins;

			copy(ins);
			int i = 31;

			for(int i=31;i>=0;i--){
				if(num){
					if(num % 2 == 1)
						bits[i] = 1;
					else
						bits[i] = 0;

					num = num / 2;
					//cout << "num = " << num << endl;
					
				}
			}
			/*
			for(int i=0;i< 32;i++){
				cout << bits[i] ;
			}
			cout << endl;
			*/

		}

		//function to copy instr32 data structure to current object
		void copy(instr32 ins){
			for(int i=0;i< 32;i++){
				bits[i] = ins.bits[i];
			}
		}

		//operator overloading to do logical AND on the two instr32 type objects
		instr32 operator&(instr32 right){
			instr32 ins;
			for(int i=0;i< 32;i++){
				ins.bits[i] = bits[i] & right.bits[i];
			}

			return ins;
		}

		//operator overloading to do logical XOR on the two instr32 type objects
		instr32 operator^(instr32 right){
			instr32 ins;
			for(int i=0;i< 32;i++){
				ins.bits[i] = bits[i] ^ right.bits[i];
			}

			return ins;
		}

		//operator overloading to do logical OR on the two instr32 type objects
		instr32 operator|(instr32 right){
			instr32 ins;
			for(int i=0;i< 32;i++){
				ins.bits[i] = bits[i] | right.bits[i];
			}

			return ins;
		}

		//operator overloading to do logical RIGHT SHIFT on instr32 type objects
		instr32 operator>>(int shift){
			instr32 ins;

			if(shift > 31)
				return ins;
			if(shift < 1){
				for(int i=0;i < 32;i++){
					ins.bits[i] = bits[i];
				}

				return ins;
			}

			for(int i=31-shift;i >=0 ;i--){
				ins.bits[i+shift] = bits[i];
			}

			return ins;
		}
		
		//operator overloading to do logical LEFT SHIFT on instr32 type objects
		instr32 operator<<(int shift){
			instr32 ins;

			if(shift > 31)
				return ins;
			if(shift < 1){
				for(int i=0;i < 32;i++){
					ins.bits[i] = bits[i];
				}

				return ins;
			}

			for(int i=shift;i < 32 ;i++){
				ins.bits[i-shift] = bits[i];
			}

			return ins;
		}
		
		//operator overloading to do arthmatic RIGHT SHIFT on instr32 type objects
		instr32 SRA(instr32 ins,long shift){
			bool flag = 0;
			instr32 temp;
			if(bits[0] == 1)
				flag = 1;
			temp = ins >> shift;
			if(flag)
				for(int i=0;i < shift ; i++)
					temp.bits[i] = 1;

			return temp; 
		}

		//function to convert current object to long number
		long toNum(){
			long num = 0;
			long mul = 1;
			for(int i=31;i>=0;i--){
				if(bits[i] == 1){
					num += mul;
				}

				mul *= 2;
			}

			return num;

		}

		//operator overloading to compare two instr32 object
		bool operator==(instr32 right){
			for(int i=0;i< 32;i++){
				if(bits[i] != right.bits[i])
					return 0;
			}

			return 1;
		}

		instr32 operator~(){
			instr32 ins;
			for(int i=0;i< 32;i++){
				ins.bits[i] = !bits[i];
			}
			return ins;
		}

		//function to add to bit
		bool add(bool a,bool b){

			if(a == b)
				return 0;
			else
				return 1;

		}

		//operator overloading to do bitwuse ADD operation on two instr32 type objects
		instr32 operator+(instr32 right){
			bool carry = 0,carry1,carry2;
			bool ans;
			instr32 ins;
			for(int i=31;i>=0;i--){
				//cout <<"initial carry = " << carry << endl;
				//cout << bits[i] << " + " << right.bits[i] << endl;
				ans = add(carry,right.bits[i]);
				carry1 = carry & right.bits[i];
				//cout << "ans = " << ans << "\tcarry = " << carry;
				ins.bits[i] = add(bits[i],ans);
				carry2 = bits[i] & ans;
				carry = carry1 | carry2 ;	
				//cout << "ins.bits[" << i <<"] = " << ins.bits[i] << endl;

			}
			return ins;

		}
};

//class to hold opcodes 
class OPCODES{
public:
		instr32 J,JR,BEQ,BLTZ,BGTZ,BREAK,SW,LW,SLL,SRL,SRA,NOP,ADD,SUB,MUL,AND,OR,XOR,NOR,SLT,ADDI,ANDI,ORI,XORI;

		OPCODES(){
			//category 1
			J 		=	"01000000000000000000000000000000";
			JR 		=	"01000100000000000000000000000000";
			BEQ		=	"01001000000000000000000000000000";
			BLTZ 	=	"01001100000000000000000000000000";
			BGTZ 	=	"01010000000000000000000000000000";
			BREAK 	=	"01010100000000000000000000000000";
			SW 		=	"01011000000000000000000000000000";
			LW 		=	"01011100000000000000000000000000";
			SLL 	=	"01100000000000000000000000000000";
			SRL 	=	"01100100000000000000000000000000";
			SRA		=	"01101000000000000000000000000000";
			NOP 	=	"01101100000000000000000000000000";

			//category 2
			ADD		=	"11000000000000000000000000000000";
			SUB		=	"11000100000000000000000000000000";
			MUL		=	"11001000000000000000000000000000";
			AND 	=	"11001100000000000000000000000000";
			OR	 	=	"11010000000000000000000000000000";
			XOR 	=	"11010100000000000000000000000000";
			NOR		=	"11011000000000000000000000000000";
			SLT		=	"11011100000000000000000000000000";
			ADDI 	=	"11100000000000000000000000000000";
			ANDI	=	"11100100000000000000000000000000";
			ORI		=	"11101000000000000000000000000000";
			XORI	=	"11101100000000000000000000000000";
		}
};


//class to hold each instruction data 
class instr_decoded{

public:
	instr32 ins;
	long rs,rd,rt,sa,base;
	long signed_offset;
	int PC;

	instr_decoded(){
		rs = rd = rt = sa = signed_offset = base = -1;
		PC = -1;
	}

};



//class to disassemble the input file
class Dissassembler{

	public:
		instr32 R[32];	//32 registers
		vector<instr32> memory;	//total memory
		int data_offset;
		int instr_offset;
		int code_size;
		OPCODES opCodes;
		ofstream out,out2;
		vector<instr_decoded*> instr;
		map<int,string> m2c;
		string FILE_NAME;

		vector<string> instr_str;

		//scorboarding specific
		instr_decoded* fetch_queue[2];
		instr_decoded* pre_issue[4];
		instr_decoded* pre_ALU1[2];
		instr_decoded* pre_ALU2[2];
		instr_decoded* post_ALU2[2];
		instr_decoded* pre_MEM[2];
		instr_decoded* post_MEM[2];

		//counters
		map<int,int> reg_table;
		//temps
		map<int,int> reg_table1;
		instr32 R1[32];

		int pre_issue_queue_entries;
		int pre_ALU1_queue_entries;
		int pre_ALU2_queue_entries;

		int PC;

		int CYCLE;


	Dissassembler(){
		data_offset = 0;
		code_size = 0;
		instr_offset = 256;
		for(int i;i<32;i++){
			R[i] = 0;
		}

		out.open("generated_disassembly.txt");
		out2.open("generated_simulation.txt");

		pre_issue_queue_entries = 0;
		pre_ALU1_queue_entries = 0;
		pre_ALU2_queue_entries = 0;

		//init buffers
		fetch_queue[0] = NULL;
		fetch_queue[1] = NULL;

		pre_issue[0] = NULL;
		pre_issue[1] = NULL;
		pre_issue[2] = NULL;
		pre_issue[3] = NULL;

		pre_ALU1[0] = NULL;
		pre_ALU1[1] = NULL;

		pre_ALU2[0] = NULL;
		pre_ALU2[1] = NULL;

		pre_MEM[0] = NULL;
		post_MEM[0] = NULL;

		post_ALU2[0] = NULL;

		PC = 0;

		for(int i=0;i<32;i++){
			reg_table[i] = 0;
		}

	}

	void print_instr32(instr32 mem){
			for(int i=0;i< 32;i++){
				out << mem.bits[i];
			}
			//cout << endl;
		}
	//read data from file and store it in memory
	void file2memory(char* file){
		FILE_NAME = file;
		string line;
  		ifstream myfile;
  		myfile.open(file);
  		int i = 0;
    	while ( myfile.good() ){

      		getline (myfile,line);
      		if(!(line.length() > 31))
      			break;
      		memory.push_back(line);
    	}
    	myfile.close();
  		code_size = i;
	}

	//find where data segment starts
	void getDataOffset(){
		instr32 mask;
		mask = "11111100000000000000000000000000";
		int i = 0;
		for(i=0;i<memory.size();i++){
			if((memory[i] & mask) == opCodes.BREAK)
				break;

		}
		data_offset = (i+1)*4 + 252 + 4;
	}

	//to extract rs value fron instr32 object
	long rs(instr32 ins){
		instr32 temp;
		temp.copy(ins);
		instr32 maskrs("00000011111000000000000000000000");

		temp = temp & maskrs;
		temp = temp >> 21;
		return temp.toNum(); 
	}
	
	//to extract rt value fron instr32 object
	long rt(instr32 ins){
		instr32 temp;
		temp.copy(ins);
		instr32 maskrt("00000000000111110000000000000000");

		temp = temp & maskrt;
		temp = temp >> 16;
		return temp.toNum(); 
	}

	//to extract rd value fron instr32 object
	long rd(instr32 ins){
		instr32 temp;
		temp.copy(ins);
		instr32 maskrt("00000000000000001111100000000000");

		temp = temp & maskrt;
		temp = temp >> 11;
		return temp.toNum(); 
	}

	//to extract sa value fron instr32 object
	long sa(instr32 ins){
		instr32 temp;
		temp.copy(ins);
		instr32 maskrt("00000000000000000000011111000000");

		temp = temp & maskrt;
		temp = temp >> 6;
		return temp.toNum(); 
	}

	//function to disaasemble the machine code to assembly codes
	void disassemble_instructions(){
		getDataOffset();
		int END_OF_INSTR = (data_offset-256)/4;
		instr32 mask;
		instr32 op, temp, tempPC;
		
		long PC = 256;

		mask = "11111100000000000000000000000000";

		//cout << "Data Offset = " << data_offset << endl;
		//cout << "END_OF_INSTR = " << END_OF_INSTR <<  endl;
		
		
		//loop to iterate over all instructions
		for(int i=0;i< END_OF_INSTR;i++){
			PC = 256 + i*4;
			op = memory[i] & mask;
			stringstream buffout;

			if(op == opCodes.J){
				instr32 mask4("11110000000000000000000000000000");
				instr32 mask26("00000011111111111111111111111111");

				temp = memory[i] & mask26;
				temp = temp << 2;
				//cout << (long)PC + 4 << endl;;
				tempPC = (long)PC + 4;
				tempPC = tempPC & mask4;
				temp = temp | tempPC;
				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "J #" << temp.toNum() << endl;

				buffout << "J #" << temp.toNum();
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.JR){
				instr32 maskrs("00000011111000000000000000000000");

				temp = memory[i] & maskrs;
				temp = temp >> 21;
				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "JR R" << temp.toNum() << endl;

				buffout << "JR R" << temp.toNum();
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.BEQ){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val,rt_val;
				rs_val = rs(memory[i]);
				rt_val = rt(memory[i]);

				temp = memory[i] & mask16;
				temp = temp << 2;
				//out << (long)PC + 4 << endl;;
				tempPC = (long)PC + 4;
				if(temp.bits[16] == 1){
					temp = ~ temp;
					instr32 mask18("00000000000000111111111111111111");
					temp = temp & mask18;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}
				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "BEQ R" << rs_val <<", R" << rt_val <<", #" << offset << endl;

				buffout << "BEQ R" << rs_val <<", R" << rt_val <<", #" << offset;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.BLTZ){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val;
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				temp = temp << 2;
				//out << (long)PC + 4 << endl;;
				tempPC = (long)PC + 4;
				if(temp.bits[16] == 1){
					temp = ~ temp;
					instr32 mask18("00000000000000111111111111111111");
					temp = temp & mask18;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}
				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "BLTZ R" << rs_val <<", #" << offset << endl;

				buffout << "BLTZ R" << rs_val <<", #" << offset;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.BGTZ){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val;
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				temp = temp << 2;
				//out << (long)PC + 4 << endl;;
				tempPC = (long)PC + 4;
				if(temp.bits[16] == 1){
					temp = ~ temp;
					instr32 mask18("00000000000000111111111111111111");
					temp = temp & mask18;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}
				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "BGTZ R" << rs_val <<", #" << offset << endl;

				buffout << "BGTZ R" << rs_val <<", #" << offset;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.BREAK){
				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "BREAK" << endl;

				buffout << "BREAK";
				instr_str.push_back(buffout.str());
				break;

			}else if(op == opCodes.SW){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val,rt_val;
				rs_val = rs(memory[i]);
				rt_val = rt(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "SW R" << rt_val <<", " << offset << "(R" << rs_val << ")" << endl;

				buffout << "SW R" << rt_val <<", " << offset << "(R" << rs_val << ")";
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.LW){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val,rt_val;
				rs_val = rs(memory[i]);
				rt_val = rt(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "LW R" << rt_val <<", " << offset << "(R" << rs_val << ")" << endl;

				buffout << "LW R" << rt_val <<", " << offset << "(R" << rs_val << ")";
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.SLL){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rd_val,rt_val,sa_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				sa_val = sa(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "SLL R" << rd_val <<", R" << rt_val << ", #" << sa_val << endl;

				buffout << "SLL R" << rd_val <<", R" << rt_val << ", #" << sa_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.SRL){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rd_val,rt_val,sa_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				sa_val = sa(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "SRL R" << rd_val <<", R" << rt_val << ", #" << sa_val << endl;

				buffout << "SRL R" << rd_val <<", R" << rt_val << ", #" << sa_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.SRA){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rd_val,rt_val,sa_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				sa_val = sa(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "SRA R" << rd_val <<", R" << rt_val << ", #" << sa_val << endl;

				buffout << "SRA R" << rd_val <<", R" << rt_val << ", #" << sa_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.NOP){
				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "NOP" << endl;

				buffout << "NOP";
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.ADD){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "ADD R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

				buffout << "ADD R" << rd_val <<", R" << rs_val << ", R" << rt_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.SUB){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "SUB R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

				buffout << "SUB R" << rd_val <<", R" << rs_val << ", R" << rt_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.MUL){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "MUL R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

				buffout << "MUL R" << rd_val <<", R" << rs_val << ", R" << rt_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.AND){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "AND R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

				buffout << "AND R" << rd_val <<", R" << rs_val << ", R" << rt_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.OR){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "OR R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

				buffout << "OR R" << rd_val <<", R" << rs_val << ", R" << rt_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.XOR){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "XOR R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

				buffout << "XOR R" << rd_val <<", R" << rs_val << ", R" << rt_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.NOR){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "NOR R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

				buffout << "NOR R" << rd_val <<", R" << rs_val << ", R" << rt_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.SLT){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "SLT R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

				buffout << "SLT R" << rd_val <<", R" << rs_val << ", R" << rt_val;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.ADDI){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rt_val,rs_val;
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "ADDI R" << rt_val <<", R" << rs_val << ", #" << offset << endl;

				buffout << "ADDI R" << rt_val <<", R" << rs_val << ", #" << offset;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.ANDI){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rt_val,rs_val;
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "ANDI R" << rt_val <<", R" << rs_val << ", #" << offset << endl;

				buffout << "ANDI R" << rt_val <<", R" << rs_val << ", #" << offset;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.ORI){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rt_val,rs_val;
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "ORI R" << rt_val <<", R" << rs_val << ", #" << offset << endl;

				buffout << "ORI R" << rt_val <<", R" << rs_val << ", #" << offset;
				instr_str.push_back(buffout.str());

			}else if(op == opCodes.XORI){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rt_val,rs_val;
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				print_instr32(memory[i]);
				out << "\t" << PC << "\t" ;
				out << "XORI R" << rt_val <<", R" << rs_val << ", #" << offset << endl;

				buffout << "XORI R" << rt_val <<", R" << rs_val << ", #" << offset;
				instr_str.push_back(buffout.str());

			}
		}
		
	}
	
	//disassemble data segment to numbers
	void disassemble_data(){
		long num, PC;
		instr32 temp;
		int END_OF_INSTR = (data_offset-256)/4;

		//cout << code_size << endl;
		//cout << data_offset << endl;

		for(int i = END_OF_INSTR;i<code_size;i++){
			PC = 256 + i*4;
			temp.copy(memory[i]);

			if(temp.bits[0] == 1){
				temp = ~ temp;
				temp = temp + instr32("00000000000000000000000000000001");
				num = -temp.toNum();
			}else{
				num = temp.toNum();
			}

			print_instr32(memory[i]);
			out << "\t" << PC << "\t" ;
			out << num << endl;
		}

		out.close();
	}

	//function to create mask from instr32 object
	instr32 make_mask(int start,int end,instr32 ins){
		instr32 temp;
		for(int i=start;i<=end;i++)
			temp.bits[i] = 1;
		return temp;
	}
	
	//function to convert 2' instr32 object to long num
	long twos_complement_2_num(instr32 ins){
		instr32 temp;
		temp.copy(ins);

		if(ins.bits[0] == 1){
			temp = ~temp;
			temp = temp + instr32("00000000000000000000000000000001");
			return -temp.toNum();
		}else{
			return ins.toNum();
		}
	}

	//function to convert long num to 2' complement
	instr32 num_2_twos_complement(long num){
		instr32 temp;
		if(num < 0){
			num = 0 - num;
			//cout << num << endl;
			temp = num;
			//temp.print();
			//cout << endl;
			temp = num;
			temp = ~temp;
			temp = temp + instr32("00000000000000000000000000000001");
			return temp;

		}else{
			temp = num;
			return temp;
		}
	}

	void print_eight_data(int index){

		for(int i=index;i<index+8;i++){
			out2 << "\t" << twos_complement_2_num(memory[i]);
		}

	}
	
	//function to print data segment
	void print_data(){
		int END_OF_INSTR = (data_offset-256)/4;
		int count = 1;
		int offset = data_offset;
		out2 << "Data" << endl;

		for(int i=END_OF_INSTR;i<code_size;i++){
			out2 << offset << ":";
			print_eight_data(i);
			i = i + 7;
			offset =  (i+1)*4 + 252 + 4;
			out2 << endl;

		}

	}

	void print_eight_regs(int index){

		for(int i=index;i<index+8;i++){
			out2 << "\t" << twos_complement_2_num(R[i]);
		}

	}

	
	string pad_digit(int num){
		if(num > 9)
			return "";
		else
			return "0";
	}

	//function to print all registers values
	void print_regs(){
		out2 << "Registers" << endl;

		for(int i=0;i<32;i++){
			out2 << "R" << pad_digit(i) << i << ":";
			print_eight_regs(i);
			i = i + 7;
			out2 << endl;

		}
	}



	//function to get all instructions into instr_decoder object
	void decoder(){

		getDataOffset();
		int END_OF_INSTR = (data_offset-256)/4;
		instr32 mask;
		instr32 op, temp, tempPC;
		
		long PC = 256;

		mask = "11111100000000000000000000000000";

		//cout << "Data Offset = " << data_offset << endl;
		//cout << "END_OF_INSTR = " << END_OF_INSTR <<  endl;
		instr_decoded *insd;
		

		for(int i=0;i< END_OF_INSTR;i++){
			insd = new instr_decoded;
			PC = 256 + i*4;
			op = memory[i] & mask;

			if(op == opCodes.J){
				 
				instr32 mask4("11110000000000000000000000000000");
				instr32 mask26("00000011111111111111111111111111");

				temp = memory[i] & mask26;
				temp = temp << 2;
				//cout << (long)PC + 4 << endl;;
				tempPC = (long)PC + 4;
				tempPC = tempPC & mask4;
				temp = temp | tempPC;
				
				insd->ins.copy(opCodes.J);
				insd->signed_offset = temp.toNum();

				insd->PC = PC;
				instr.push_back(insd);


			}else if(op == opCodes.JR){
				
				instr32 maskrs("00000011111000000000000000000000");

				temp = memory[i] & maskrs;
				temp = temp >> 21;
				
				insd->ins.copy(opCodes.JR);
				insd->rs = temp.toNum();

				insd->PC = PC;
				instr.push_back(insd);
				

			}else if(op == opCodes.BEQ){
				
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val,rt_val;
				rs_val = rs(memory[i]);
				rt_val = rt(memory[i]);

				temp = memory[i] & mask16;
				temp = temp << 2;
				//cout << (long)PC + 4 << endl;;
				tempPC = (long)PC + 4;
				if(temp.bits[16] == 1){
					temp = ~ temp;
					instr32 mask18("00000000000000111111111111111111");
					temp = temp & mask18;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				insd->ins.copy(opCodes.BEQ);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->signed_offset = offset;

				insd->PC = PC;
				instr.push_back(insd);
				

			}else if(op == opCodes.BLTZ){
				
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val;
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				temp = temp << 2;
				//cout << (long)PC + 4 << endl;;
				tempPC = (long)PC + 4;
				if(temp.bits[16] == 1){
					temp = ~ temp;
					instr32 mask18("00000000000000111111111111111111");
					temp = temp & mask18;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				insd->ins.copy(opCodes.BLTZ);
				insd->rs = rs_val;
				insd->signed_offset = offset;

				insd->PC = PC;
				instr.push_back(insd);
				

			}else if(op == opCodes.BGTZ){
				
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val;
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				temp = temp << 2;
				//cout << (long)PC + 4 << endl;;
				tempPC = (long)PC + 4;
				if(temp.bits[16] == 1){
					temp = ~ temp;
					instr32 mask18("00000000000000111111111111111111");
					temp = temp & mask18;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				insd->ins.copy(opCodes.BGTZ);
				insd->rs = rs_val;
				insd->signed_offset = offset;

				insd->PC = PC;
				instr.push_back(insd);
				

			}else if(op == opCodes.BREAK){
				
				insd->ins.copy(opCodes.BREAK);

				insd->PC = PC;
				instr.push_back(insd);
				
				break;

			}else if(op == opCodes.SW){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val,rt_val;
				rs_val = rs(memory[i]);
				rt_val = rt(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				insd->ins.copy(opCodes.SW);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->signed_offset = offset;

				insd->PC = PC;
				instr.push_back(insd);


			}else if(op == opCodes.LW){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rs_val,rt_val;
				rs_val = rs(memory[i]);
				rt_val = rt(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				insd->ins.copy(opCodes.LW);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->signed_offset = offset;

				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.SLL){
				instr32 mask16("00000000000000001111111111111111");
		
				long rd_val,rt_val,sa_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				sa_val = sa(memory[i]);

				insd->ins.copy(opCodes.SLL);
				insd->rd = rd_val;
				insd->rt = rt_val;
				insd->sa = sa_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.SRL){
				instr32 mask16("00000000000000001111111111111111");
				
				long rd_val,rt_val,sa_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				sa_val = sa(memory[i]);

				insd->ins.copy(opCodes.SRL);
				insd->rd = rd_val;
				insd->rt = rt_val;
				insd->sa = sa_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.SRA){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rd_val,rt_val,sa_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				sa_val = sa(memory[i]);

				insd->ins.copy(opCodes.SRA);
				insd->rd = rd_val;
				insd->rt = rt_val;
				insd->sa = sa_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.NOP){
				//memory[i].print();
				//cout << "\t" << PC << "\t" ;
				//cout << "NOP" << endl;

				insd->ins.copy(opCodes.NOP);
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.ADD){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				insd->ins.copy(opCodes.ADD);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->rd = rd_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.SUB){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				insd->ins.copy(opCodes.SUB);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->rd = rd_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.MUL){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				insd->ins.copy(opCodes.MUL);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->rd = rd_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.AND){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				insd->ins.copy(opCodes.AND);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->rd = rd_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.OR){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				insd->ins.copy(opCodes.OR);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->rd = rd_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.XOR){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				insd->ins.copy(opCodes.XOR);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->rd = rd_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.NOR){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				insd->ins.copy(opCodes.NOR);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->rd = rd_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.SLT){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				insd->ins.copy(opCodes.SLT);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->rd = rd_val;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.ADDI){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rt_val,rs_val;
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				insd->ins.copy(opCodes.ADDI);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->signed_offset = offset;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.ANDI){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rt_val,rs_val;
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				insd->ins.copy(opCodes.ANDI);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->signed_offset = offset;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.ORI){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rt_val,rs_val;
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				insd->ins.copy(opCodes.ORI);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->signed_offset = offset;
				
				insd->PC = PC;
				instr.push_back(insd);

			}else if(op == opCodes.XORI){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rt_val,rs_val;
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				temp = memory[i] & mask16;
				if(temp.bits[18] == 1){
					temp = ~ temp;
					temp = temp & mask16;
					temp = temp + instr32("00000000000000000000000000000001");
					offset = -temp.toNum();
				}else{
					offset = temp.toNum();

				}

				insd->ins.copy(opCodes.XORI);
				insd->rs = rs_val;
				insd->rt = rt_val;
				insd->signed_offset = offset;
				
				insd->PC = PC;
				instr.push_back(insd);

			}

			insd = NULL;
		}

	}
	
	//function to extract opcode from string
	string format(string str){
		stringstream ss;
		string new_str;
		bool flag = 0;
		for ( std::string::iterator it=str.begin(); it!=str.end(); ++it){
			if(*it == ' ' && flag == 0){
				ss << "\t";
				flag = 1;
			}else{
				ss << *it;
			}
		}
		new_str = ss.str();
		return new_str;
	}


	//function to create memory -> instruction map
	void create_map(){
		string line;
  		ifstream myfile;
  		myfile.open("generated_disassembly.txt");
  		int PC = 256;
  		int i = 0;
    	while ( i < code_size ){

      		getline (myfile,line);
      		
      		m2c.insert ( pair<int,string>(PC + i*4,line) );

      		i++;
    	}
    	myfile.close();
	}

	int convert_PC_to_index(int pc){
		int base = 256;
		return (pc - 256) / 4;
	}

	void print_all(){
		string s_fetch_queue[2],s_pre_issue[4],s_pre_ALU1[2],s_pre_MEM,s_post_MEM,s_post_ALU2,s_pre_ALU2[2];
		s_fetch_queue[0] = " ";
		s_fetch_queue[1] = " ";

		s_pre_issue[0] = " ";
		s_pre_issue[1] = " ";
		s_pre_issue[2] = " ";
		s_pre_issue[3] = " ";		

		s_pre_ALU1[0] = " ";
		s_pre_ALU1[1] = " ";

		s_pre_ALU2[0] = " ";
		s_pre_ALU2[1] = " ";

		s_post_ALU2 = " ";

		s_pre_MEM = " ";
		s_post_MEM = " ";

		/*
		if(pre_issue[0] != NULL){
			cout << "ISSUE NOT NULL" << endl;
			cout << pre_issue[0]->PC << endl;
			cout << convert_PC_to_index(pre_issue[0]->PC) << endl;

			s_pre_issue[0]  = "[" + instr_str[convert_PC_to_index(pre_issue[0]->PC)] + "]";
			cout << "Pre-Issue Queue:" << endl;
			cout << "\tEntry 0: " << s_pre_issue[0] << endl;
		}
		*/
		
		if(fetch_queue[0] != NULL)
			s_fetch_queue[0]  = "[" + instr_str[convert_PC_to_index(fetch_queue[0]->PC)] + "]";
		if(fetch_queue[1] != NULL)
			s_fetch_queue[1]  = "[" + instr_str[convert_PC_to_index(fetch_queue[1]->PC)] + "]";

		if(pre_issue[0] != NULL)
			s_pre_issue[0]  = "[" + instr_str[convert_PC_to_index(pre_issue[0]->PC)] + "]";
		if(pre_issue[1] != NULL)
			s_pre_issue[1]  = "[" + instr_str[convert_PC_to_index(pre_issue[1]->PC)] + "]";
		if(pre_issue[2] != NULL)
			s_pre_issue[2]  = "[" + instr_str[convert_PC_to_index(pre_issue[2]->PC)] + "]";
		if(pre_issue[3] != NULL)
			s_pre_issue[3]  = "[" + instr_str[convert_PC_to_index(pre_issue[3]->PC)] + "]";

		if(pre_ALU1[0] != NULL)
			s_pre_ALU1[0]  = "[" + instr_str[convert_PC_to_index(pre_ALU1[0]->PC)] + "]";
		if(pre_ALU1[1] != NULL)
			s_pre_ALU1[1]  = "[" + instr_str[convert_PC_to_index(pre_ALU1[1]->PC)] + "]";

		if(pre_ALU2[0] != NULL)
			s_pre_ALU2[0]  = "[" + instr_str[convert_PC_to_index(pre_ALU2[0]->PC)] + "]";
		if(pre_ALU2[1] != NULL)
			s_pre_ALU2[1]  = "[" + instr_str[convert_PC_to_index(pre_ALU2[1]->PC)] + "]";

		if(post_ALU2[0] != NULL)
			s_post_ALU2  = "[" + instr_str[convert_PC_to_index(post_ALU2[0]->PC)] + "]";

		if(pre_MEM[0] != NULL)
			s_pre_MEM  = "[" + instr_str[convert_PC_to_index(pre_MEM[0]->PC)] + "]";
		if(post_MEM[0] != NULL)
			s_post_MEM  = "[" + instr_str[convert_PC_to_index(post_MEM[0]->PC)] + "]";

		cout << "Cycle:" << CYCLE << endl;
		cout << "IF Unit:" << endl;
		cout << "\tWaiting Instruction: " << s_fetch_queue[1] << endl;
		cout << "\tExecuted Instruction: " << s_fetch_queue[0] << endl;
		
		cout << "Pre-Issue Queue:" << endl;
		cout << "\tEntry 0: " << s_pre_issue[0] << endl;
		cout << "\tEntry 1: " << s_pre_issue[1] << endl;
		cout << "\tEntry 2: " << s_pre_issue[2] << endl;
		cout << "\tEntry 3: " << s_pre_issue[3] << endl;

		cout << "Pre-ALU1 Queue:" << endl;
		cout << "\tEntry 0: " << s_pre_ALU1[0] << endl;
		cout << "\tEntry 1: " << s_pre_ALU1[1] << endl;

		cout << "Pre-MEM Queue: " << s_pre_MEM << endl;

		cout << "Pre-MEM Queue: " << s_post_MEM << endl;

		cout << "Pre-ALU2 Queue:" << endl;
		cout << "\tEntry 0: " << s_pre_ALU2[0] << endl;
		cout << "\tEntry 1: " << s_pre_ALU2[1] << endl;

		cout << "Post-ALU2 Queue: " << s_post_ALU2 << endl;
		
		CYCLE++;
	}

	int get_destination_reg(instr_decoded *ins){
		if(ins->ins == opCodes.ADDI || ins->ins == opCodes.ORI || ins->ins == opCodes.ANDI || ins->ins == opCodes.XORI){
			return ins->rt;
		}else{
			return ins->rd;
		}
	}

	void add_to_reg_table(instr_decoded *ins){
		int reg = get_destination_reg(ins);
		reg_table[reg] = reg_table.find(reg)->second + 1;
	}

	bool is_branch(instr_decoded *in){
		instr32 ins = in->ins;
		if(ins == opCodes.J || ins == opCodes.JR || ins == opCodes.BEQ || ins == opCodes.BGTZ || ins == opCodes.BLTZ){
			return 1;			
		}

		return 0;

	}

	bool is_indirect(instr_decoded *ins){
		if(ins->ins == opCodes.ADDI || ins->ins == opCodes.ORI || ins->ins == opCodes.ANDI || ins->ins == opCodes.XORI){
			return 1;
		}

		return 0;
	}

	
	bool no_dependancy(instr_decoded *ins){
		bool rs,rt,sa,rd;
		rs=rt=sa=rd=0;
		bool flag = 1;

		if(ins->ins == opCodes.JR){
			rs = 1;

		}else if(ins->ins == opCodes.BEQ){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.BLTZ){
			rs = 1;

		}else if(ins->ins == opCodes.BGTZ){
			rs = 1;

		}else if(ins->ins == opCodes.BREAK){
			return 1;

		}else if(ins->ins == opCodes.SW){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.LW){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.SLL){
			rt = 1;
			sa = 1; 

		}else if(ins->ins == opCodes.SRL){
			rt = 1;
			sa = 1;

		}else if(ins->ins == opCodes.SRA){
			rt = 1;
			sa = 1;

		}else if(ins->ins == opCodes.NOP){
			return 1;

		}else if(ins->ins == opCodes.ADD){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.SUB){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.MUL){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.AND){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.OR){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.XOR){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.NOR){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.SLT){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.ADDI){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.ANDI){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.ORI){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.XORI){
			rs = 1;
			rt = 1;

		}

		if(rt){
			flag = flag & (reg_table.find(ins->rt)->second == 0); 
		}
		if(rd){
			flag = flag & (reg_table.find(ins->rd)->second == 0); 
		}
		if(rs){
			flag = flag & (reg_table.find(ins->rs)->second == 0); 
		}
		if(sa){
			flag = flag & (reg_table.find(ins->sa)->second == 0); 
		}

		if(flag)
			return 1;

		return 0;
	}

	bool check_prev_dependancy(bool *regs,instr_decoded *ins){
		bool rs,rt,sa,rd;
		rs=rt=sa=rd=0;
		bool flag = 1;

		if(ins->ins == opCodes.JR){
			rs = 1;

		}else if(ins->ins == opCodes.BEQ){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.BLTZ){
			rs = 1;

		}else if(ins->ins == opCodes.BGTZ){
			rs = 1;

		}else if(ins->ins == opCodes.BREAK){
			return 1;

		}else if(ins->ins == opCodes.SW){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.LW){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.SLL){
			rt = 1;
			sa = 1; 

		}else if(ins->ins == opCodes.SRL){
			rt = 1;
			sa = 1;

		}else if(ins->ins == opCodes.SRA){
			rt = 1;
			sa = 1;

		}else if(ins->ins == opCodes.NOP){
			return 1;

		}else if(ins->ins == opCodes.ADD){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.SUB){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.MUL){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.AND){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.OR){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.XOR){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.NOR){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.SLT){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.ADDI){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.ANDI){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.ORI){
			rs = 1;
			rt = 1;

		}else if(ins->ins == opCodes.XORI){
			rs = 1;
			rt = 1;

		}

		if(rt){
			flag = flag & (regs[ins->rt] == 0); 
		}
		if(rd){
			flag = flag & (regs[ins->rd] == 0); 
		}
		if(rs){
			flag = flag & (regs[ins->rs] == 0); 
		}
		if(sa){
			flag = flag & (regs[ins->sa] == 0); 
		}

		if(flag)
			return 1;

		return 0;
	}

	bool no_issue_dependancy(int ii){
		bool regs[32];
		instr_decoded *ins;
		int i;
		for(i=0;i<32;i++){
			regs[i] = 0;
		}
		//todo not dependant on earlier not-issued instruction
		if(no_dependancy(pre_issue[i])){
			for(int i = 0;i<ii;i++){
				//check for earlier dependant
				ins = pre_issue[i];

				if(ins->ins == opCodes.SLL){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.SRL){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.SRA){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.ADD){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.SUB){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.MUL){
					regs[ins->rd] = 1;
					
				}else if(ins->ins == opCodes.AND){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.OR){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.XOR){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.NOR){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.SLT){
					regs[ins->rd] = 1;

				}else if(ins->ins == opCodes.ADDI){
					regs[ins->rt] = 1;

				}else if(ins->ins == opCodes.ANDI){
					regs[ins->rt] = 1;

				}else if(ins->ins == opCodes.ORI){
					regs[ins->rt] = 1;

				}else if(ins->ins == opCodes.XORI){
					regs[ins->rt] = 1;

				}else if(ins->ins == opCodes.LW){
					regs[ins->rt] = 1;

				}
			}

			if(check_prev_dependancy(regs,pre_issue[ii])){
				return 1;
			}

		}

		return 0;
		
	}

	//todo
	void execute_branch(instr_decoded *ptr){
		int pc = 0;
		if(ptr->ins == opCodes.J){
			pc = ptr->signed_offset - 4;
			PC = convert_PC_to_index(pc);

		}else if(ptr->ins == opCodes.JR){
			pc = twos_complement_2_num(R[ptr->rs]) - 4;
			PC = convert_PC_to_index(pc);

		}else if(ptr->ins == opCodes.BEQ){
			long rs_val = twos_complement_2_num(R[ptr->rs]);
			long rt_val = twos_complement_2_num(R[ptr->rt]);

			if(rs_val == rt_val){
				pc = ptr->PC + 4 + ptr->signed_offset - 4;
				PC = convert_PC_to_index(pc);
			}

		}else if(ptr->ins == opCodes.BLTZ){
			long rs_val = twos_complement_2_num(R[ptr->rs]);

			if(rs_val < 0){
				pc = ptr->PC + 4 + ptr->signed_offset - 4;
				PC = convert_PC_to_index(pc);
			}

		}else if(ptr->ins == opCodes.BGTZ){
			long rs_val = twos_complement_2_num(R[ptr->rs]);

			if(rs_val > 0){
				pc = ptr->PC + 4 + ptr->signed_offset - 4;
				PC = convert_PC_to_index(pc);
			}

		}
	}
	
	void add_to_pre_issue_queue(instr_decoded *ins){
		pre_issue_queue_entries++;
		pre_issue[pre_issue_queue_entries-1] = ins;
	}
	
	void fetch_instr1(){
		instr_decoded *instr_fetch = instr[PC];

		if(fetch_queue[1] != NULL){
			if(no_dependancy(instr_fetch)){
				PC++;
				execute_branch(instr_fetch);
			}
			return;
		}
		if(is_branch(instr_fetch)){
			if(no_dependancy(instr_fetch)){
				PC++;
				fetch_queue[0] = instr_fetch;
				execute_branch(instr_fetch);
			}else{
				PC++;
				fetch_queue[1] = instr_fetch;
			}
		}else{
			if(pre_issue_queue_entries == 4){
				return;
			}else{
				PC++;
				add_to_pre_issue_queue(instr_fetch);
			}
		}

		
	}

	void fetch_instr(){
		instr_decoded *instr_fetch = instr[PC];

		if(fetch_queue[1] != NULL){
			if(no_dependancy(instr_fetch)){
				PC++;
				execute_branch(instr_fetch);
				fetch_queue[1] = NULL;
			}
			return;
		}
		if(is_branch(instr_fetch)){
			if(no_dependancy(instr_fetch)){
				PC++;
				fetch_queue[0] = instr_fetch;
				execute_branch(instr_fetch);
				fetch_queue[0] = NULL;
			}else{
				PC++;
				fetch_queue[1] = instr_fetch;
			}
		}else{
			if(pre_issue_queue_entries == 4){
				return;
			}else{
				PC++;
				add_to_pre_issue_queue(instr_fetch);
				fetch_instr1();
			}
		}

		
	}

	void add_to_pre_ALU1_queue(instr_decoded *ins){
		
		if(pre_ALU1[0] == NULL){
			pre_ALU1[0] = ins;
		}else if(pre_ALU1[1] == NULL){
			pre_ALU1[0] = ins;
		}

	}

	void add_to_pre_ALU2_queue(instr_decoded *ins){
		
		if(pre_ALU2[0] == NULL){
			pre_ALU2[0] = ins;
		}else if(pre_ALU2[1] == NULL){
			pre_ALU2[0] = ins;
		}
	}

	

	void remove_from_pre_issue(int i){
		int ii = 0;

		add_to_reg_table(pre_issue[i]);

		if(i == 3){
			pre_issue[3] = NULL;
		}else{
			for(ii=i;ii<pre_issue_queue_entries-1;ii++){
				pre_issue[ii] = pre_issue[ii+1];
			}
			pre_issue[ii] = NULL;
		}
		pre_issue_queue_entries--;
	}

	bool no_more_SW(int ii){
		for(int i=ii;i<pre_issue_queue_entries;i++){
			if(pre_issue[i]->ins == opCodes.SW){
				return 1;
			}
		}
		return 0;
	}

	void issue_instr(){
		//todo -> no WAR hazard with earlier not-issued instruction
		int counter = 0;
		bool ALU1flag = 0;
		bool ALU2flag = 0;

		for(int i=0; i < pre_issue_queue_entries; i++){
			if(no_issue_dependancy(i) && counter < 2){
				if(pre_issue[i]->ins == opCodes.LW || pre_issue[i]->ins == opCodes.SW){
					if(pre_ALU1_queue_entries < 2 && ALU1flag == 0){
						if(pre_issue[i]->ins == opCodes.LW && no_more_SW(i)){
							add_to_pre_ALU1_queue(pre_issue[i]);
							remove_from_pre_issue(i);
						}else if(pre_issue[i]->ins == opCodes.SW){
							add_to_pre_ALU1_queue(pre_issue[i]);
							remove_from_pre_issue(i);
						}
						
						ALU1flag = 1;
					}
				}else{
					if(pre_ALU2_queue_entries < 2 && ALU2flag == 0){
						add_to_pre_ALU2_queue(pre_issue[i]);
						remove_from_pre_issue(i);
						ALU2flag = 1;
					}
				}

				counter++;
			}
			
		}
		

	}

	void ALU1_exec(){
		instr_decoded *instr_ALU1 = post_MEM[0];

		if(instr_ALU1->ins == opCodes.LW){
			long rs_val = twos_complement_2_num(R[instr_ALU1->rs]);
		
			R1[instr_ALU1->rt] = memory[((rs_val + instr_ALU1->signed_offset) - 256)/4];
			reg_table1[instr_ALU1->rt] = reg_table.find(instr_ALU1->rt)->second - 1;

		}
	}

	void ALU1(){
		if(pre_ALU1[0] == NULL){
			return;
		}

		pre_MEM[0] = pre_ALU1[0];
		pre_ALU1[0] = pre_ALU1[1];
		pre_ALU1[1] = NULL;
	}

	void ALU2(){
		if(pre_ALU2[0] == NULL){
			return;
		}

		post_ALU2[0] = pre_ALU2[0];
		pre_ALU2[0] = pre_ALU2[1];
		pre_ALU2[1] = NULL;
	}

	void ALU2_exec(){
		instr_decoded *instr_ALU2 = post_ALU2[0];

		if(instr_ALU2->ins == opCodes.SLL){
			R1[instr_ALU2->rd] = R[instr_ALU2->rt] << instr_ALU2->sa; 
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.SRL){
			R1[instr_ALU2->rd] = R[instr_ALU2->rt] >> instr_ALU2->sa; 
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.SRA){
			R1[instr_ALU2->rd] = R[instr_ALU2->rt].SRA(R[instr_ALU2->rt],instr_ALU2->sa);
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.ADD){
			R1[instr_ALU2->rd] = R[instr_ALU2->rs] + R[instr_ALU2->rt];
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.SUB){
			long rs_val = twos_complement_2_num(R[instr_ALU2->rs]);
			long rt_val = twos_complement_2_num(R[instr_ALU2->rt]);

			R1[instr_ALU2->rd] = num_2_twos_complement(rs_val - rt_val);
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.MUL){
			long rs_val = twos_complement_2_num(R[instr_ALU2->rs]);
			long rt_val = twos_complement_2_num(R[instr_ALU2->rt]);

			//cout << rs_val << endl;
			//cout << rt_val << endl;


			R1[instr_ALU2->rd] = num_2_twos_complement(rs_val * rt_val);
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;
			//R[instr_ALU2->rd].print();
			//cout << endl;

		}else if(instr_ALU2->ins == opCodes.AND){
			R1[instr_ALU2->rd] = R[instr_ALU2->rs] & R[instr_ALU2->rt];
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.OR){
			R1[instr_ALU2->rd] = R[instr_ALU2->rs] | R[instr_ALU2->rt];
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.XOR){
			R1[instr_ALU2->rd] = R[instr_ALU2->rs] ^ R[instr_ALU2->rt];
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.NOR){
			R1[instr_ALU2->rd] = ~(R[instr_ALU2->rs] | R[instr_ALU2->rt]);
			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.SLT){
			long rs_val = twos_complement_2_num(R[instr_ALU2->rs]);
			long rt_val = twos_complement_2_num(R[instr_ALU2->rt]);

			if(rs_val < rt_val){
				R1[instr_ALU2->rd] = 1;
			}

			reg_table1[instr_ALU2->rd] = reg_table.find(instr_ALU2->rd)->second - 1;

		}else if(instr_ALU2->ins == opCodes.ADDI){
			long rs_val = twos_complement_2_num(R[instr_ALU2->rs]);
			long rt_val = twos_complement_2_num(R[instr_ALU2->rt]);

			R1[instr_ALU2->rt] = R[instr_ALU2->rs] + num_2_twos_complement(instr_ALU2->signed_offset);
			reg_table1[instr_ALU2->rt] = reg_table.find(instr_ALU2->rt)->second - 1;

		}else if(instr_ALU2->ins == opCodes.ANDI){
			long rs_val = twos_complement_2_num(R[instr_ALU2->rs]);
			long rt_val = twos_complement_2_num(R[instr_ALU2->rt]);

			R1[instr_ALU2->rt] = R[instr_ALU2->rs] & num_2_twos_complement(instr_ALU2->signed_offset);
			reg_table1[instr_ALU2->rt] = reg_table.find(instr_ALU2->rt)->second - 1;

		}else if(instr_ALU2->ins == opCodes.ORI){
			long rs_val = twos_complement_2_num(R[instr_ALU2->rs]);
			long rt_val = twos_complement_2_num(R[instr_ALU2->rt]);

			R1[instr_ALU2->rt] = R[instr_ALU2->rs] | num_2_twos_complement(instr_ALU2->signed_offset);
			reg_table1[instr_ALU2->rt] = reg_table.find(instr_ALU2->rt)->second - 1;

		}else if(instr_ALU2->ins == opCodes.XORI){
			long rs_val = twos_complement_2_num(R[instr_ALU2->rs]);
			long rt_val = twos_complement_2_num(R[instr_ALU2->rt]);

			R1[instr_ALU2->rt] = R[instr_ALU2->rs] ^ num_2_twos_complement(instr_ALU2->signed_offset);
			reg_table1[instr_ALU2->rt] = reg_table.find(instr_ALU2->rt)->second - 1;

		}
	}

	void MEM_load_store(){
		if(pre_MEM[0] == NULL){
			return;
		}

		instr_decoded *instr_LS = pre_MEM[0];

		if(instr_LS->ins == opCodes.LW){
			post_MEM[0] = instr_LS;
		}else{
			long rs_val = twos_complement_2_num(R[instr_LS->rs]);
			memory[((rs_val + instr_LS->signed_offset) - 256)/4] = R[instr_LS->rt];
		}
		pre_MEM[0] = NULL;

	}

	void WB_data(){
		if(post_MEM[0] != NULL){
			ALU1_exec();
			post_MEM[0] = NULL;
		}

		if(post_ALU2[0] != NULL){
			ALU2_exec();
			post_ALU2[0] = NULL;
		}

	}

	void simulate(){
		PC = 0;
		CYCLE = 1;

		while(CYCLE < 30){
			WB_data();
			MEM_load_store();
			ALU2();
			ALU1();
			issue_instr();
			fetch_instr();			
			
			print_all();
		}
	}

	void print_instr_str(){
		cout << instr_str.size() << endl;
		for(vector<string>::iterator it = instr_str.begin(); it != instr_str.end(); ++it){

			cout << *it << endl;
		}
	}

};



// main function
int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "Usage: ./MIPSsim <file>" << endl;
	}else{
		Dissassembler dis;
		dis.file2memory(argv[1]);
		dis.disassemble_instructions();
		dis.disassemble_data();
		//dis.R[30] = 340;
		//dis.R[1] = 8;
		//dis.R[2] = dis.num_2_twos_complement(-3);
		//dis.R[8] = dis.num_2_twos_complement(-2);
		//dis.R[5] = dis.num_2_twos_complement(-9);
		//dis.R[6] = dis.num_2_twos_complement(-5);
		//dis.R[9] = dis.num_2_twos_complement(10);
		dis.decoder();
		dis.create_map();
		dis.simulate();

		//cout << dis.code_size << endl;
	}
	


	/*
	cout << "--------------------------------------------------------" << endl;
	instr32 ins = dis.make_mask(1,4,instr32("10101010101010101010101010101010"));
	ins.print();	cout << endl;

	ins = dis.num_2_twos_complement(-1);
	ins.print();	cout << endl;

	cout << dis.twos_complement_2_num(instr32("11111111111111111111111111111111")) << endl;

	instr32 one, two;
	one = dis.num_2_twos_complement(-1);
	two = dis.num_2_twos_complement(-3);
	ins = one + two;

	cout << dis.twos_complement_2_num(ins) << endl;

	dis.print_data();
	cout << endl << endl;
	dis.print_regs();
	
	instr32 ins("00001111111111111111111111111111");
	ins = ins.SRA(ins,3);
	ins.print();
	cout << endl;
	*/
	return 0;
}