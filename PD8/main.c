#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MY_BUFFER_SIZE 4096

unsigned char mybuffer[MY_BUFFER_SIZE];
int blockCount = 0;
int freeSpace = MY_BUFFER_SIZE;
int last = 0;

void * myalloc(size_t size){

    int intSize = (int)size;
	if(intSize >= MY_BUFFER_SIZE){
		return NULL;
	}
	//Initial myAlloc
    void * pos;
    if(freeSpace == MY_BUFFER_SIZE){
        last = 5;
        freeSpace = MY_BUFFER_SIZE - last;
        *((int *)(mybuffer)) = freeSpace;
        pos = mybuffer + last;
        blockCount++;
        *((char *)(pos-1)) = '0'; //This block is free
	}

	int block;
    int posControl = last;
    pos = mybuffer + last;
	void * mem;
	bool findFromStart = false;
	for(block = 1; block <= blockCount; block++){
        int blockSize = *((int *)(pos-5));
		if(*((char *)(pos-1)) == '0' && (blockSize == size || blockSize > size+5)){
		    if(blockSize > size+5){ //Requested size is smaller than free block size
                *((char *)(pos-1)) = '1';
                *((int *)(pos-5)) = intSize;
                mem = pos;
                pos = pos + intSize + 5;
                *((int *)(pos-5)) = blockSize-intSize-5;
                *((char *)(pos-1)) = '0';
                last = posControl + intSize + 5;
                freeSpace = freeSpace - intSize + 5;
		    }
		    else{ //Requested size is equal with free block size
                *((char *)(pos-1)) = '1';
		        mem = pos;
                freeSpace = freeSpace - intSize;
		    }
            blockCount++;
		    printf("Allocated memory for %d bytes at %p\n", intSize, mem);
            return mem;
		}
		posControl = posControl + blockSize + 5;
		pos = pos + blockSize + 5;
		if(last + intSize >= MY_BUFFER_SIZE && findFromStart == false) {
		    pos = mybuffer + 5;
            posControl = 5;
            findFromStart = true;
		}

	}
	printf("Unable to allocate memory for size: %d\n", intSize);
	return NULL;
}

int myfree(void * ptr){
    if((char *)ptr >= (char *)mybuffer && (char *)ptr <= (char *)mybuffer + MY_BUFFER_SIZE){
        *((char *)(ptr-1)) = '0';
        freeSpace = freeSpace + *((int *)(ptr-5));
        printf("Released %d bytes of memory at %p\n", *((int *)(ptr-5)), ptr);
        return 1;
    }
    return -1;
}

int main(void){

    void *p1, *p2, *p3, *p4, *p5;
    p1 = myalloc(100);
//    int size = *((int *)(m-5));
//    printf("Size %d, address: %p\n", size, m);
    p2 = myalloc(2500);
//    int size1 = *((int *)(m1-5));
//    printf("Size %d, address: %p\n", size1, m1);
    p3 = myalloc(70);
    p4 = myalloc(978);

    myfree(p3);
    myfree(p4);
    myalloc(900);
    myalloc(50);

	int blockNum;
	int pos = 5;
	for(blockNum = 1;blockNum<=blockCount;blockNum++){
	    if(mybuffer[pos-1] == '1'){}
	    printf("Block %d: status %c, size %d\n", blockNum, mybuffer[pos-1], *((int *)(&mybuffer[pos-5])));
        pos = pos+*((int *)(&mybuffer[pos-5]))+5;
	}
	return 0;
}
