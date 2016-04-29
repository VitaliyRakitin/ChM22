/* 
 * Явный нелинейный случай 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Для strcat
#define DIM 1000
#define h 0.0001
#define tau 0.1
#define T_max 1.0 / tau

/* начальные условия t = 0 */
double initial_conditions(double x){
	double u = 0.0;
	if ((x > 0.0) && (x<= 0.25)) u = 4*x;
	if (x > 0.25) u = 1.0;
	return u; 
}

/* зададим начальные условия 
 * идём по x: всего DIM точек на расстояние 2, начинаем из -1 c шагом 2/DIM
 * x = (2.0/DIM) * i - 1.0
 */
void create_initals(double *u){
	for (int i = 0; i < DIM; i++)
		u[i] = initial_conditions((2.0/DIM) * i - 1.0);
}

/* собственно моя схема */
double step(double v_last, double v_cur, double v_next){
	double v = 0.0;
	double sp = 0.5 * tau / h;
	v = sp*(v_next * v_next - v_last * v_last) + 0.5 * v_last + 0.5 * v_next;
	return v;
} 

/* посчитаем новый слой
 * bias -- смещение
 */
void count_new_layer(double *previos, double *new, int bias){
	for (int i = 0; i < bias; i++)
 		new[i] = 0.0;
 	for (int i = bias; i < DIM - bias; i++)
		new[i] = step(previos[i-1],previos[i], previos[i+1]);	
 	for (int i = DIM - bias; i< DIM; i++)
 		new[i] = 1.0;
}

/*
 * Выведем слой в файл соответствующего времени
 */
void print(double *u, int num){
	FILE *f;
	char name[100] = "calculations3/"; // папка
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
	system("rm -r -f calculations3");
	system("mkdir calculations3"); // в папке calculations будут лежать все расчёты

	printf("%e\n", tau/h );
	u = (double*)malloc(DIM * sizeof(double));
	u1 = (double*)malloc(DIM * sizeof(double));
	create_initals(u);
	print(u, 0);
	for (int t = 1; t <= T_max;t++){
		count_new_layer(u,u1,1);
		memcpy(u, u1, DIM*sizeof(double));
		print(u1, t);
	}
	
	free(u);
	free(u1);
	return 0;
}