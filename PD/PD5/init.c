#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

jmp_buf jump_buffer;

void termination_handler (int signum)
{
	longjmp(jump_buffer, 666);
}

int main(void)
{
    // TODO: jūsu kods te!
	struct sigaction new_action;
	new_action.sa_handler = termination_handler;
  	sigemptyset (&new_action.sa_mask);

  	new_action.sa_flags = 0;
	sigaction(SIGALRM, &new_action, NULL);
	int result = setjmp(jump_buffer);
	if(result != 0){
		return 0;
	}
    // taalaako kodu nemainiit!

    // taimeris uz 3 sekundeem
    alarm(3);

    // bezgaliigs cikls
    while (1);

    // atgriezh kaut kaadu nenulles kodu, lai kompilators nesuudzeetos
    return -1;
}