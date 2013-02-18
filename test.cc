#include <iostream>
#include <string>

using namespace std;

class instr32{
	public:
		bool bits[32];

		instr32(){
			for(int i=0;i<32;i++)
				bits[i] = 0;
		}

		void print(){
			for(int i=0;i< 32;i++){
				cout << bits[i];
			}
			cout << endl;
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
			cout << num << endl;
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

		instr32 operator|(instr32 right){
			instr32 ins;
			for(int i=0;i< 32;i++){
				ins.bits[i] = bits[i] | right.bits[i];
			}

			return ins;
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

int main(){
	instr32 mask25to21("00000011111000000000000000000000");
	mask25to21 = mask25to21 >> 21;
	mask25to21.print();
	mask25to21 = ~mask25to21;
	mask25to21.print();

	instr32 one("00000000000000000000000000000101"), one1;
	one1.copy(one);
	instr32 ans;
	ans = one + one1;
	ans.print();
	return 0;
}