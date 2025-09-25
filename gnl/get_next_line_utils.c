/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raelhara <raelhara@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-28 08:57:55 by raelhara          #+#    #+#             */
/*   Updated: 2025-05-28 08:57:55 by raelhara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup(char *src)
{
	int		i;
	int		j;
	char	*myarray;

	i = 0;
	j = 0;
	while (src[i])
		i++;
	myarray = malloc(i + 1);
	if (!myarray)
		return (NULL);
	while (src[j])
	{
		myarray[j] = src[j];
		j++;
	}
	myarray[j] = '\0';
	return (myarray);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	s_len;
	char	*r;

	i = 0;
	s_len = 0;
	if (!s)
		return (NULL);
	while (s[s_len])
		s_len++;
	if (start >= s_len)
		len = 0;
	else if (len > s_len - start)
		len = s_len - start;
	r = (char *)malloc(len + 1);
	if (!r)
		return (NULL);
	while (i < len && s[start + i])
	{
		r[i] = s[start + i];
		i++;
	}
	r[i] = '\0';
	return (r);
}

char	*ft_strchr(const char *s, int c)
{
	unsigned char	uc;
	int				i;

	uc = (unsigned char)c;
	i = 0;
	while (s[i])
	{
		if ((unsigned char)s[i] == uc)
			return ((char *)s + i + 1);
		i++;
	}
	if (uc == '\0')
		return ((char *)s + i);
	return (NULL);
}

char	*ft_strchr2(const char *s, int c)
{
	unsigned char	uc;
	int				i;
	char			*x;

	x = ft_strdup((char *)s);
	uc = (unsigned char)c;
	i = 0;
	while (s[i])
	{
		if ((unsigned char)s[i] == uc)
		{
			x[i + 1] = '\0';
			return (x);
		}
		i++;
	}
	if (uc == '\0')
		return ((char *)s + i);
	return (NULL);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}
