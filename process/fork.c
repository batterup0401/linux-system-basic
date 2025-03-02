#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void test_func(void)
{
	printf("test_func! in pid of %d\n", getpid());
}

int main(void)
{
	pid_t pid;

	printf("origin process pid: %d\n", getpid());

	pid = fork();
	if (pid == 0)
	{
		// child process
		printf("child process pid: %d\n", getpid());
	}
	else if (pid > 0)
	{
		// parent process
		printf("parent process pid: %d\n", getpid());
		printf("\\_child process pid: %d\n", pid);
	
		// But, the parent process should not end
		// without clearing the memory of the child process
		// Use wait() API!
	}

	test_func();

	return 0;
}
