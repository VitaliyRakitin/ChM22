#include "Euler.h"


#define eps 10e-10


/* Поиск решения системы уравнений метода Ньютона */
void Newton( double x[DIM], double prev[DIM], double (*f[DIM])(double*, double*)){
	matrix A(0);
	matrix B(0);
	matrix C(0);
	double y[DIM];
	double part_time[DIM];

	A = Jacobian(x, prev, f);
	B = Vector_Func(x, prev, f);
	C = C.ArrayToMatrix(x, DIM).transpose();
	A = C - A.Back() * B;
	toArray(y, A);

	while( norm(x,prev,f) > eps){
		for (int i = 0; i< DIM; i++) x[i] = y[i];

		A = Jacobian(x, prev, f); 
		B = Vector_Func(x, prev, f);
		C = C.ArrayToMatrix(x, DIM).transpose();
		A = C - A.Back() * B;
		toArray(y, A);
	}
	for (int i = 0; i< DIM; i++) x[i] = y[i];
}

/* подсчёт матрицы Якоби */
matrix Jacobian(double x[DIM], double prev[DIM], double (*f[DIM])(double*, double*)){
	matrix A(DIM);
		for (int i = 0; i < DIM; i++)
			for (int j = 0; j < DIM;j++)
				A.matr[i][j] = count_derivative(j,x,prev,f[i]);

	return A;
}

/* возвращает максимальное значение вектора функций */
double norm(double x[DIM], double prev[DIM], double (*f[DIM])(double*, double*)){
	double max = 0;
	double perm = 0;
	for (int i = 0;i < DIM; i++){
		perm = abs((f[i])(x,prev));
		if (perm > max) max = perm;
	}
	return perm;
}

/* модуль */
double abs(double a){
	if (a > 0) return a;
	else return -a;
}

/* считаем производную */
double count_derivative(int num, double x[DIM], double prev[DIM], double  (*f)(double*, double*)){
	double New[DIM];
	for (int i = 0; i<DIM; i++)
		if (i == num) New[i] = x[i] + eps;
		else New[i] = x[i];
	return (f(New,prev) - f(x,prev))/eps;
}

/* создаём вектор функцию в точке */
matrix Vector_Func(double x[DIM], double prev[DIM], double (*f[DIM])(double*, double*)){
	matrix A(DIM, 1);
		for (int i = 0; i < DIM; i++)
				A.matr[i][0] = (f[i])(x,prev);
	return A;
}

/* преобразуем матрицу в массив */
void toArray(double x[DIM], matrix A){
	if ((A.sto == DIM)&&(A.str == 1)){
		for (int i = 0; i < DIM; i++) x[i] = A.matr[0][i];
	}
	else if ((A.sto == 1)&&(A.str == DIM)){
		for (int i = 0; i < DIM; i++) x[i] = A.matr[i][0];
	}
	else {printf("ERROR! Wrong toArray!\n"); throw(1);}
}
