#include<iostream>
using namespace std;
unsigned long isqrt32 (unsigned long n) //
{
        unsigned long root;
        unsigned long remainder;
        unsigned long  place;

        root = 0;
        remainder = n;
        place = 0x40000000; // OR place = 0x4000; OR place = 0x40; - respectively

        while (place > remainder)
        place = place >> 2;
        while (place)
        {
                if (remainder >= root + place)
                {
                        remainder = remainder - root - place;
                        root = root + (place << 1);
                }
                root = root >> 1;
                place = place >> 2;
        }
        return root;
}
int main(){
	unsigned long k = 121;
	unsigned long sk = isqrt32(k);
	cout << sk;
	
	system("pause");
	return 0;
}
