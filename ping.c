#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>

volatile sig_atomic_t exiting = false;
volatile sig_atomic_t value;

void handle_rts (int signum, siginfo_t* info, void* extra) {
        const int s_pid = info->si_pid;
        value = info->si_value.sival_int;
	printf("ping %u\n", value);
	fflush(stdout);
}


void sigterm_handler (int signum) {
	exiting = true;
}

int main (int argc, char* argv[]) {
	printf("%d\n", getpid());
	fflush(stdout);

/*	sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask, SIGRTMIN);
        sigprocmask(SIG_SETMASK, &mask, NULL);

        sigset_t allowed_mask;
	sigemptyset(&allowed_mask);
	sigaddset(&allowed_mask, SIGTERM);
	sigprocmask(SIG_UNBLOCK, &allowed_mask, &mask);
*/
	struct sigaction action_term;
        memset(&action_term, 0, sizeof(action_term));
        action_term.sa_handler = sigterm_handler;
        action_term.sa_flags = SA_RESTART;
        sigaction(SIGTERM, &action_term, NULL);

	sigaction(SIGRTMIN, &(struct sigaction) {
                .sa_sigaction = handle_rts,
                .sa_flags = SA_RESTART | SA_SIGINFO
                }, NULL);

	printf("Enter the value to pong: ");
	fflush(stdout);
	scanf("%u", &value);

	int pid_int = atoi(argv[1]);
	pid_t pid =  pid_int;

	printf("ping %u\n", value);
	fflush(stdout);
	while (!exiting) {
		sleep(1);
		sigqueue(pid, SIGRTMIN, (union sigval) {
		.sival_int = value
		});

	}
	
	kill(pid_int, SIGTERM);
	return 0;
}
													

