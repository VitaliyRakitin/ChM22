
#include <stdio.h>
#include <stdlib.h>


class matrix{
public:
	double **matr; 	// матрица
	int str, sto; 	// количество строк/столбцов
	
	/* конструкторы */
	matrix(size_t n, size_t m);
	matrix(size_t n);
	matrix(const matrix &m);
	/* деструктор */
	~matrix();

	void print();

	matrix &operator =(matrix m){ copy(m); return *this;} // присваивание
	matrix operator/ (double val); // Деление
	matrix operator* (double val);	
	matrix operator* (matrix A);	
	matrix operator- (matrix A); 
	matrix operator+ (matrix A);


	double det();	// определитель
	matrix Back(); 	// обратная матрица
	double ** toArray(){return matr;}
	matrix transpose(){
		matrix A(sto,str);
		for (int i = 0;i< str; i++)
			for (int j = 0; j <sto; j++)
				A.matr[j][i] = matr[i][j];
		return A;
	}

	matrix ArrayToMatrix(double *v, int len){
		matrix A(1,len);
		for (int i=0;i<len;i++) A.matr[0][i] = v[i];
		return A;
	}
private:
	/* технические функции */
	void zero();
	void free();
	void create(size_t n, size_t m);
	void copy(const matrix &m);
	matrix duplicate(); // создать копию матрицы this
	matrix Identity(); // создать единичную матрицу аналогичную исходной

	void Minus(int n, int k,double multy); // det
	void Del(int n, double ); // det
	void swap(int i, int j); //det

	void Gauss();
	void Back_Gauss();
	matrix Double_Matrix();
	matrix Single_Matrix();
	};