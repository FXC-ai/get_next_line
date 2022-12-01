/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 18:20:17 by fcoindre          #+#    #+#             */
/*   Updated: 2022/12/01 16:55:29 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include<stdio.h> 
#include <fcntl.h>
void check_leaks();

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

char	*ft_strdup(const char *s1)
{
	char	*s1_dup;
	int		i;
	size_t	s1_size;

	s1_size = ft_strlen(s1);
	s1_dup = malloc((s1_size + 1) * sizeof(char));
	if (s1_dup == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		s1_dup[i] = s1[i];
		i++;
	}
	s1_dup[i] = '\0';
	return (s1_dup);
}


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
	char		buf[BUFFER_SIZE + 1]; //attention car si aucun caratere n est lu le buffer est plus grand que la chaine qu on met dedans
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
		if (rst > 0)
		{
			tmp = ft_strjoin(stash, buf);
		//printf("stash = %s a l'adresse %p \n", stash, stash);
			free(stash);											/*APRES AFFICHAGE DE LA DERNIERE LIGNE LA STASH A DEJA ETE FREE DU COUP SI ON RAPPEL LA FONCTION ON ESSAIE DE REFREE UN TRUC DEJA FREE*/
			stash = ft_strdup(tmp);
			free(tmp);
		}
	
		if (rst > 0 && ft_strchr_n(stash) == 1)
		{
			line = extract_line(stash);
			tmp = trim_stash(stash);
			free(stash);
			stash = ft_strdup(tmp);
			free(tmp);
			//free(tmp);
			return (line);
		}
		else if (rst == 0 && stash != NULL && ft_strlen(stash) > 0)
		{
			line = ft_strdup(stash);
			free(stash);
			stash = NULL;
			return (line);
		}
		
		/*
		if (rst == 0 && ft_strlen(stash) > 0)
		{

			tmp = ft_strdup(stash);
			free(stash);
			stash = NULL;
			printf("tmp : %s %p || ", tmp ,tmp);
			printf("sth : %s %p || \n\n", stash , stash);

			return (tmp);
		}*/

	}
	
	return (NULL);
}



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
			printf("Ligne numero %d : %s", i+1, line);
			free(line);		
		}
		
		i++;
	}

/*
	char *test = ft_strdup("\n");
	free(test);
*/	
	close(fd);
	check_leaks();

    return 0; 
}