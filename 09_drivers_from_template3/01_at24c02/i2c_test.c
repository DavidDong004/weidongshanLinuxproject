
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <signal.h>

static int fd;

/*
 * ./button_test /dev/100ask_button0
 *
 */
int main(int argc, char **argv)
{
	int ret;
	char buf[100];

	int i;
	
	/* 1. 判断参数 */
	if (argc != 2 && argc != 3 )  
	{
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}


	/* 2. 打开文件 */
	fd = open(argv[1], O_RDWR | O_NONBLOCK);
	if (fd == -1)
	{
		printf("can not open file %s\n", argv[1]);
		return -1;
	}

	if (argc == 3)
	{
		ret = write(fd, argv[2], strlen(argv[2] + 1));
	}
	else
	{
		ret = read(fd, buf,100);
		printf("read: %s\n", buf);

	}

	close(fd);
	
	return 0;
}


