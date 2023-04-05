#include<iostream>
#include<cstdlib>
#include<iomanip>
using namespace std;
#define RF 0b00000000000000000000000000000001uL 
void co(unsigned int k){
	cout << dec << setw(20) << k << setw(20) << oct << k << endl;
}
int main(){
	unsigned long a = RF;//¤£·|´`Àô 
//	short a = 1;
	cout << showbase;
	for(int i = 0; i < 34; i++){
		co(a);
		a <<= 1;
	}
	// co(sl[0] | sl[1] | sl[2] );
	
	system("pause");
	return 0;
}
/*
		RF	RM	RB	LF	LM	LB  Arm 	
	0b0 000 000 000 000 000 000 000000 000 0001u
	
	
*/ 
