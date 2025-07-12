/*#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>

int main()
{

	int fd = open("test", O_RDWR);

	char	*line;
	int count;

	line = "";
	count = 0;
	while (line != NULL)
	{
		line = get_next_line(fd);
		printf(" line %d : '%s'", count ,line);
		free(line);
		count ++;
	}


	close(fd);
	return 0;
}*/