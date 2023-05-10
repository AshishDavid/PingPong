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

volatile sig_atomic_t flag = false;

void sigusr1_handler (int signal) {
    flag = true;
}


int main () {
	struct sigaction action_usr1;
	memset(&action_usr1, 0, sizeof(action_usr1));
        action_usr1.sa_handler = sigusr1_handler;
        action_usr1.sa_flags = SA_RESTART;
        sigaction(SIGUSR1, &action_usr1, 0);
	printf("%d\n", getpid());
	fflush(stdout);
	while (true) {
		pause();

		if (flag) {
			printf("pong\n");
			break;
		}
	}
	return 0;
}
