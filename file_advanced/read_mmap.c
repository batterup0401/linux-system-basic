#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

struct person
{
	char name[16];
	int age;
};

static int write_info(struct person *p)
{
	int fd;

	fd = open("person_info", O_WRONLY | O_CREAT | O_APPEND, 0644);

	write(fd, p, sizeof(struct person));

	close(fd);

	return 0;
}

static int dump_info(void)
{
	int fd;
	struct stat sb;
	struct person *p;

	fd = open("person_info", O_RDONLY);

	fstat(fd, &sb);

	p = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

	for (int i=0; i<sb.st_size/sizeof(struct person); i++)
	{
		printf("name: %s / age: %d\n", p[i].name, p[i].age);
	}

	close(fd);

	return 0;
}

int main(void) 
{
	struct person persons[] = {
		{"kim", 40},
		{"mike", 23}
	};

	write_info(&persons[0]);
	write_info(&persons[1]);

	dump_info();

	return 0;
}
