#include <stdio.h>
#include <stdbool.h>

int sv_garums(char *virkne)
{
	int len = 0;
	for(int i = 0; virkne[i] != '\0'; i++)
	{
		len += 1;
	}
	return len;
}
 
void sv_kopet(char *no, char *uz)
{
	int j;
	for(j = 0; no[j] != '\0'; j++)
	{
		uz[j] = no[j];
	}
	uz[j] = '\0';
}

int sv_meklet(char *kur, char *ko)
{
	int num = 0;
	bool found = false;
	for(int i = 0; kur[i] != '\0'; i++)
	{
		if(kur[i] == ko[0])
		{
			for(int j = 0; ko[j] != '\0'; j++)
			{
				if(kur[i] != ko[j]) break;
				if(ko[j+1] == '\0')found = true;
				else i++;
			}
			i = i-1;	
		}
		if(found == false)num += 1;
		if(found == true)return num;
	}
	return -1;
}

void sv_apgriezt(char *virkne)
{
	char tmpChar;
	int len = sv_garums(virkne);
	
		int tmplen = len/2;
		for(int i = 0; i < tmplen; i++)
		{
			tmpChar = virkne[i];
			virkne[i] = virkne[len-i-1];
			virkne[len-i-1] = tmpChar; 	
		}

}


int main(void) {
    char buferis[100];

    printf("Tests uzdevumam PD1.1:\n");
    printf("%d\n", sv_garums("hello world"));
    printf("%d\n", sv_garums("123"));
    printf("%d\n", sv_garums(""));

    printf("Tests uzdevumam PD1.2:\n");
    sv_kopet("First Copy", buferis);
    printf("%s\n", buferis);
    buferis[0] = 0;
    sv_kopet("SecondCopy", buferis);
    printf("%s\n", buferis);
    buferis[0] = 0;
    sv_kopet("1", buferis);
    printf("%s\n", buferis);
    buferis[0] = 0;
    sv_kopet("Copy three strings", buferis);
    printf("%s\n", buferis);
    buferis[0] = 0;
    printf("Tests uzdevumam PD1.3:\n");
    printf("%d\n",sv_meklet("to be or not to be", "be"));
    printf("%d\n",sv_meklet("something else", "be"));
    printf("Tests uzdevumam PD1.4:\n");
    sv_kopet("abcd", buferis);
    sv_apgriezt(buferis);
    printf("%s\n", buferis);
    buferis[0] = 0;

    sv_kopet("hello world ", buferis);
    sv_vapgriezt(buferis);
    printf("%s\n", buferis);
    buferis[0] = 0;
    return 0;
}
