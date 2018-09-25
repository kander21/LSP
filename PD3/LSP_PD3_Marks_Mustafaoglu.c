#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>


bool found = false;
char finalPath[256];

void FindFile(char *filename, char *directory){

	char *path = malloc(256);
	char * tmpPath = malloc(256);
	path[0] = '\0';
	strcat(path, directory);

	DIR *d;
	struct  dirent  *de;
	d = opendir(path);
	if(d != NULL){
		while((de = readdir(d)) != NULL){
            tmpPath[0] = '\0';
            strcat(tmpPath,path);
            strcat(tmpPath,"\\");
            strcat(tmpPath,de->d_name);

			struct stat buf;
    		stat(tmpPath,&buf);

			if(strcmp(de->d_name, filename) == 0 && S_ISREG(buf.st_mode)){
				found = true;
				printf("%s\n", tmpPath);

			}
			if(S_ISDIR(buf.st_mode) && strcmp(de->d_name, ".") && strcmp(de->d_name, "..")){
                FindFile(filename, tmpPath);
			}
		}
		closedir(d);
	}
}

int main(int argc, char **argv) {

    struct stat buf;
    stat(argv[2],&buf);

    if(!S_ISDIR(buf.st_mode) || argc < 3){
    	if(argc < 3){
    		printf("Please provide 2 arguments!");
    	}
    	else{
    		printf("Second argument is not directory!");
    	}
    	return -1;
    }
	else{
    	FindFile(argv[1], argv[2]);
    	if(!found){
            printf("File not found!\n");
    	}
	}
	return 0;
}
