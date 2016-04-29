
#include "matrix.h"

/* оператор деления матрицы на число */
matrix matrix::operator/ (double val) 
{	matrix New(*this);
	if (val == 0) {
		printf("zero division error\n");
		throw(2);
	}
	else{
		for (int i=0;i<str;i++)
			for (int j =0; j< sto;j++){
				New.matr[i][j] = matr[i][j]/val;
			}
		}
	return New;
}

matrix matrix::operator* (double val) 
{	matrix New(*this);	
	for (int i=0;i<str;i++)
		for (int j =0; j< sto;j++){
			New.matr[i][j] = matr[i][j]*val;
		}

	return New;
}

matrix matrix::operator* (matrix A) 
{
	if (sto != A.str){
		printf("ERROR! Multyplication\n");
		throw(3);
	}

	matrix New(str, A.sto);

	for (int i = 0; i< New.str; i++)
		for (int j = 0; j< New.sto; j++){
			for (int k = 0; k< sto; k++)
				New.matr[i][j] += matr[i][k] * A.matr[k][j];
		}
			 
	return New;
}

matrix matrix::operator+ (matrix A) 
{
	if ((sto != A.sto)||(str!=A.str)){
		printf("ERROR! Sum \n");
		throw(4);
	}

	matrix New(*this);

	for (int i = 0; i< New.str; i++)
		for (int j = 0; j< New.sto; j++)
				New.matr[i][j] += A.matr[i][j];
 
	return New;
}


matrix matrix::operator- (matrix A) 
{
	if ((sto != A.sto)||(str!=A.str)){

		printf("!!!! %d %d %d %d\n", sto, str, A.sto, A.str);
		printf("ERROR! Minus\n");
		throw(5);
	}

	matrix New(*this);

	for (int i = 0; i< New.str; i++)
		for (int j = 0; j< New.sto; j++)
				New.matr[i][j] -= A.matr[i][j];
				 
	return New;
}

