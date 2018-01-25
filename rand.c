#include <stdio.h> 
#include <stdlib.h> 
#define NUMBER_OF_PROCESSES 4

int main() { 
	int i, status; 
	pid_t pid; 
	printf("Hello from the parent process with PID %d\n", getpid()); 
	pid = fork(); 
	if(pid < 0) { 
		printf("fork error\n"); 
	} else if(pid == 0) { 
		sleep(1); 
		printf("Hello from the child process with PID %d\n", getpid()); 
		return; 
	} 
	waitpid(pid, &status, WUNTRACED); 
	printf("Child process has finished\n"); 
}