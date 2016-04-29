/*
 * Неявный метод Эйлера для решения системы Диффуров 
 */

#include "Euler.h"
 void Euler(){

    double x[DIM];
    double old[DIM];
	double (*f[DIM])(double*, double*);
	FILE *file;
	file = fopen("out.csv", "w");
	Create_Initials(x, f);

	for (double t = 0; t < TIME_LIMIT; t+= h){
		for (int i=0; i< DIM; i++) old[i] = x[i];
		Newton(x,old,f);
		fprintf(file, "%e \t ", t);
		fprintf(file, "%e \t ", x[0]);
		fprintf(file, "%e \t ", x[2]);
		fprintf(file,"\n");
	}
	fclose(file);
	
}

 void Create_Initials(double x[DIM], double (*f[DIM])(double*, double*)){
	f[0] = f1;
	f[1] = f2;
	f[2] = f3;
	f[3] = f4;

	x[0] = 1;
	x[1] = 0;
	x[2] = 0;
	x[3] = 1;

}
/*
void Create_New_Vector_Functions((*f[DIM])(double*),(*g[DIM])(double*, double*)){
	for (i=0; i < DIM; i++) g[i](x,y) = f[i](x) + f[i](y);
}
*/