#include <sys/types.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h>

int main() {
	pid_t pid = 0;
	pid = fork();
	if(pid < 0) { 
		printf("Could not create process\n"); 
		exit(1);
	}else if(pid == 0) { 
		sleep(1);
		printf("Hello from the child process\n");
	}else if(pid > 0) { 
		printf("Hello from the parent process\n");
	}
	printf("Child and parent\n");
}