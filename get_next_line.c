/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 18:20:17 by fcoindre          #+#    #+#             */
/*   Updated: 2022/12/06 18:27:25 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include<stdio.h> 
#include <fcntl.h>
void check_leaks();

void	check_ret(char *check_s, int check_d)
{
	//sleep(1);
	static int count;

	FILE* fd = fopen("/Users/fcoindre/Desktop/get_next_line_dev/logs", "a");
	if (count == 0)
	{
		fprintf(fd, "\n\n\n");
	}
	
	fprintf(fd, "--------- %d ---------\n", (count + 1));
	fprintf(fd, "[%s] (%i)\n", check_s, check_d);
	count ++;
	fclose(fd);

}

int	save_buffer(int fd, char **buf)
{
	int		char_read;

	*buf = ft_calloc((BUFFER_SIZE + 1), 1);
	if (*buf == NULL)
		return (0);

	char_read = read(fd, *buf, BUFFER_SIZE);
	//(*buf)[char_read] = '\0';

	return (char_read);
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

char *get_next_line(int fd)
{

	char		*buf;
	static char	*stash;
	int			chr_read;
	char		*tmp;
	char		*line;


	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		check_ret("fd = ", fd);
		return (NULL);
	}
	

	chr_read = save_buffer(fd, &buf);
	if (chr_read == -1)
	{
		free(stash);
		stash = NULL;
	}
	while (chr_read > 0)
	{
		tmp = ft_strjoin(stash, buf);

		free(buf);
		buf = NULL;

		free(stash);
		stash = NULL;
		stash = ft_strdup(tmp);

		//printf("controle = %p\n", stash);
		free(tmp);
		tmp = NULL;

		if (ft_strchr(stash, '\n') != NULL)
		{

			line = extract_line(stash);
			tmp = trim_stash(stash);

			free(stash);
			stash = NULL;
			stash = ft_strdup(tmp);

			free(tmp);
			tmp = NULL;

			check_ret(line, 0);
			return (line);
		}

		chr_read = save_buffer(fd, &buf);
		if (chr_read == -1)
		{
			free(stash);
			stash = NULL;
		}
		
	}

	if (ft_strlen(stash) > 0 && ft_strchr(stash, '\n') != NULL)
	{

		line = extract_line(stash);
		tmp = trim_stash(stash);

		free(stash);
		stash = NULL;

		stash = ft_strdup(tmp);
		free(tmp);
		tmp = NULL;

		if (buf != NULL)
		{
			free(buf);
			buf = NULL;
		}
		
		check_ret(line, 1);
		return (line);
	}

	if (ft_strlen(stash) > 0 && ft_strchr(stash, '\n') == NULL)
	{
		line = ft_strdup(stash);
		free(stash);
		stash = NULL;
		if (buf != NULL)
		{
			free(buf);
			buf = NULL;
		}
		check_ret(line, 2);
		return (line);
	}
	
	if (ft_strlen(stash) == 0 && stash != NULL)
	{
		free(stash);
		stash = NULL;
	}
	

	if (buf != NULL)
	{
		free(buf);
		buf = NULL;
	}
	

	check_ret(line, 3);
	return (NULL);	
}
/*
int main()
{
	//sleep(5);
	int fd = open("files/41_no_nl", O_RDWR);

	char	*line;
	int count;

	count = 0;
	while (count < 2)
	{
		line = get_next_line(fd);	
		printf(" line %d : '%s'", count ,line);
		free(line);
		count ++;
	}
	

	close(fd);

	check_leaks();
	return 0;
}*/
