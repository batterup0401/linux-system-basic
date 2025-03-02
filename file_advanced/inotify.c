#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
		
int main()
{
	int fd;
	int wd1, wd2;
	char buf[1024];
	struct inotify_event *event;

	// Create inotify object
	fd = inotify_init();
	
	// Add watch
	wd1 = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
	wd2 = inotify_add_watch(fd, "..", IN_CREATE | IN_DELETE);
	
	while(1)
	{
		printf("I'm waiting for something to happen...\n");
		int ret = read(fd, buf, sizeof(buf));
		
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

	// close
	close(wd1);
	close(wd2);
	close(fd);

	return 0;
}
