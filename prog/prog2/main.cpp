
#include <time.h>
#include "Euler.h"
int main(void){
    unsigned int start, finish;
    start = clock();
    try{
	    Euler();
	}
	catch(int a){
		printf("Couht exception number: %d",a);
	}

	printf("\nFinished calculations. Plotting.\n");
	system("python graphics.py");
    finish = clock();
    printf("Work-time: %e sec.\n", (double)(finish-start)/CLOCKS_PER_SEC);
	return 0;
}