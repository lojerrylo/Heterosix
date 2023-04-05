#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;
long r_cons_count = 0;
long r_copy_count = 0;
long r_assi_count = 0;
long c_cons_count = 0;
long c_copy_count = 0;
long c_assi_count = 0;
clock_t start, stop;

class r_double{ // by reference
public: 
	double x;
	r_double(){x = 0; r_cons_count+=1;}
	r_double(double xx){x = xx; r_cons_count+=1;}
	r_double(const r_double&rd){x = rd.x; r_copy_count+=1;}
	r_double&operator=(const r_double&rd){x = rd.x; r_assi_count+=1;}
	r_double operator*(r_double rd){return r_double(this->x * rd.x);}
	r_double operator+(r_double rd){return r_double(this->x + rd.x);}
};
class c_double{// by copy it
public:
	double x;
	c_double(){x = 0; c_cons_count+=1;}
	c_double(double xx){x = xx; c_cons_count+=1;}
	c_double(const c_double&rd){x = rd.x; c_copy_count+=1;}
	c_double&operator=(const c_double&rd){x = rd.x; c_assi_count+=1;}
	c_double operator*(c_double rd){return c_double(this->x * rd.x);}
	c_double operator+(c_double rd){return c_double(this->x + rd.x);}
}; 
class r_TM{
	public:
	r_double v[4][4];
	r_TM operator*(r_TM&T){
		r_TM temp;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				temp.v[i][j] = this->v[i][j] * T.v[i][j];
			}
		}
		return temp;
	}
	r_TM plus(r_TM&T){
		r_TM temp;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				temp.v[i][j] = this->v[i][j] + T.v[i][j];
			}
		}
		return temp;
	} 
};
class c_TM{
	public:
	c_double v[4][4];
	c_TM operator*(c_TM T){
		c_TM temp;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				temp.v[i][j] = this->v[i][j] * T.v[i][j];
			}
		}
		return temp;
	}
	c_TM plus(c_TM T){
		c_TM temp;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				temp.v[i][j] = this->v[i][j] + T.v[i][j];
			}
		}
		return temp;
	} 
};
double howlong(){
	//return stop - start;	
	return (double(stop - start) / double(CLOCKS_PER_SEC));
}
#define runtimes 100000
double r_test(){
	r_TM r_A; r_TM r_B;
	start = clock();
	for(int i = 0; i<runtimes; i++){
		r_TM r_E = r_A.plus(r_B);
		r_TM r_F(r_E);
	}
	stop = clock();
	return howlong();
}
double c_test(){
	c_TM c_A; c_TM c_B;	
	start = clock();
		for(int i = 0; i<runtimes; i++){
		c_TM c_E(c_A.plus(c_B));
	}
	stop = clock();
	return howlong();
}

int main(){
/*
	r_TM r_A; r_TM r_B;		// same
	r_TM r_C = r_A * r_B;	// 
	r_TM r_D(r_A*r_B);		//
	
	c_TM c_A; c_TM c_B;		// 
	c_TM c_C = c_A * c_B;	// +16
	c_TM c_D(c_A*c_B);		// +16
*/
/*	// 結果呼叫次數沒差。 
		r_TM r_E = r_A.plus(r_B);
		r_TM r_F(r_E);
		
		c_TM c_E(c_A.plus(c_B));
*/


	

	cout << "clock per sec: " << CLOCKS_PER_SEC << endl;
	start = clock();
	r_TM r_A; r_TM r_B;
	for(int i = 0; i<runtimes; i++){
		r_TM r_E = r_A.plus(r_B);
		r_TM r_F(r_E);
	}
	stop = clock();
	c_TM c_A; c_TM c_B;	
	cout << "r_test:\t" << howlong() << endl;
	start = clock();
	for(int i = 0; i<runtimes; i++){
		c_TM c_E(c_A.plus(c_B));
	}
	stop = clock();
	cout << "c_test:\t" << howlong() << endl << endl;
	
	cout << "r cons: " << r_cons_count << endl;
	cout << "r copy: " << r_copy_count << endl;
	cout << "r assi: " << r_assi_count << endl << endl;
	
	cout << "c cons: " << c_cons_count << endl;
	cout << "c copy: " << c_copy_count << endl;
	cout << "c assi: " << c_assi_count << endl << endl;
	system("pause");
	return 0;
	
}
