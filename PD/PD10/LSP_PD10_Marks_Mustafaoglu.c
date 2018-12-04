#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int socketPair[2];
    pid_t pid;

    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair);
    if (ret) {
        perror("socketpair");
        return 1;
    }

    pid = fork();

    if(pid == 0){
        // in child code: read from socket, process arguments, write reply, exit

        char myBuffer[256] = {'\0'};

        read(socketPair[1], &myBuffer, sizeof(myBuffer));
        printf("Child socket: '%s'\n", myBuffer);

        int num1 = 0;
        int num2 = 0;
        sscanf(myBuffer, "%d %d", &num1, &num2);

        int sum = num1 + num2;
        printf("Sum result: %d\n", sum);
        write(socketPair[1], &myBuffer, sizeof(myBuffer));

        close(socketPair[1]);
    }
    else{
        // in parent code: write to socket, wait for reply, read it, and output it

        char input[20];
        printf("Input two numbers:\n");
        scanf("%[^\n]", input);

        write(socketPair[0], &input, sizeof(input));
        read(socketPair[0], &input, sizeof(input));

        printf("Parent socket: '%s'\n", input);
        close(socketPair[0]);
    }

    return 0;
}