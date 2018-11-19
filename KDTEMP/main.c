#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <time.h>



#define BADKEY -1
#define A1 1
#define A2 2
#define B1 3
#define B2 4
#define NKEYS (sizeof(lookupTable)/sizeof(paramKeyStruct))
typedef struct { char *key; int val; } paramKeyStruct;

static paramKeyStruct lookupTable[] = {
        { "A1", A1 }, { "A2", A2 }, { "B1", B1 }, { "B2", B2 }
};

int valueFromString(char *key)
{
    int i;
    for (i=0; i < NKEYS; i++) {
        paramKeyStruct *sym = &lookupTable[i];
        if (strcmp(sym->key, key) == 0)
            return sym->val;
    }
    return BADKEY;
}

int main(int argc, char **argv) {

    if(argc <= 0){
        printf("No parameters were specified!\n");
        return 0;
    }

    int i;
    for(i=1;i<argc;i++){

        switch (valueFromString(argv[i])){
            case A1:
                printf("Entered param A1\n");
                break;
            case A2:
                printf("Entered param A2\n");
                break;
            case B1:
                printf("Entered param B1\n");
                break;
            case B2:
                printf("Entered param B2\n");
                break;
            case BADKEY:
                printf("Invalid parameter entered\n");
                break;
        }
    }

    return 0;
}
