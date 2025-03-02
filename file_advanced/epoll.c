#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>

int main()
{
	int fd;
	int wd1, wd2;
	char buf[1024];
	struct inotify_event *event;
	int ret;

	int epfd;
	struct epoll_event ep_event;

	// Create inotify object
	fd = inotify_init();
	
	// Add watch
	wd1 = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
	wd2 = inotify_add_watch(fd, "..", IN_CREATE | IN_DELETE);

	// Create epoll instance
	epfd = epoll_create1(0);

	memset(&ep_event, 0, sizeof(ep_event));
	ep_event.events = EPOLLIN;
	ep_event.data.fd = fd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ep_event); 

	memset(&ep_event, 0, sizeof(ep_event));
	ep_event.events = EPOLLIN;
	ep_event.data.fd = STDIN_FILENO;
	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ep_event);	

	while(1)
	{
		memset(&ep_event, 0, sizeof(ep_event));

		printf("I'm waiting for something to happen...\n");
		int fd_count = epoll_wait(epfd, &ep_event, 1, 5000);

		if (fd_count == 0)
		{
			printf("Timeout occured in select() API~!\n");
		}
		else if (fd_count > 0)
		{
			memset(buf, 0, sizeof(buf));
		
			if (ep_event.data.fd == fd)
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
			else if (ep_event.data.fd == STDIN_FILENO)
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
