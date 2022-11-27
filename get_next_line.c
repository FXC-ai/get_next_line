/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 18:20:17 by fcoindre          #+#    #+#             */
/*   Updated: 2022/11/27 14:07:31 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include<stdio.h> 
#include<string.h> 
#include<unistd.h> 
#include<fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

size_t	ft_strlen(const char *str)
{
	size_t	count;

	count = 0;
	while (*str != '\0')
	{
		str++;
		count++;
	}
	return (count);
}

int	ft_strchr_gnl(const char *s, int c)
{
	char		caract;
	size_t		i;
	size_t		size_s;

	i = 0;
	caract = c;
	size_s = ft_strlen(s);
	while (i <= size_s)
	{
		if (s[i] == caract)
		{
			return (i);
		}
		i++;
	}
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size_s1;
	size_t	size_s2;
	char	*result;
	size_t	i;
	size_t	c;

	if (s1 == NULL)
		s1 = "";
	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	result = malloc(sizeof(char) * (size_s1 + size_s2 + 1));
	if (result == NULL)
		return (NULL);
	i = 0;
	while (i < size_s1)
	{
		result[i] = s1[i];
		i++;
	}
	c = 0;
	while (c < size_s2)
		result[i++] = s2[c++];
	result[i] = '\0';
	return (result);
}


char *extract_line(char *stash)
{

	int		size_line;
	char	*line;
	int		i;

	//line = NULL;
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
	//free(stash);
	
	//printf("i = %d", i);
	return line;


}

char	*trim_stash(char *stash)
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

	
	free(stash);
	stash = tmp;



	return stash;

}

char *get_next_line(int fd)
{

	char		buf[BUFFER_SIZE + 1];
	char		*tmp;
	static char	*stash;
	int			rst;
	char		*line;

	//stash = "";
	//tmp = "";

	int c = 0;	
	while (c < 3)
	{
		printf("-----------------%d-------------------\n", c);
		rst = read(fd, buf, BUFFER_SIZE);
		buf[rst] = 0;

		tmp = ft_strjoin(stash, buf);
		free(stash);
		stash = tmp;
		line = extract_line(stash);
		stash = trim_stash(stash);

		printf("  rst = %d\n", rst);
		printf("  buf = \"%s\"  \n", buf);
		printf("stash = \"%s\"\n", stash);
		printf(" line = \"%s\"\n", line);

		free(line);
		c++;
	}

	return "ccc";
}


int main (int argc, char *argv[]) 
{

	(void) argc;

	int fd = open(argv[1], O_RDWR);         


	get_next_line(fd);

	//free(test);

	//int i = ft_strchr_gnl("0123456789", '5');

	//stash_trim("AAA\nBBB\nCC\n");

	close(fd);

	check_leaks();

    return 0; 
}