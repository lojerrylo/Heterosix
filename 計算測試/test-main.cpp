#include<iostream>
#include"../Heterosix/het_math.h"
using namespace std;
int main() {
	vec4 k(1, 1, 0);
	cout << k << endl;
	k.extend(sqrt(2));
	cout << k;
	cout << k.extend(sqrt(2));



	system("pause");
	return 0;
}