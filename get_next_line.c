/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 18:20:17 by fcoindre          #+#    #+#             */
/*   Updated: 2022/12/08 16:05:12 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
//void check_leaks();
#include<stdio.h> 


void	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	char	*result;

	i = 0;
	result = malloc(count * size);
	if (result == NULL)
		return (NULL);
	while (i < count * size)
	{
		*(result + i) = 0;
		i++;
	}
	return ((void *) result);
}

int	save_buffer(int fd, char **buf)
{
	int		char_read;

	*buf = ft_calloc((BUFFER_SIZE + 1), 1);
	if (*buf == NULL)
		return (0);
	char_read = read(fd, *buf, BUFFER_SIZE);
	return (char_read);
}

static char	*extract_line(char *stash)
{
	int		size_line;
	char	*line;
	int		i;

	size_line = 0;
	while (stash[size_line] != '\n' && stash[size_line] != '\0')
	{
		size_line ++;
	}
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
	return (line);
}

static char	*trim_stash(char *stash)
{
	size_t	size_line;
	size_t	size_stash;
	int		i;
	char	*tmp;
	size_t	montre;

	size_stash = 0;
	size_line = 0;
	while (stash[size_line] != '\n')
		size_line ++;
	size_stash = ft_strlen(stash);
	montre = size_stash - size_line;
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
	return (tmp);
}

int	get_next_line_1(char **line, char **stash, char **tmp, char **buf)
{
	if (ft_strlen(*stash) == 0 && *stash != NULL)
	{
		free(*stash);
		*stash = NULL;
	}
	if (ft_strlen(*stash) > 0 && ft_strchr(*stash, '\n') != NULL)
	{
		*line = extract_line(*stash);
		*tmp = trim_stash(*stash);
		free(*stash);
		*stash = ft_strdup(*tmp);
		free(*tmp);
		if (*buf != NULL)
		{
			free(*buf);
		}
		return (1);
	}
	if (ft_strlen(*stash) > 0 && ft_strchr(*stash, '\n') == NULL)
	{
		*line = ft_strdup(*stash);
		free(*stash);
		*stash = NULL;
		if (*buf != NULL)
			free(*buf);
		return (1);
	}
	return (0);
}

char	*get_next_line(int fd)
{
	char		*buf;
	static char	*stash;
	int			chr_read;
	char		*tmp;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
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
		free(stash);
		stash = ft_strdup(tmp);
		free(tmp);
		
		if (ft_strlen(stash) > 0 && ft_strchr(stash, '\n') != NULL)
		{
			line = extract_line(stash);
			tmp = trim_stash(stash);
			free(stash);
			stash = ft_strdup(tmp);
			free(tmp);
			return (line);
		}
		chr_read = save_buffer(fd, &buf);
		if (chr_read == -1)
			free(stash);
	}

	if (get_next_line_1(&line, &stash, &tmp, &buf) == 1)
		return (line);

	if (buf != NULL)
		free(buf);
	return (NULL);
}


/*
#include <fcntl.h>

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

	//check_leaks();
	return 0;
}*/
