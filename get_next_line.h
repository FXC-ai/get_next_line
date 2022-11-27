#ifndef GET_NEXT_LINE
# define GET_NEXT_LINE 

#include <unistd.h> 
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 5
#endif 

size_t	ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);

char *get_next_line(int fd);




#endif