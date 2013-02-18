#include <string>
#include <map>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <malloc.h>

using namespace std;

#define MEM 256

class instr32{
	public:
		bool bits[32];

		instr32(){
			for(int i=0;i<32;i++)
				bits[i] = 0;
		}

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

		void print(){
			for(int i=0;i< 32;i++){
				cout << bits[i];
			}
			//cout << endl;
		}

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

		void operator=(long num){
			instr32 ins;
			copy(ins);
			int i = 31;
			//cout << num << endl;
			while(num || i < 0){
				bits[i] = (bool)num % 2;
				num = num / 2;
				//cout << "num = " << num << endl;
				i--;
			}

		}

		void copy(instr32 ins){
			for(int i=0;i< 32;i++){
				bits[i] = ins.bits[i];
			}
		}

		instr32 operator&(instr32 right){
			instr32 ins;
			for(int i=0;i< 32;i++){
				ins.bits[i] = bits[i] & right.bits[i];
			}

			return ins;
		}


		instr32 operator|(instr32 right){
			instr32 ins;
			for(int i=0;i< 32;i++){
				ins.bits[i] = bits[i] | right.bits[i];
			}

			return ins;
		}

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

		bool add(bool a,bool b){

			if(a == b)
				return 0;
			else
				return 1;

		}

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



class Dissassembler{

	public:
		instr32 R[32];
		instr32 memory[MEM];
		int data_offset;
		int instr_offset;
		int code_size;
		OPCODES opCodes;


	Dissassembler(){
		data_offset = 0;
		code_size = 0;
		instr_offset = 256;
		for(int i;i<32;i++){
			R[i] = 0;
		}
	}

	void file2memory(string file){
		string line;
  		ifstream myfile;
  		myfile.open((char*)file.c_str());
  		int i = 0;
    	while ( myfile.good() ){

      		getline (myfile,line);
      		if(line.length() != 32)
      			break;
      		memory[i++] = line;
    	}
    	myfile.close();
  		code_size = i;
	}

	void getDataOffset(){
		instr32 mask;
		mask = "11111100000000000000000000000000";
		int i = 0;
		for(i=0;i<MEM;i++){
			if((memory[i] & mask) == opCodes.BREAK)
				break;

		}
		data_offset = (i+1)*4 + 252 + 4;
	}

	long rs(instr32 ins){
		instr32 temp;
		temp.copy(ins);
		instr32 maskrs("00000011111000000000000000000000");

		temp = temp & maskrs;
		temp = temp >> 21;
		return temp.toNum(); 
	}

	long rt(instr32 ins){
		instr32 temp;
		temp.copy(ins);
		instr32 maskrt("00000000000111110000000000000000");

		temp = temp & maskrt;
		temp = temp >> 16;
		return temp.toNum(); 
	}

	long rd(instr32 ins){
		instr32 temp;
		temp.copy(ins);
		instr32 maskrt("00000000000000001111100000000000");

		temp = temp & maskrt;
		temp = temp >> 11;
		return temp.toNum(); 
	}

	long sa(instr32 ins){
		instr32 temp;
		temp.copy(ins);
		instr32 maskrt("00000000000000000000011111000000");

		temp = temp & maskrt;
		temp = temp >> 6;
		return temp.toNum(); 
	}

	void disassemble_instructions(){
		getDataOffset();
		int END_OF_INSTR = (data_offset-256)/4;
		instr32 mask;
		instr32 op, temp, tempPC;
		
		long PC = 256;

		mask = "11111100000000000000000000000000";

		//cout << "Data Offset = " << data_offset << endl;
		//cout << "END_OF_INSTR = " << END_OF_INSTR <<  endl;
		
		

		for(int i=0;i< END_OF_INSTR;i++){
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
				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "J #" << temp.toNum() << endl;

			}else if(op == opCodes.JR){
				instr32 maskrs("00000011111000000000000000000000");

				temp = memory[i] & maskrs;
				temp = temp >> 21;
				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << 'JR R' << temp.toNum() << endl;

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
				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "BEQ R" << rs_val <<", R" << rt_val <<", #" << offset << endl;

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
				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "BLTZ R" << rs_val <<", #" << offset << endl;

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
				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "BGTZ R" << rs_val <<", #" << offset << endl;

			}else if(op == opCodes.BREAK){
				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "BREAK" << endl;
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

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "SW R" << rt_val <<", " << offset << "(R" << rs_val << ")" << endl;

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

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "LW R" << rt_val <<", " << offset << "(R" << rs_val << ")" << endl;

			}else if(op == opCodes.SLL){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rd_val,rt_val,sa_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				sa_val = sa(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "SLL R" << rd_val <<", R" << rt_val << ", #" << sa_val << endl;

			}else if(op == opCodes.SRL){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rd_val,rt_val,sa_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				sa_val = sa(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "SRL R" << rd_val <<", R" << rt_val << ", #" << sa_val << endl;

			}else if(op == opCodes.SRA){
				instr32 mask16("00000000000000001111111111111111");
				long offset;
				long rd_val,rt_val,sa_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				sa_val = sa(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "SRA R" << rd_val <<", R" << rt_val << ", #" << sa_val << endl;

			}else if(op == opCodes.NOP){
				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "NOP" << endl;

			}else if(op == opCodes.ADD){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "ADD R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

			}else if(op == opCodes.SUB){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "SUB R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

			}else if(op == opCodes.MUL){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "MUL R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

			}else if(op == opCodes.AND){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "AND R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

			}else if(op == opCodes.OR){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "OR R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

			}else if(op == opCodes.XOR){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "XOR R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

			}else if(op == opCodes.NOR){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "NOR R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

			}else if(op == opCodes.SLT){
				long rd_val,rt_val,rs_val;
				rd_val = rd(memory[i]);
				rt_val = rt(memory[i]);
				rs_val = rs(memory[i]);

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "SLT R" << rd_val <<", R" << rs_val << ", R" << rt_val << endl;

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

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "ADDI R" << rt_val <<", R" << rs_val << ", #" << offset << endl;

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

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "ANDI R" << rt_val <<", R" << rs_val << ", #" << offset << endl;

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

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "ORI R" << rt_val <<", R" << rs_val << ", #" << offset << endl;

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

				memory[i].print();
				cout << "\t" << PC << "\t" ;
				cout << "XORI R" << rt_val <<", R" << rs_val << ", #" << offset << endl;

			}
		}
		
	}

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

			memory[i].print();
			cout << "\t" << PC << "\t" ;
			cout << num << endl;
		}
	}

};




int main(){

	
	Dissassembler dis;
	dis.file2memory("sample.txt");
	dis.disassemble_instructions();
	dis.disassemble_data();

	
	return 0;
}