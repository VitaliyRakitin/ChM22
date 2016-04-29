/*
 * Технические функции класса Matrix
 */


#include "matrix.h"

/* Обнуление матрицы */
void matrix::zero(){
	if ((str > 0)&&(sto > 0))
		for (int i=0;i<str;i++)
			for (int j=0;j<sto;j++)
				matr[i][j] = 0;
}

/* создать матрицу n x m */
void matrix::create(size_t n, size_t m){
	matr = new double *[n];
	for (int i = 0; i < n; i++)
		matr[i] = new double [m];
	str = n;
	sto = m;
}

/* очистить матрицу */
void matrix::free(){
	if ((str > 0)||(sto > 0)){
		for (int i=0;i<str;i++)
			delete[] matr[i];
		delete[] matr;
	str = 0;
	sto = 0;
	}
}

/* сделать копию матрицы m */
void matrix::copy(const matrix &m){
	free();
	create(m.str, m.sto);
	for (int i=0;i<str;i++)
		for (int j=0;j<sto;j++)
			matr[i][j] = m.matr[i][j];
}

/* Конструктор, n --- строк, m --- столбцов */
matrix::matrix(size_t n, size_t m){
	create(n,m);
	zero();
}

/* Конструктор матрицы n x n */
matrix::matrix(size_t n){
	create(n,n);
	zero();
}

/* Конструктор копирования */
matrix::matrix(const matrix &m){
	str = 0;
	sto = 0;
	copy(m);
}

/* Деструктор */
matrix::~matrix(){
	free();
}

/* Вывод в std */
void matrix::print(){
	if ((str > 0)&&(sto > 0))
		for (int i=0;i<str;i++){
			for (int j=0;j<sto;j++)
				printf("%lf ",matr[i][j]); 
			printf("\n");
		}
	else printf("Free Matrix.\n");
	printf("\n");
}

/* вернуть копию матрицы this */
matrix matrix::duplicate(){
	matrix A(str,sto);
	for (int i=0;i< str; i++)
		for (int j=0;j<sto;j++)
			A.matr[i][j] = matr[i][j];
	return A;
}

/* Задание единичная матрица */
matrix matrix:: Identity(){
    matrix A(str);
    for (int i=0;i<A.str; i++) A.matr[i][i] = 1;
    return A;
}