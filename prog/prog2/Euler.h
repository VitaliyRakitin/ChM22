
#include "Matrix.h"

#define DIM 4
void Newton( double x[DIM], double prev[DIM], double (*f[DIM])(double*, double*));
double  f1(double x[DIM], double y[DIM]);
double  f2(double x[DIM], double y[DIM]);
double  f3(double x[DIM], double y[DIM]);
double  f4(double x[DIM], double y[DIM]);
void Euler();

matrix Jacobian(double x[DIM], double prev[DIM], double (*f[DIM])(double*, double*));
double count_derivative(int num, double x[DIM],double prev[DIM], double (*f)(double*,double*));
matrix Vector_Func(double x[DIM], double prev[DIM],double (*f[DIM])(double*, double*));
void toArray(double x[DIM], matrix A);
double norm(double x[DIM],double prev[DIM], double (*f[DIM])(double*, double*));
double abs(double a);

void Create_Initials(double x[DIM], double (*f[DIM])(double*, double*));

#define TIME_LIMIT 30

#define h 10e-5 
