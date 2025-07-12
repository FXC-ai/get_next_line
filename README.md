# Get Next Line

# A propos des variables statiques

Les variables statiques conservent la valeur qu’elles ont reçus dans leur scope précédent. Elles ne sont par réinitialisées à chaque utilisation.

Ainsi le code suivant permet de compter le nombre de fois que la fonction fun() est appelée car elle incrémente une variable statique à chaque appel. L’initialisation de variables statiques est faite avant l’execution du main. C’est pourquoi, il est impossible d’initialiser une variable statique avec une fonction car cela supposerait de lancer le programme sans connaître la valeur initiale de la variable statique.

La variable statique est stockée dans le segment data de la mémoire. C’est pourquoi il est impossible mettre une variable statique dans une structure. Tous les éléments de la structure doivent se trouver dans le même segment de mémoire.

```c
#include<stdio.h>

int fun()
{
  static int count = 0;
  count++;
  return count;
}
  
int main()
{
  printf("%d ", fun());
  printf("%d ", fun());
  return 0;
}
```

Les variables statiques sont initialisées à 0 même si elles n’ont pas été initialisées explicitement.

## Sources

[Static functions in C - GeeksforGeeks](https://www.geeksforgeeks.org/what-are-static-functions-in-c/)

[Static Variables in C - GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/)

[Static variable - Wikipedia](https://en.wikipedia.org/wiki/Static_variable)

# Le code avant Norminette

```c
#include "get_next_line.h"

#include<stdio.h> 
#include <fcntl.h>
void check_leaks();

void	check_ret(char *check_s, int check_d)
{
	//sleep(1);
	static int count;

	FILE* fd = fopen("/Users/fcoindre/Desktop/backup_get_next_line_dev/logs", "a");
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
```

```c
#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	count;

	if (str == NULL)
	{
		return (0);
	}
	
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

	if (s1 == NULL)
	{
		return (NULL);
	}
	
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

char	*ft_strchr(const char *s, int c)
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
			return ((char *) &s[i]);
		}
		i++;
	}
	return (NULL);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*dst_cpy;
	char	*src_cpy;

	if (!dst && !src)
		return (NULL);
	dst_cpy = (char *) dst;
	src_cpy = (char *) src;
	i = 0;
	while (i < n)
	{
		dst_cpy[i] = src_cpy[i];
		i++;
	}
	return (dst);
}
```

```c
#ifndef GET_NEXT_LINE
# define GET_NEXT_LINE 

#include <unistd.h> 
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 5
#endif 

size_t	ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s1);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strchr(const char *s, int c);

char    *get_next_line(int fd);

#endif
```

# Sources

[https://www.youtube.com/watch?v=XgjJKeL77Ls](https://www.youtube.com/watch?v=XgjJKeL77Ls)

[https://www.youtube.com/watch?v=xgDSXNOHTIA](https://www.youtube.com/watch?v=xgDSXNOHTIA)
