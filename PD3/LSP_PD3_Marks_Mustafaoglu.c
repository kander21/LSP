#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>


bool found = false; 

char * FindFile(char *filename, char *directory){

	char *path = malloc(256);
	strcat(path, directory);

	DIR *d;
	struct  dirent  *de;
	d  =  opendir(path);
	if(d  !=  NULL){
		while((de = readdir(d)) != NULL){
			printf("Function");
			struct stat buf;
    		stat(path,&buf);

			if(strcmp(de->d_name, filename) == 0 && S_ISREG(buf.st_mode)){
				strcat(path,filename);
				found = true;
				printf("FOUND FILE");
				return path;
			}

			//printf("%s\n", de->d_name);
			if(!S_ISDIR(buf.st_mode)){
				return FindFile(filename, strcat(path,de->d_name));
			}
		}
		closedir(d);
	}
	return path;
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
		char *path = malloc(256);
    	path = FindFile(argv[1], argv[2]);
    	printf("%s\n", path);
		//path = 0;
	}
	return 0;
}

// #include <stdio.h> 
// #include <dirent.h> 
  
// int main(void) 
// { 
//     struct dirent *de;  // Pointer for directory entry 
  
//     // opendir() returns a pointer of DIR type.  
//     DIR *dr = opendir("/usr/bin"); 
  
//     if (dr == NULL)  // opendir returns NULL if couldn't open directory 
//     { 
//         printf("Could not open current directory" ); 
//         return 0; 
//     } 
  
//     // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
//     // for readdir() 
//     while ((de = readdir(dr)) != NULL) 
//             printf("%s\n", de->d_name); 
  
//     closedir(dr);     
//     return 0; 
// } 