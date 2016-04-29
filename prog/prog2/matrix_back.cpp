/*
 *  Нахождение обратной матрицы
 */
#include "matrix.h"

/* прямой ход метода Гаусса */
void matrix::Gauss(){
    int min;
    if (str < sto) min = str;
    else min = sto;

    for (int i=0;i < min;i++){ 
        if (matr[i][i] == 0)
            for (int j=i; j< min; j++)
                if (matr[j][i] != 0) swap(i,j);
        Del(i,matr[i][i]);
        for (int j = i+1;j<str;j++) Minus(j,i,matr[j][i]);
    }
}

/* обратный ход метода Гаусса */
void matrix::Back_Gauss(){
    int min;
    int errflag = 0;
    if (str < sto) min = str;
    else min = sto;

    for (int i=0;i < min;i++) 
        if (matr[i][i] != 1) errflag = 1; 

    if (errflag == 1) printf("ERROR! Not diag!\n");
    else{ 
        for (int i=min-1;i >= 0;i--) 
            for (int j=i-1; j >= 0; j--)
                Minus(j,i,matr[j][i]);
    }
}

/* Добавление к матрицы единичную того же размера справа */
matrix matrix::Double_Matrix(){
    matrix A(str, sto*2);
    for (int i=0;i<str;i++)
       for (int j=0;j<sto;j++)
            A.matr[i][j] = matr[i][j];
    
    for (int i=sto; i< A.sto; i++)
        A.matr[i-sto][i] = 1;
    return A;
}
/* удаление левой единичной половины */
matrix matrix::Single_Matrix(){
    matrix A(str, sto/2);
    for (int i=0;i<A.str;i++)
       for (int j=0;j<A.sto;j++)
            A.matr[i][j] = matr[i][j+A.sto];

    return A;
}

/* Нахождение обратной матрицы */
matrix matrix:: Back(){
    matrix A(0);
    if (det() == 0) {
        printf("This Matrix doesn't have an Inverse.\n" );
        return A;
    }
    A = Double_Matrix();
    A.Gauss();
    A.Back_Gauss();
    A = A.Single_Matrix();
    return A;
}
