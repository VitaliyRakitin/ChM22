/*
 * Реализация подсчёта определителя матрицы методом Гаусса из класса matrix
 */
#include "matrix.h"

double matrix::det(){
	double det = 1;

	matrix A(0);
	A = duplicate();
	int min;
	if (str < sto) min = str;
	else min = sto;

	for (int i=0;i < min;i++){ 
		if (A.matr[i][i] == 0)
			for (int j=i; j< min; j++)
				if (A.matr[j][i] != 0) A.swap(i,j);
		
		if (A.matr[i][i] == 0){ det = 0; break;}
		det = det * A.matr[i][i];
		A.Del(i,A.matr[i][i]);
		for (int j = i+1;j<str;j++) A.Minus(j,i,A.matr[j][i]);
	}

	return det;
}

/* из n строки, начиная с k-го элемента, вычесть k-ю умноженную на multy */
void matrix::Minus(int n, int k, double multy){
	for (int i = k;i<sto;i++) {
		matr[n][i] = matr[n][i] - matr[k][i]*multy;
	}
}

/* разделить n строку, начиная с n-го элемента, на val */
void matrix::Del(int n, double val){
	for (int i = n;i<sto;i++) matr[n][i] = matr[n][i]/val;
}

/* поменять местами i и j строки */
void matrix::swap(int i, int j){
	double *c;
	c = matr[i];
	matr[i] = matr[j];
	matr[j] = c; 
}