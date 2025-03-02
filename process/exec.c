#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void test_func(void)
{
	printf("test_func! in pid of %d\n", getpid());
}

int main(void)
{
	pid_t pid;
	int exit_status;

	printf("origin process pid: %d\n", getpid());

	pid = fork();
	if (pid == 0)
	{
		// child process
		printf("child process pid: %d\n", getpid());

		execl("/bin/ls", "ls", "-al", "/tmp",  NULL);
	}
	else if (pid > 0)
	{
		// parent process
		printf("parent process pid: %d\n", getpid());
		printf("\\_child process pid: %d\n", pid);
	}

	test_func();

	pid = wait(&exit_status);
	if (WIFEXITED(exit_status))
	{
		printf("child process[pid: %d] returns %d\n", pid, WEXITSTATUS(exit_status));
	}
	else
	{
		printf("child process[pid: %d] is not exited\n", pid);
	}

	return 0;
}
