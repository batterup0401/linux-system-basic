#define _GNU_SOURCE      

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>

#define MAXCPU 6 // check count of cpu using command "cat /proc/cpuinfo"

static void print_cpuset(cpu_set_t *mask)
{
	printf("Available CPU nums:");
	
	for (int i=0; i<MAXCPU; i++)
	{
		if (CPU_ISSET(i, mask))
		{
			printf(" %d", i);
		}
	}
	printf("\n");
}

int main(void)
{
	int cur_cpu;
	cpu_set_t mask;
	
	sched_getaffinity(getpid(), sizeof(cpu_set_t), &mask);
	print_cpuset(&mask);

	cur_cpu = sched_getcpu();
	printf("running on CPU %d\n", cur_cpu);

	CPU_ZERO(&mask);
	CPU_SET(cur_cpu^1, &mask);

	sched_setaffinity(getpid(), sizeof(cpu_set_t), &mask);

	sleep(2);

	cur_cpu = sched_getcpu();
	printf("Now, running on CPU %d\n", cur_cpu);

	return 0;
}
