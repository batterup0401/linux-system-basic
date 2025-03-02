#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

int main()
{
	int fd;
	int wd1, wd2;
	char buf[1024];
	struct inotify_event *event;
	int ret;

	fd_set fds;
	struct timeval timeout;

	// Create inotify object
	fd = inotify_init();
	
	// Add watch
	wd1 = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
	wd2 = inotify_add_watch(fd, "..", IN_CREATE | IN_DELETE);
	
	while(1)
	{
		// You should set up whenever call select() API!
		// Setting up fd_set
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		FD_SET(STDIN_FILENO, &fds);
		int nfds = fd > STDIN_FILENO ? fd+1 : STDIN_FILENO+1;

		// Setting timeval
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		printf("I'm waiting for something to happen...\n");
		int fd_count = select(nfds, &fds, NULL, NULL, &timeout);
			
		if (fd_count == 0)
		{
			printf("Timeout occured in select() API~!\n");
		}
		else if (fd_count > 0)
		{
			memset(buf, 0, sizeof(buf));
		
			if (FD_ISSET(fd, &fds))
			{
				ret = read(fd, buf, sizeof(buf));

				event = (struct inotify_event *)&buf[0];
	
				while (ret > 0)
				{
					if (event->mask & IN_CREATE)
					{
						printf("File[%s](len: %d) is created!\n", event->name, event->len);
					}
					else if (event->mask & IN_DELETE)
					{
						printf("File[%s](len: %d) is deleted!\n", event->name, event->len);
					}

					int realSize = sizeof(struct inotify_event) + event->len;
					ret -= realSize;
					event = (struct inotify_event *)((char *)event + realSize);
				}
			}
			else if (FD_ISSET(STDIN_FILENO, &fds))
			{
				ret = read(STDIN_FILENO, buf, sizeof(buf));

				printf("user input: [%s]\n", buf);
			}	
		}
	}

	// close
	close(wd1);
	close(wd2);
	close(fd);

	return 0;
}
