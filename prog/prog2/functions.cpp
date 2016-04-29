#include "Euler.h"
//x
double f1(double x[DIM], double y[DIM]){
	double a = 0;
	double b = 0;
	a = x[1];
	b = y[1];
	return -x[0] + y[0] + h*(a + b)/2;
}
//dx 
double f2(double x[DIM], double y[DIM]){
	double a = 0;
	double b = 0;

	a = -x[0];
	b = -y[0];
 	return - x[1] + y[1] + h*(a + b)/2;
}
//y
double f3(double x[DIM], double y[DIM]){
	double a = 0;
	double b = 0;

	a = x[3];
	b = y[3];
   return -x[2] + y[2] + h*(a + b)/2;
}
//dy
double f4(double x[DIM], double y[DIM]){
	double a = 0;
	double b = 0;
	a = -x[2];
	b = -y[2];
   return -x[3] + y[3] + h*(a + b)/2;  
}