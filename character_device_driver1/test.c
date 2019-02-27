#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IOCTL_SET_MIXED _IO(202, 0)
#define IOCTL_SET_UPPER _IO(202, 1)

int main (int argc, char **argv)
{
	if (argc != 2)
	{
		printf("error - need to specify mode.");
	}

	int fd = open("char_dev_drv1", O_RDWR);
	if (fd < 0)
	{
		printf("could not open\n");
		exit(0);
	}

	int rc;
	switch (atoi(argv[1]))
	{
		case 0:
			rc = ioctl(fd, IOCTL_SET_MIXED, 0);
			break;

		case 1:
			rc = ioctl(fd, IOCTL_SET_UPPER, 0);
			break;
	}
	close(fd);

	printf("%d\n", rc);

	return 0;
}
