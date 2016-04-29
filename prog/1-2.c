/* 
 * Явный нелинейный случай
 * Метод прогонки 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Для strcat
#include <math.h>
#define tau 0.0001
#define T_max 1.0 / tau
#define DIM 2000
#define h 2.0/DIM
#define w 0.1
#define A -2 * h * h/(2 * h * h + w * tau * tau)
#define B -tau *(w * tau - h)/(4 * h * h + 2 * w * tau * tau)
#define C -tau *(w * tau + h)/(4 * h * h + 2 * w * tau * tau)

/* начальные условия t = 0 */
double initial_conditions(double x){
	double u = 0.0;
	if ((x > 0.0) && (x<= 0.25)) u = 4*x;
	if (x > 0.25) u = 1.0;
	return u; 
}


void G(double *v, double *v_new, double *b){
	b[0] = A * v[0] + v_new[0] + C * v_new[1];
	b[DIM-1] = A * v[DIM-1] + B * v_new[DIM-1] + v_new[DIM];
	for (int i = 1; i < DIM-1; i++){
		b[i] = A * v[i] + B * v_new[i-1] + v_new[i] + C * v_new[i+1];
	}
}


/* зададим начальные условия 
 * идём по x: всего DIM точек на расстояние 2, начинаем из -1 c шагом 2/DIM
 * x = (2.0/DIM) * i - 1.0
 */
void create_initals(double *u){
	for (int i = 0; i < DIM; i++)
		u[i] = initial_conditions((2.0/DIM) * i - 1.0);
}


/* Решение системы с помощью LU разложения */	
void Solve(double a, double c, double d, double *b, double *x){
	double u[DIM-1]; // u
	double v[DIM];
	double l[DIM];
	double y[DIM];
	
	/* LU разложение */
	v[1] = d;
	l[1] = a / v[1];
	u[1] = c / l[1];
	for (int i=2;i<DIM-2;i++){
		v[i] = d;
		l[i] = (a - u[i-1])/v[i];
		u[i] = c / l[i];
	}
	v[DIM-2] = 1.0;
	l[DIM-2] = a - u[DIM-2];



	/* найдём y */
	y[1] = b[1]/l[1];
	printf("!!%e!!\n",y[1]);

	for (int i=2;i<DIM-1;i++){
		y[i] = (b[i] - y[i-1])/l[i];
	//	printf("%i %e %e %e %e \n",i,y[i], b[i],y[i-1], l[i] );
}	
//	for (int i=2;i<DIM-1;i++)
//		printf("%i %e \n",i,y[i]);

	/* Найдём x */
	x[DIM-2] = y[DIM-2];
	for (int i=DIM-3;i>0;i--){
		if (x[i]< 1e-40) x[i] = 0.;
		x[i] = (y[i] - u[i]*x[i+1])/v[i];
	}
}
double norm(double *u){
	double max = 0;
	for (int i = 0; i < DIM;i++)
		if (fabs(u[i]) > max) max = fabs(u[i]);
	return max;
}

/* с помощью моей схемы */
void count_new_layer(double *u, double *u1){
	double b[DIM];
	double x[DIM];
	double a = 1;
	double c = B;
	double d = C;

	G(u,u1,b);

//	for (int i=1;i<DIM-1;i++)
//		printf("%i %e \n",i,b[i] );
	printf("!%e\n", norm(b));
	while(norm(b) > 0.001)
	{
		Solve(a,c,d,b,x);
		for (int i=1;i<DIM-1;i++){
			printf("%i %e\n",i,x[i] );
			u1[i] = u1[i] - x[i];
		}
		G(u,u1,b);
//		for (int i=1;i<DIM-1;i++)
//			printf("%i %e\n",i,b[i] );
		printf("%e\n",norm(b) );
//		printf("A = %e\n",A );
//		printf("B = %e\n",B );
//		printf("C = %e\n",C );
	}
}


/*
 * Выведем слой в файл соответствующего времени
 */
void print(double *u, int num){
	FILE *f;
	char name[100] = "calculations1/"; // папка
	char number[100] ; // номер
	char ext[100]  = ".txt"; // расширение
	double x;

 	sprintf(number, "%d", num);
 	strcat(name, number);
 	strcat(name, ext);
	f = fopen(name, "w");
	for (int i=0;i < DIM; i++){
		x = (2.0/DIM) * i - 1.0;
		fprintf(f, "%e\t%e\n", x,u[i]);
	}
	fclose(f);
}

int main(void){
	double *u;
	double *u1;


	u = (double*)malloc(DIM*sizeof(double));
	u1 = (double*)malloc(DIM*sizeof(double));
	system("rm -r -f calculations1");
	system("mkdir calculations1"); // в папке calculations будут лежать все расчёты

//	printf("%e\n", tau/h );

	create_initals(u);
	print(u, 0);
	int t = 1;
	//for (int t = 1; t <= T_max;t++){
		count_new_layer(u,u1);
		memcpy(u, u1, DIM*sizeof(double));
		print(u1, t);
	//}

	free(u);
	free(u1);
	return 0;
}
