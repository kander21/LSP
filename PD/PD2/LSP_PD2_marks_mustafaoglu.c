#include <stdio.h>

int main(int argc, char **argv) {

    FILE *source, *target;
    int errnum;

    source = fopen(argv[1], "r");
    if(source == NULL){
        perror("Error printed by perror");
        return -1;
    }
    else{

        printf("%s\n", "Rewrite output file? (1)Yes (0)No");
        char choice = getchar();
        if(choice == '1'){

            target = fopen(argv[2], "wb");
            if(target == NULL){
                perror("can not open output file");
                return -1;
            }
            size_t n, m;
            unsigned char buff[8192];
            do {
                n = fread(buff, 1, sizeof buff, source);
                if (n) m = fwrite(buff, 1, n, target);
                else   m = 0;
            } while ((n > 0) && (n == m));
            if (m) {
                perror("copy");
                return -1;
            }

            fclose(source);
            fclose(target);
        }
    }
    return 0;
}