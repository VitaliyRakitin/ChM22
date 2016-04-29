#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include<unistd.h>
#define AESH_MIN 1e-15
/* Решение системы линейных уравнений с трёхдиагональной матрицей.
 * n --- размерность
 * dest --- куда положить решение
 * a --- главная диагональ (a[0] на первой строке, a[n-1] на последней)
 * с --- диагональ выше главной (c[0] на первой строке, с[n-2] на предпоследней,
 * 	 на последней строке такого элемента вообще нет)
 * d --- диагональ ниже главной (d[0] на второй строке, d[n-2] на последней строке,
 * 	 на первой строке такого элементе нет
 * b --- вектор правых частей (b[0]..b[n-1])
 */



double analitic(double x, double t);
double myabs(double a);
double norm_Ch(double *v,double DIM);
double norm_L(double *v, double h, double DIM);
double norm_Ch_analitic(double *v, double t, double DIM);
double norm_L_analitic(double *v, double t, double h,double DIM);
int thomas_a(int n,double *dest,double*a,double*be,double*ga,double *b);
void *lin(void* mn);
int show(FILE *f,double *v,int M,double t);
double norm_c(double *v,int dim);
double A(double *v, int m, int n, double h, double tau, double w);
double B(double *v, int m, double h, double tau, double w);
double C(double *v, int m, double h, double tau, double w);
double G1(double*v,double*hat_v,int m, double h, double tau, double omega);
void *notlin(void* mn);
int main(int argc,char **argv);

/* начальные условия t = 0 */
 /*
double phi2(double t, double x){
  if (2 * x < t + 0.25) return 0.;
  else return 1.;
}

double phi1(double t, double x){
  if (x<=0.0) return 0.;
  if (x-t>0.25) return 1.;
  return 4*x/(1+4*t);
}

double analitic(double x,double t){
  if (t<0.25) return phi1(-t,x);
  else return phi2(-t,x);
}

*/
double analitic(double x, double t){
if ((x + 0.5 * t) <= 0) return 0.0;
if ((x + 0.5 * t) <= 0.25) return 4. * x + 2. * t;
return 1;
}

double myabs(double a){
  if (a > 0) return a;
  return -a;
}


double norm_Ch(double *v,double DIM){
  double max = 0.0;
  double pt = 0.0;
  for (int i = 0; i < DIM; i++){
    pt = myabs(v[i]);
    if (pt > max) max = pt;
  }
  return max;
}

double norm_L(double *v, double h, double DIM){
  double sum = 0.0;
  for (int i = 0; i < DIM; i++)
    sum += myabs(v[i]);
  return h * sum;
}


double norm_Ch_analitic(double *v, double t, double DIM){
  double max = 0.0;
  double pt = 0.0;
  for (int i = 0; i < DIM; i++){
    pt = myabs(v[i] - analitic(((2.0/DIM) * i - 1.0),t));
    if (pt > max) max = pt;
  }
  return max;
}

double norm_L_analitic(double *v, double t, double h,double DIM){
  double sum = 0.0;
  double pt = 0.0;
  for (int i = 0; i < DIM; i++){
    pt = myabs(v[i] - analitic(((2.0/DIM) * i - 1.0),t));
    sum += pt;
  }
  return h * sum;
}




int thomas_a(int n,double *dest,double*a,double*be,double*ga,double *b){
  double *A,*B; int i;
  if (n<2) return n-2;
  A = (double*)malloc(2*n*sizeof(double));
  B = A + n;
  A[1] = -be[0]/a[0]; B[1] = b[0]/a[0];
  for (i=1;i<n-1;i++) {
    A[i+1] = -be[i]/(ga[i-1]*A[i]+a[i]);
    B[i+1] = (b[i]-ga[i-1]*B[i])/(ga[i-1]*A[i]+a[i]);
  }
  dest[i] = (b[i]-ga[i-1]*B[i])/(A[i]*ga[i-1] + a[i]);
  if (dest[i] < AESH_MIN) dest[i] = 0.0;
  for(i=n-1;i>0;i--) 
    { dest[i-1] = A[i]*dest[i] + B[i];
      if (dest[i-1] < AESH_MIN) dest[i-1] = 0.0;
    }
  free(A);
  return 0;
}

int show(FILE *f,double *v,int M,double t){
  int m,d; double h; h = 2./M;
  if (M<5000) for (m=0;m<=M; m++) fprintf(f,"%e\t%e\n",-1 + m*h,v[m]);
  else for (m=0, d=M/5000; m<M; m+= d) fprintf(f,"%e\t%e\n",-1 + m*h,v[m]);
  return d;
}

void *lin(void* mn){
  double *v,*a,*b,*c,*d,h,tau,kappa;
  double A,B,C;
  double w = 0.1;
  FILE *f;
  double norm1, norm2, norm3, norm4;
  int M,N,m,n;
  M = ((int*)mn)[0]; 
  h = 2./M;
  N = ((int*)mn)[1]; 
  tau = 1./N; 
  A = -2 * h * h/(2 * h * h + w * tau * tau);
  B = - tau *(w * tau - h)/(4 * h * h + 2 * w * tau * tau);
  C = - tau *(w * tau + h)/(4 * h * h + 2 * w * tau * tau);
//  kappa = tau/(4*h);
  if (stop==1) return mn;
  v = (double*)malloc((5*M-5)*sizeof(double));
  /* a --- главная диагональ матрица, M-1 единица
   * b --- правая часть системы. Копия старого слоя 
   * c --- диагональ выше главной; d --- ниже
   */
  a = v+(M+1); 
  b = a + (M-1); 
  c = b+(M-1); 
  d = c +(M-2);

  /* начальные условия */
  for (m=0;m<=M;m++){ 
    if (stop==1) {free(v); return mn;}
    if (-1 + m * h <0) v[m] = 0;
    else if (m*h-1 < 0.25) v[m] = 4*(-1 + m * h);
    else v[m]=1;
  }

  /* Пересчиваем слои */
  for(n=1;n<=N;n++){// layer
    if (stop==1) {free(v); return mn;} // stopping
    for(m=0;m<M-2;m++){// заполнение векторов для прогонки
      a[m]=1; 
      d[m] = -C; 
      c[m] = -B; 
      b[m] = v[m+1];
    }
    a[M-2] = 1; 
    b[M-2] = v[M-1] - C;
    thomas_a(M-1,v+1,a,c,d,b);
  }
  f = fopen("l1.csv","w");
  
  show(f,v,M,(n-1)*tau);
  fclose(f);

  norm1 = norm_L_analitic(v,1.0,h,M);
  norm2 = norm_Ch_analitic(v,1.0,M);
  norm3 = norm1/norm_L(v,h,M);
  norm4 = norm2/norm_Ch(v,M);
    //printf("%d\n",t );
  printf(" %e & %e & %e & %e & %e & %e \\\\ \n \\hline \n",tau, h, norm2, norm1, norm4, norm3);

  free(v); finished++;
  printf("Finished.\n");
  return mn;
}


double norm_c(double *v,int dim){
  double max=0; int m;
  for(m=0;m<dim;m++) max = fmax(fabs(v[m]),max);
  return max;
}


double A(double *v, int m, int n, double h, double tau, double w){
  double up, down;
  if (n < 0) return 0.0;
  up = 4 * w * tau * tau * v[n] * v[n];
  down = 4 * h * h + 4 * w * tau * tau * (v[m] * v[m] + v[m-1] * v[m-1]); 
  up = up/down;
 // if (up < AESH_MIN) return 0;
  return up;
}

double B(double *v, int m, double h, double tau, double w){
  double up, down;
  up = tau * h;
  down = 4 * h * h + 4 * w * tau * tau * (v[m] * v[m] + v[m-1] * v[m-1]); 
  up = up/down;
  //if (up < AESH_MIN) return 0;
  return up;
}

double C(double *v, int m, double h, double tau, double w){
  double up, down;
  up = - h * h;
  down =  h * h + w * tau * tau * (v[m] * v[m] + v[m-1] * v[m-1]); 
  up = up/down;
 // if (up < AESH_MIN) return 0;
  return up;
}

double G1(double*v,double*hat_v,int m, double h, double tau, double omega){
  double Cl = C(v,m,h,tau,omega);
  double Al =  A(v,m,m-1,h,tau,omega);
  double Ap =  A(v,m,m,h,tau,omega);
  double Bl = B(v,m,h,tau,omega);
   return Cl * v[m] + (Al * hat_v[m-1]  + Bl * hat_v[m-1] *hat_v[m-1]) + hat_v[m] - (Ap * hat_v[m+1]  + Bl * hat_v[m-1] *hat_v[m+1]);
}

void *notlin(void* mn){
  double *v,*hat_v,*w,*a,*b,*c,*d,h,tau,kappa,err,eps=1e-5;
  double omega = 0.1;
  double norm1, norm2, norm3, norm4;
  FILE *f;
  int M,N,m,n;
  M = ((int*)mn)[0]; h = 2./M;
  N = ((int*)mn)[1]; tau = 1./N; kappa = tau/(2*h);
  if (stop==1) return mn;
  v = (double*)malloc((7*M-5)*sizeof(double));
  /* a --- главная диагональ матрица, M-1 единица
  *  b --- правая часть системы. Копия старого слоя 
  *  c --- диагональ выше главной; d --- ниже
  */
  hat_v = v+(M+1); 
  w = hat_v+(M+1); 
  a = w+(M-1);
  b = a + (M-1); 
  c = b+(M-1); 
  d = c +(M-2);

  /* начальные условия */
  for (m=0;m<=M;m++){ //начальные условия
    if (stop==1) {free(v); return mn;}
    if (-1+m*h <0) v[m] = 0;
    else if (m*h-1 < 0.25) v[m] = 4*(-1+m*h);
    else v[m]=1;
  }

  for(n=1;n<=N;n++){// layer
    if (stop==1) {free(v); return mn;} // stopping
    for (m=0;m<=M;m++) hat_v[m] = v[m]; // сначала в новый слой копируем старый
    for (err =100; err>eps; ){
      for(m=0;m<M-2;m++){// заполнение векторов для прогонки
        a[m]=1; 
        d[m] = -(A(v,m,m-1,h,tau,omega) + 2 * B(v,m,h,tau,omega)) ; 
        c[m] = -(A(v,m,m,h,tau,omega) - 2 * B(v,m,h,tau,omega)) ;
       // if (d[m] != - c[m]) printf("%e %e\n", d[m], c[m]);
        b[m] = -G1(v,hat_v,m+1,h,tau,omega);
      }
      a[M-2] = 1; 
      b[m] = -G1(v,hat_v,m+1,h,tau,omega);// (A(v,m,m,h,tau,omega) + 2 * B(v,m,h,tau,omega)) ;

      err = norm_c(b,M-1); // когда правая часть уравнений посчитана, можно и погрешность 
			  //оценить на текущей итерации
      if (err<eps) break; // И если она сильно хорошая, выйти
      thomas_a(M-1,w,a,c,d,b);
      for(m=1;m<M;m++) hat_v[m]+=w[m-1];// собственно попытка расчёта следующего слоя
       
    }
    for(m=0;m<=M;m++) v[m] = hat_v[m];// новый слой кладём на место старого, теперь он старый
  }
  f = fopen("nl1.csv","w");
  show(f,v,M,(n-1)*tau);
  fclose(f);

  norm1 = norm_L_analitic(v,1.0,h,M);
  norm2 = norm_Ch_analitic(v,1.0,M);
  norm3 = norm1/norm_L(v,h,M);
  norm4 = norm2/norm_Ch(v,M);
    //printf("%d\n",t );
  printf(" %e & %e & %e & %e & %e & %e \\\\ \n \\hline \n",tau, h, norm2, norm1, norm4, norm3);


  free(v); finished++;
  printf("Нелинейная 1 Finished.\n");

  return mn;
}

int main(int argc,char **argv){
  int N,M,mn[2],ans;
  if (argc<3) { printf("%s M N\n",argv[0]); return 0;}
  mn[0]=M = atoi(argv[1]); mn[1]=N = atoi(argv[2]);
  finished = stop = 0;
  lin(mn);
 // notlin(mn);

  return 0;
}
