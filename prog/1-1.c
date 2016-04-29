/* 
 * Явный линейный случай 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Для strcat
#define h 2e-03
#define tau 1e-4
#define DIM 2000
#define T_max 1.0 / tau

double analitic(double x, double t);
double myabs(double a);
double step(double v_last, double v_cur, double v_next);
double norm_Ch(double *v);
double norm_L(double *v);
double norm_Ch_analitic(double *v, double t);
double norm_L_analitic(double *v, double t);
void create_layer(double *u, double t);
void count_new_layer(double *previos, double *new, int bias);
int print(char dir[100], double *u, int num);
int print_diff(char dir[100], double *u, int num);
int main(void);




/* начальные условия t = 0 */
double analitic(double x, double t){
if ((x + 0.5 * t) <= 0) return 0.0;
if ((x + 0.5 * t) <= 0.25) return 4. * x + 2. * t;
return 1;
}

double myabs(double a){
	if (a > 0) return a;
	return -a;
}


/* собственно моя схема */
double step(double v_last, double v_cur, double v_next){
	double v = 0.0;
	double sp = tau / h;
	v = 0.5 * ((1 + sp) * v_next + (1 - sp) * v_last);
	return v;
} 


double norm_Ch(double *v){
	double max = 0.0;
	double pt = 0.0;
	for (int i = 0; i < DIM; i++){
		pt = myabs(v[i]);
		if (pt > max) max = pt;
	}
	return max;
}

double norm_L(double *v){
	double sum = 0.0;
	for (int i = 0; i < DIM; i++)
		sum += myabs(v[i]);
	return h * sum;
}


double norm_Ch_analitic(double *v, double t){
	double max = 0.0;
	double pt = 0.0;
	for (int i = 0; i < DIM; i++){
		pt = myabs(v[i] - analitic(((2.0/DIM) * i - 1.0),t));
		if (pt > max) max = pt;
	}
	return max;
}

double norm_L_analitic(double *v, double t){
	double sum = 0.0;
	double pt = 0.0;
	for (int i = 0; i < DIM; i++){
		pt = myabs(v[i] - analitic(((2.0/DIM) * i - 1.0),t));
		sum += pt;
	}
	return h * sum;
}
/*

double norm_Ch_compare(double *v, double *u){
	double max = 0.0;
	double pt = 0.0;
	for (int i = 0; i < DIM; i++){
		pt = myabs(v[i] - u[2*i]);
		if (pt > max) max = pt;
	}
	return max;
}

double norm_L_compare(double *v, double *u, double h){
	double sum = 0.0;
	double pt = 0.0;
	for (int i = 0; i < DIM; i++){
		pt = myabs(v[i] - u[2*i]);
		sum += pt;
	}
	return h * sum;
}
*/

/* зададим начальные условия 
 * идём по x: всего DIM точек на расстояние 2, начинаем из -1 c шагом 2/DIM
 * x = (2.0/DIM) * i - 1.0
 */
/* зададим начальные условия 
 * идём по x: всего DIM точек на расстояние 2, начинаем из -1 c шагом 2/DIM
 * x = (2.0/DIM) * i - 1.0
 */
void create_layer(double *u, double t){
	for (int i = 0; i < DIM; i++)
		u[i] = analitic((2.0/DIM) * i - 1.0, t);
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
int print(char dir[100], double *u, int num){
	FILE *f;
	char name[100] = ""; // папка
	char number[100] ; // номер
	char ext[100]  = ".txt"; // расширение
	double x;
 	sprintf(number, "%d", num);
 	strcat(name, dir);
 	strcat(name, number);
 	strcat(name, ext);
	f = fopen(name, "w");
	if (f == NULL)
		return -1;
	for (int i=0;i < DIM; i++){
		x = (2.0/DIM) * i - 1.0;
		fprintf(f, "%e\t%e\n", x,u[i]);
	}
	fclose(f);
	return 0;
}

int print_diff(char dir[100], double *u, int num){
	FILE *f;
	char name[100] = ""; // папка
	char number[100] ; // номер
	char ext[100]  = "ott.txt"; // расширение
	double x;
	double v;
 	sprintf(number, "%d", num);
 	strcat(name, dir);
 	strcat(name, number);
 	strcat(name, ext);
	f = fopen(name, "w");
	if (f == NULL)
		return -1;
	for (int i=0;i < DIM; i++){
		x = (2.0/DIM) * i - 1.0;
		v = analitic((2.0/DIM) * i - 1.0, 1.0);
		fprintf(f, "%e\t%e\n", x,u[i] - v);
	}
	fclose(f);
	return 0;
}

int main(void){
	double *u;
	double *u1;
	double *u2;
	int check;
	double norm1, norm2, norm3, norm4;
	system("rm -r -f calculations1");
	system("mkdir calculations1"); // в папке calculations будут лежать все расчёты
	system("rm -r -f anal");
	system("mkdir anal"); // в папке calculations будут лежать все расчёты

	u = (double*)malloc(DIM*sizeof(double));
	u1 = (double*)malloc(DIM*sizeof(double));
	u2 = (double*)malloc(DIM*sizeof(double));

	create_layer(u,0);

	check = print("calculations1/",u, 0);
	if (check != 0) return -1;

	for (int t = 1; t <= T_max;t++){
		//norm1 = norm_L_analitic(u,t*1.0/T_max);
		//norm2 = norm_Ch_analitic(u,t*1.0/T_max);
		//printf("%d\n",t );
		//printf("n1 = %e \t n2 = %e\n",norm1, norm2 );
		count_new_layer(u,u1,1);
		memcpy(u, u1, DIM*sizeof(double));
		//printf("t*1.0/T_max = %e\n", 1.0 / (T_max));	
	//	create_layer(u2,t*1.0/(T_max));
		//check = print("calculations1/",u1, t);if (check != 0) return -1;
		//check = print("anal/",u2, t);if (check != 0) return -1;
	}
	create_layer(u2,1.0);
	check = print("calculations1/",u1, 1);if (check != 0) return -1;
	check = print("anal/",u2, 1);if (check != 0) return -1;
	check = print_diff("anal/",u1, 2);if (check != 0) return -1;
	norm1 = norm_L_analitic(u,1.0);
	norm2 = norm_Ch_analitic(u,1.0);
	norm3 = norm1/norm_L(u);
	norm4 = norm2/norm_Ch(u);
		//printf("%d\n",t );
	printf(" %e & %e & %e & %e & %e & %e \\\\ \n \\hline \n",tau, h, norm2, norm1, norm4, norm3);
	free(u);
	free(u1);
	free(u2);
	return 0;
}