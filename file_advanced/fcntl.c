#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() 
{
	int fd;
	char buf[128];

	fd = STDIN_FILENO;

	// When fd is Blocking
	memset(buf, 0, sizeof(buf));
	printf("Tyring to read stdin when blocking...\n");
	int ret = read(fd, buf, sizeof(buf));
	printf("read() [%s] : %d bytes\n", buf, ret);

	// get flag
	int flag = fcntl(fd, F_GETFL);
	
	// set flag
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);

	// When fd is Non-Blocking
	memset(buf, 0, sizeof(buf));
	printf("Tyring to read stdin when non-blocking...\n");
	ret = read(fd, buf, sizeof(buf));
	printf("read() [%s] : %d bytes\n", buf, ret);
	if (ret == -1)
	{
		printf("errno=%d : %s\n", errno, strerror(errno));
	}

	return 0;
}
