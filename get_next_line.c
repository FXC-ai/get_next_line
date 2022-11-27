/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 18:20:17 by fcoindre          #+#    #+#             */
/*   Updated: 2022/11/27 16:48:59 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	while (stash[size_line] != '\n')
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

	int		size_line;
	size_t	size_stash;
	int		i;
	char	*tmp;

	size_line = 0;	
	while (stash[size_line] != '\n')
		size_line ++;


	size_stash = ft_strlen(stash);

	tmp = (char *) malloc(sizeof(char) * (size_stash - size_line));
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
	char		buf[BUFFER_SIZE + 1];
	char		*tmp;
	static char	*stash;
	int			rst;
	char		*line;

	if (fd <= 0)
	{
		return (NULL);
	}
	
	rst = 1;
	while (rst > 0)
	{
		rst = read(fd, buf, BUFFER_SIZE);
		buf[rst] = 0;
		tmp = ft_strjoin(stash, buf);
		free(stash);
		stash = tmp;
		if (ft_strchr_n(stash) == 1)
		{
			line = extract_line(stash);
			tmp = trim_stash(stash);
			free(stash);
			stash = tmp;
			return (line);
		}
	}
	return (NULL);
}

#include<stdio.h> 
#include <fcntl.h>
void check_leaks();

int main (int argc, char *argv[]) 
{

	(void) argc;
	char *line;

	int fd = open(argv[1], O_RDWR);         

	int i = 0;
	while (i < 9)
	{

		line = get_next_line(fd);
		
		if (line != NULL)
		{
			printf("Ligne numero %d : %s\n", i, line);
			free(line);		
		}
		
		i++;
	}
		
	close(fd);
	check_leaks();

    return 0; 
}