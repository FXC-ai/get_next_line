/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 16:22:04 by fcoindre          #+#    #+#             */
/*   Updated: 2022/12/01 11:10:09 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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