#include <iostream>
#include <string>
#include <map>

using namespace std;


int main(){
	map<int,int> m;
	m[1] = 2;

	cout << m[1] << endl;
	m[1] = m.find(1)->second + 1;

	cout << m[1] << endl;
	return 0;
}