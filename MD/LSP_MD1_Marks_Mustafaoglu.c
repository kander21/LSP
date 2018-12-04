#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/*************COMPILATION***************

gcc -std=c90 LSP_MD1_Marks_Mustafaoglu.c
./a.out stdin < input.in

***************************************/

typedef struct infoNode{
    struct infoNode *prev;
    char name[63];
    char mother[63];
    char father[63];
    struct infoNode *next;
} infoNode;

typedef struct threeNode{
    char name[63];
    struct threeNode *next;
    struct threeNode *prev;
} threeNode;

typedef struct youngestNode{
    char name[63];
    char mother[63];
    char father[63];
    struct youngestNode *next;
    struct threeNode *familyThree;
    struct threeNode *lastThreeNode;
} youngestNode;

bool AlreadyContain(threeNode *tcurr, char *name){ /*Parbauda vai tada persona jau eksiste gimenes zaraa*/
    for(tcurr; tcurr != NULL; tcurr = tcurr->next){
        if(strcmp(tcurr->name, name) == 0)return true;
    }
    return false;
}

bool IsInvalidInput(infoNode *first){ /*Parbauda vai nav invalidi dati, piem, A ir tevs B un B ir tevs A*/
    infoNode *curr;
    infoNode *curr2;
    for(curr = first; curr != NULL; curr = curr->next){
        for(curr2 = first; curr2 != NULL; curr2 = curr2->next){
            if((strcmp(curr2->name, curr->father) == 0 || strcmp(curr2->name, curr->mother) == 0)
                && (strcmp(curr2->father, curr->name) == 0 || strcmp(curr2->mother, curr->name) == 0)){
                fprintf(stderr, "Invalid input: %s is parent of %s and %s is parent of %s", curr2->name, curr->name, curr->name, curr2->name);
                return true;
            }
        }
    }
    return false;
}

void FillFamilies(infoNode *first, youngestNode *yfirst){ /*Sapilda visas ciltis ar jaunaako personu vecakiem, vecvecakiem utt.*/
    infoNode *curr = NULL;
    threeNode *tcurr = NULL;
    threeNode *tcurr2 = NULL;
    youngestNode *ycurr = NULL;

    for(ycurr = yfirst; ycurr != NULL; ycurr = ycurr->next){ /*Cikls, kas iet caur jaunako personu sarakstu jeb cilšu sarakstu*/
        if(ycurr->familyThree == NULL){ /*Uzreiz pievieno jaunakas personas vecakus cilts zaram, jo tos nevajag meklet, tie jau ir zinami - ari glabajas pie jaunakas personas*/
            if(strlen(ycurr->father) > 1){
                tcurr = malloc(sizeof(threeNode));
                tcurr->next = NULL;
                tcurr->prev = NULL;
                ycurr->familyThree = tcurr;
                ycurr->lastThreeNode = tcurr;
                strcpy(tcurr->name, ycurr->father);
            }
            if(strlen(ycurr->mother) > 1){
                tcurr = malloc(sizeof(threeNode));
                tcurr->next = NULL;
                if(ycurr->familyThree == NULL){
                    tcurr->prev = NULL;
                    ycurr->familyThree = tcurr;
                    ycurr->lastThreeNode = tcurr;
                }
                else{
                    tcurr->prev = ycurr->lastThreeNode;
                    ycurr->lastThreeNode->next = tcurr;
                    ycurr->lastThreeNode = tcurr;
                }
                strcpy(tcurr->name, ycurr->mother);
            }
        }

        for(tcurr2 = ycurr->familyThree; tcurr2 != NULL; tcurr2 = tcurr2->next){ /*Cikls, kas aizpilda parejos cilts zara locekļus*/
            for(curr = first; curr != NULL; curr = curr->next){ /*Meklē personas informācijas sarakstā, lai izdabūt to vecākus*/
                if(strcmp(tcurr2->name, curr->name) == 0){
                    if(strlen(curr->mother) > 1){ /*Pievieno personas mati sarakstam*/
                        if(!AlreadyContain(ycurr->familyThree, curr->mother)){
                            tcurr = malloc(sizeof(threeNode));
                            tcurr->next = NULL;
                            tcurr->prev = ycurr->lastThreeNode;
                            ycurr->lastThreeNode->next = tcurr;
                            ycurr->lastThreeNode = tcurr;
                            strcpy(tcurr->name, curr->mother);
                        }
                    }
                    if(strlen(curr->father) > 1){  /*Pievieno personas tevu sarakstam*/
                        if(!AlreadyContain(ycurr->familyThree, curr->father)){
                            tcurr = malloc(sizeof(threeNode));
                            tcurr->next = NULL;
                            tcurr->prev = ycurr->lastThreeNode;
                            ycurr->lastThreeNode->next = tcurr;
                            ycurr->lastThreeNode = tcurr;
                            strcpy(tcurr->name, curr->father);
                        }
                    }
                }
            }
        }
    }

    /* Izvada datus terminali - iziet cauru visam ciltim un katras cilts zaru izvada sākot no beigām*/
    for(ycurr = yfirst; ycurr != NULL; ycurr = ycurr->next){
        for(tcurr = ycurr->lastThreeNode; tcurr != NULL; tcurr = tcurr->prev){
            fprintf(stdout, "%s\n", tcurr->name);
        }
        fprintf(stdout, "%s\n", ycurr->name);
        if(ycurr->next != NULL)fprintf(stdout, "\n");
    }
}


void FindYoungestPersons(infoNode *first){ /*Izveido saistitu sarakstu no katras cilts visjaunakajam personam. Katra node ir viena cilts*/
    infoNode *curr = NULL;
    infoNode *curr2= NULL;
    youngestNode *ycurr = NULL;
    youngestNode *yfirst = NULL;
    youngestNode *ylast = NULL;
    youngestNode *ycurr2 = NULL;
    youngestNode *yBrotherOrSister = NULL;
    threeNode *tcurr = NULL;
    char person[63];
    bool isYoungest;
    bool hasBrotherOrSister;

    for(curr = first; curr != NULL; curr = curr->next){
        strcpy(person, curr->name);
        isYoungest = true;
        hasBrotherOrSister = false;
        yBrotherOrSister = NULL;
        for(curr2 = first; curr2 != NULL; curr2 = curr2->next){
            if(strcmp(curr2->father, person) == 0
                || strcmp(curr2->mother, person) == 0){
                isYoungest = false;
                break;
            }
        }

        if(isYoungest){ /*Parbauda vai jaunakajam ir bralis vai masa. Vajadzigs, lai neveidotu atsevisku cilti*/
            for(ycurr2 = yfirst; ycurr2 != NULL; ycurr2 = ycurr2->next){
                if(strcmp(ycurr2->father, curr->father) == 0
                    || strcmp(ycurr2->mother, curr->mother) == 0){
                    yBrotherOrSister = ycurr2;
                    hasBrotherOrSister = true;
                }
            }
        }

        if(isYoungest && hasBrotherOrSister){ /*Ja jaunakajai personai ir bralis vai masa, tad to pievieno uzreiz jaunakas personas cilts zaraa*/
            tcurr = malloc(sizeof(threeNode));
            tcurr->next = NULL;
            if(yBrotherOrSister->familyThree == NULL){
                tcurr->prev = NULL;
                yBrotherOrSister->familyThree = tcurr;
                yBrotherOrSister->lastThreeNode = tcurr;
            }
            else{
                yBrotherOrSister->lastThreeNode->next = tcurr;
                tcurr->prev = yBrotherOrSister->lastThreeNode;
                yBrotherOrSister->lastThreeNode = tcurr;
            }
            strcpy(tcurr->name, curr->name);
            continue;
        }

        if(isYoungest && !hasBrotherOrSister){ /*Ja jaunakai personai nav bralis vai masa, nozime ka ta ir vieniga jaunaka persona ciltii. Pievieno jaunako personu sarakstam*/
            ycurr = malloc(sizeof(youngestNode));
            ycurr->familyThree = NULL;
            ycurr->lastThreeNode = NULL;
            ycurr->next = NULL;
            if(yfirst == NULL){
                yfirst = ycurr;
                ylast = ycurr;
            }
            else{
                ylast->next = ycurr;
                ylast = ycurr;
            }
            strcpy(ycurr->name, curr->name);
            strcpy(ycurr->father, curr->father);
            strcpy(ycurr->mother, curr->mother);
        }
    }
    FillFamilies(first, yfirst);
}

struct infoNode * LoadData(){ /*Nolasa datus no faila parastaja saistitaja saraksta*/
    infoNode *curr = NULL;
    infoNode *curr2 = NULL;
    infoNode *first = NULL;
    infoNode *last = NULL;
    char line[256];
    char personType[10];

    fgets(line, sizeof(line), stdin);
    while(!feof(stdin)) {
        if(!isspace(line[0])){
            curr = malloc(sizeof(infoNode));
            curr->next = NULL;
            curr->mother[0] = '\0';
            curr->father[0] = '\0';
            if(first == NULL){
                first = curr;
                last = curr;
                curr->prev = NULL;
            }
            else{
                curr->prev = last;
                last->next = curr;
                last = last->next;
            }

            sscanf(line,"%*s %s",curr->name);
            personType[0] = '\0';
            fgets(line, sizeof(line), stdin);
            sscanf(line,"%s",personType);
            if(strcmp(personType, "VARDS") == 0)continue;

            while(strcmp(personType, "VARDS") != 0){
                sscanf(line,"%s",personType);
                if(strcmp(personType, "MATE") == 0){
                    sscanf(line,"%*s %s",curr->mother);
                    personType[0] = '\0';
                }
                if(strcmp(personType, "TEVS") == 0){
                    sscanf(line,"%*s %s",curr->father);
                    personType[0] = '\0';
                }
                line[0] = '\0';
                fgets(line, sizeof(line), stdin);
                if(isspace(line[0])){
                    fgets(line, sizeof(line), stdin);
                    sscanf(line,"%s",personType);
                    if(strcmp(personType, "VARDS") == 0)break;
                    continue;
                }
                sscanf(line,"%s",personType);
                if(feof(stdin) && line[0] == '\0')break;
            }
        }
        else{
            fgets(line, sizeof(line), stdin);
        }
    }
    return first;
}



int main()
{
    struct infoNode * pt = LoadData();
    if(IsInvalidInput(pt))return -1;
    FindYoungestPersons(pt);
    return 0;
}
