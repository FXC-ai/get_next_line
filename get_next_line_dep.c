/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 18:20:17 by fcoindre          #+#    #+#             */
/*   Updated: 2022/12/05 21:34:32 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include<stdio.h> 
#include <fcntl.h>
void check_leaks();

static int	ft_strchr_n(const char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static char *extract_line(char *stash)
{

	int		size_line;
	char	*line;
	int		i;

	size_line = 0;	
	while (stash[size_line] != '\n' && stash[size_line] != '\0')
		size_line ++;
	
	line = (char *) malloc(sizeof(char) * (size_line + 2));
	if (line == NULL)
		return (NULL);
	i = 0;
	while (i < (size_line + 1))
	{
		line[i] = stash[i];
		i ++;
	}
	line[i] = '\0';
	return line;
}

static char	*trim_stash(char *stash)
{

	size_t	size_line = 0;
	size_t	size_stash = 0;
	int		i;
	char	*tmp;

	size_line = 0;	
	while (stash[size_line] != '\n')
		size_line ++;


	size_stash = ft_strlen(stash);

	size_t montre = size_stash - size_line;

	tmp = (char *) malloc(sizeof(char) * (montre));
	if (tmp == NULL)
		return (NULL);

	i = 0;
	while (stash[i + size_line + 1] != '\0')
	{
		tmp[i] = stash[i + size_line + 1];
		i++;
	}
	
	tmp[i] = '\0';

	return tmp;

}
#include <stdio.h>
void test(char *line, int lol)
{
	if (BUFFER_SIZE == 42)
		sleep(1);
	static int count;
	FILE* fd = fopen("/Users/fcoindre/Desktop/get_next_line_dev/logs", "a");
	fprintf(fd, "%d : [%s] (%i)\n", count,line, lol);
	count ++;
	fclose(fd);
}

char *get_next_line(int fd)
{
	char		buf[BUFFER_SIZE + 1];
	char		*tmp;
	static char	*stash;
	int			rst;
	char		*line;


	line = 0;
	if (fd < 0 || BUFFER_SIZE <= 0 )
	{
		test(line, 5);
		return (NULL);
	}
	
	rst = 0;
	while ((rst = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[rst] = '\0';

		tmp = ft_strjoin(stash, buf);
		if (tmp == NULL)
		{
			free(stash);
			stash = NULL;
			test(line, 3);
			return NULL;
		}
		

		free(stash);				
		stash = ft_strdup(tmp);
		free(tmp);

	}

	if (rst < 0)
	{
		free(stash);
		stash = NULL;
		test(line, 4);
		return NULL;
	}
	
	if (stash != NULL && ft_strchr_n(stash) == 1)
	{
		line = extract_line(stash);
		tmp = trim_stash(stash);

		if (stash != NULL)
		{
			free(stash);
			stash = NULL;
		}

		stash = ft_strdup(tmp);

		free(tmp);
		
		if (stash[0] == '\0')
		{
			free(stash);
			stash = NULL;
		}
		
		test(line, 0);
		return (line);
	}

	if (ft_strlen(stash) > 0)
	{

		line = ft_strdup(stash);
		free(stash);
		stash = NULL;
		test(line, 1);
		return (line);
	}
	
	test(line, 2);
	return (NULL);
}


/*int main (int argc, char *argv[]) 
{

	(void) argc;
	

	char *line;

	line = NULL;
	
	int fd = open(argv[1], O_RDWR);         

	int i = 0;
	while (i < 9)
	{

		line = get_next_line(fd);
		
		if (line != NULL)
		{
			printf("Ligne numero %d : %s", i+1, line);
			free(line);
		}

		i++;
	}

	close(fd);


	check_leaks();

    return 0; 
}*/