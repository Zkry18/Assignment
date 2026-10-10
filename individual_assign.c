#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void stop(int sig) {
    const char msg[] = "\nInterrupt: Program stopped.\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    _exit(0);
}

int main(void) {
   /* signal(SIGINT, stop);*/ 
    for(int i=0; i<4; i++){
    int fd[2];
    if (pipe(fd) < 0) { perror("pipe"); exit(1); }

    pid_t pid = fork();

    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0){ //Child
	signal(SIGINT, stop);
	close(fd[1]); //closed input
	char name[50];
	ssize_t n = read(fd[0], name, sizeof(name) - 1);
	if (n > 0) {
		name[n] = '\0';
		printf("Child %d: My name is %s\n",i+1, name);
	}
	close(fd[0]);
	exit(0);
	}

    else { //Parent
	close(fd[0]);//close output
	char in[50];
	printf("Parent: Enter child %d's name: ",i+1);
	fgets(in, sizeof(in), stdin);
	write(fd[1], in, strlen(in));
	close(fd[1]);
	wait(NULL);
        }
	}
return(0);
}
