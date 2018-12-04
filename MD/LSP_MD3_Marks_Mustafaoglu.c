#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


/* Īss apraksts - veido sarakstu no visiem failiem kurus atrod,
katram failam kas ir sarakstā ir pointeris uz līdzīgu sarakstu, kur glabājas duplikāti.
Kā rezultātu izvada tikai tos failus, kuriem duplikātu pointeris nav NULL. Programmā implementēta
tikai duplikātu meklēšana pēc faila tipa un nosaukuma + modifikācijas datuma, kā arī rakstīšana log failā*/

typedef struct fileNode{
    char filePath[255];
    char fileName[100];
    long long size;
    time_t time;
    struct fileNode *next;
    struct duplicateNode *duplicates;
} fileNode;

typedef struct duplicateNode{
    char filePath[255];
    char fileName[100];
    long long size;
    time_t time;
    struct duplicateNode *next;
} duplicateNode;

fileNode *first = NULL;
bool noParams;
bool dIsEnabled;
bool logIsEnabled;

char * GetDate(time_t rawDate){  /*Iegūst datumu pareizā formātā. Beigās nebija vajadzīga, jo netiku līdz detalizētas informācijas izvadīšanai lietotājam*/
    char *timeBuff = malloc(50);
    time_t time = rawDate;
    struct tm conv;
    conv = *localtime(&time);
    strftime(timeBuff, 50, "%Y-%m-%d %H:%M", &conv);
    return timeBuff;
}

fileNode * IsFileInList(char *fileName, long long size, time_t time, fileNode *foundInList){ /*Pārbauda vai fails jau ir failu sarakstā*/
    fileNode *curr;
    for(curr = first; curr != NULL; curr = curr->next){
        if(noParams){ /*Defaultā pārbaude*/
            if(strcmp(curr->fileName, fileName) == 0 && curr->size == size){
                foundInList = curr;
                return foundInList;
            }
        }
        if(dIsEnabled){ /*Defaultā pārbaude + datums*/
            if(strcmp(curr->fileName, fileName) == 0 && curr->size == size && curr->time == time){
                foundInList = curr;
                return foundInList;
            }
        }
    }
    foundInList = NULL;
    return foundInList;
}

void FindFile(char *directory){ /*Meklē failus un aizpilda datu struktūru*/

	char *path = malloc(256);
	char * tmpPath = malloc(256);
	path[0] = '\0';
	strcat(path, directory);

	fileNode *ftmp = NULL;
	duplicateNode *dtmp = NULL;
	fileNode *foundInList = NULL;

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

			if(S_ISREG(buf.st_mode)){

                foundInList = IsFileInList(de->d_name, buf.st_size, buf.st_mtime, foundInList);
                if(foundInList != NULL){ /*Pārbauda vai fails ir failu sarakstā*/
                    dtmp = malloc(sizeof(fileNode));
                    if(foundInList->duplicates == NULL){
                        dtmp->next = NULL;
                    }
                    else{
                        dtmp->next = foundInList->duplicates;
                    }
                    foundInList->duplicates = dtmp;
                    strcpy(dtmp->filePath, tmpPath);
                    strcpy(dtmp->fileName, de->d_name);
                    memmove(dtmp->filePath, dtmp->filePath+2, strlen(dtmp->filePath));
                    dtmp->size = buf.st_size;
                    dtmp->time = buf.st_mtime;
                }
                else{
                    ftmp = malloc(sizeof(fileNode));
                    ftmp->duplicates = NULL;
                    if(first == NULL){
                        first = ftmp;
                        ftmp->next = NULL;
                    }
                    else{
                        ftmp->next = first;
                        first = ftmp;
                    }
                    strcpy(ftmp->filePath, tmpPath);
                    strcpy(ftmp->fileName, de->d_name);
                    memmove(ftmp->filePath, ftmp->filePath+2, strlen(ftmp->filePath));
                    ftmp->size = buf.st_size;
                    ftmp->time = buf.st_mtime;
                }
			}
			if(S_ISDIR(buf.st_mode) && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0){
                FindFile(tmpPath);
			}
		}
		closedir(d);
	}
}

int main(int argc, char **argv) {

    noParams = false;
    dIsEnabled = false;
    logIsEnabled = false;

    if(argc > 1){
        if(strcmp(argv[argc - 2], ">") == 0){
            logIsEnabled = true;
        }
        if(strcmp(argv[2], "-d") == 0){
            dIsEnabled = true;
        }
        if(strcmp(argv[2], "-h") == 0){
            printf("Welcome to Help\n\n");
            printf("Information about parameters:\n\n");
            printf("No params => default search (by file name and size).\n");
            printf("-d => Find duplicates by last modified date + default conditions.\n");
            printf("> fileName => Write output to file \"filename\".\n");
            printf("-h => Show help menu.\n");
            return 0;

        }
    }
    else{
        noParams = true;
    }

    FindFile(".");

    fileNode *curr = NULL;
    duplicateNode *dcurr = NULL;
    bool foundDuplicates = false;

    if(logIsEnabled){

        FILE *output;
        output = fopen(argv[argc-1], "w");
        if(output != NULL){
            for(curr = first; curr != NULL; curr = curr->next){
                dcurr = NULL;
                if(curr->duplicates != NULL){
                    foundDuplicates = true;
                    fwrite(curr->filePath, 1, sizeof(curr->filePath), output);
                    fwrite("\n", sizeof(char), 1, output);
                    for(dcurr = curr->duplicates; dcurr != NULL; dcurr = dcurr->next){
                        fwrite(dcurr->filePath , 1 , sizeof(dcurr->filePath), output);
                        fwrite("\n", sizeof(char), 1, output);
                    }
                }
            }
            if(foundDuplicates == false){
                char str[] = "No duplicates found!";
                fwrite(str , 1 , sizeof(str)-1 , output);
            }
        }
        fclose(output);
    }
    else{
    foundDuplicates = false;
        for(curr = first; curr != NULL; curr = curr->next){
                dcurr = NULL;
                if(curr->duplicates != NULL){
                    foundDuplicates = true;
                    printf("%s\n", curr->filePath);
                    for(dcurr = curr->duplicates; dcurr != NULL; dcurr = dcurr->next){
                        printf("%s\n", dcurr->filePath);
                    }
                }
            }
            if(foundDuplicates == false){
                printf("No duplicates found!");
            }
    }
    return 0;
}
