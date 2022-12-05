/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 18:20:17 by fcoindre          #+#    #+#             */
/*   Updated: 2022/12/05 17:54:06 by fcoindre         ###   ########.fr       */
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

char *get_next_line(int fd)
{
	char		buf[BUFFER_SIZE + 1]; //attention car si aucun caratere n est lu le buffer est plus grand que la chaine qu on met dedans
	char		*tmp;
	static char	*stash;
	int			rst;
	char		*line;



	if (fd < 0 || BUFFER_SIZE <= 0 )
	{
		return (NULL);
	}
	
	rst = 0;
	while ((rst = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		//rst = read(fd, buf, BUFFER_SIZE);
		

		buf[rst] = '\0';

		//printf("buf = %s\n", buf);

		
		tmp = ft_strjoin(stash, buf); 
		free(stash);				/*APRES AFFICHAGE DE LA DERNIERE LIGNE LA STASH A DEJA ETE FREE DU COUP SI ON RAPPEL LA FONCTION ON ESSAIE DE REFREE UN TRUC DEJA FREE*/
		stash = ft_strdup(tmp);
		free(tmp);



	}

	if (rst < 0)
	{
		free(stash);
		stash = NULL;
		return NULL;
	}
	
	//lorsque la derniere ligne comporte un \n alors la stash n'est pas free et ça fait des leaks
	//lor



	if (stash != NULL && ft_strchr_n(stash) == 1)
	{
		line = extract_line(stash);
		tmp = trim_stash(stash);

		
		if (stash != NULL)
		{
			free(stash);
			stash = NULL;
			/* code */
		}
		

		//printf("sth = %s || %p\n", stash, stash);
		
		stash = ft_strdup(tmp);

		//printf("sth = %s || %p\n",stash, stash);


		free(tmp);
		
		if (stash[0] == '\0')
		{
			free(stash);
			stash = NULL;
		}
		
		//free(tmp);
		return (line);
	}

	if (ft_strlen(stash) > 0)
	{
		//printf("stash = %s \n", stash);
		line = ft_strdup(stash);
		free(stash);
		stash = NULL;
		//printf("line = %s \n", line);
		return (line);
	}
	
	return (NULL);
}

/*
int main (int argc, char *argv[]) 
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

	//(void) argv;

	//free(line);

	check_leaks();

    return 0; 
}*/