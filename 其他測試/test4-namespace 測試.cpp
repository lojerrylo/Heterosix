#include<iostream>
using namespace std;
namespace testing{
	int a = 0;
}



int main(){
	testing::a+=1;
	cout << testing::a ;
	system("pause");
	return 0;
}
