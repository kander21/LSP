#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

int main(void)
{
        printf("allocating...\n");

        // //one chunk
        // void *i1, *i2;
        // i1 = malloc(UINT_MAX*INT_MAX);
        // printf("i1 after 'i1 = malloc(4GB)': %p; sbrk(0): %p\n", i1, sbrk(0));
        // i1 = malloc(UINT_MAX*INT_MAX);
        // printf("i1 after 'i1 = malloc(4GB)': %p; sbrk(0): %p\n", i1, sbrk(0));

        // //many chunks
        // int c;
        // for(c=0;c<100;c){
        // 	i2 = malloc(1024*1024);
        // 	printf("i2 after 'i2 = malloc(1024*1024)': %p; sbrk(0): %p\n",i2, sbrk(0));
        // }

        //one chunk 100MB
        void *i3;
		clock_t begin = clock();
        i3 = malloc(1024*1024*100); //100MB
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("i1 after 'i1 = malloc(100MB)': %p; sbrk(0): %p; time: %f\n", i3, sbrk(0), time_spent);
        printf("done\n");

        //many chunks 100MB
        int c;
		clock_t begin2 = clock();
        for(c=0;c<100;c++){
        	i3 = malloc(1024*1024);
        	printf("i3 after 'i3 = malloc(1MB)': %p; sbrk(0): %p\n",i3, sbrk(0));
        }
        clock_t end2 = clock();
        double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
        printf("time: %f\n", time_spent2);
        return 0;
}