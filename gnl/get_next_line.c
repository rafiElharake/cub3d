/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raelhara <raelhara@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-28 08:46:01 by raelhara          #+#    #+#             */
/*   Updated: 2025-05-28 08:46:01 by raelhara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s2)
		return (NULL);
	res = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!res)
		return (NULL);
	while (s1 && s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = '\0';
	return (res);
}

char	*read_and_build_line(int fd, char *buffer, char *line, char **stash)
{
	ssize_t	bytes_read;
	char	*temp;
	char	*line_part;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		if (ft_strchr(buffer, '\n'))
		{
			line_part = ft_strchr2(buffer, '\n');
			temp = line;
			line = ft_strjoin(temp, line_part);
			free(temp);
			*stash = ft_strdup(buffer + ft_strlen(line_part));
			free(line_part);
			break ;
		}
		temp = line;
		line = ft_strjoin(temp, buffer);
		free(temp);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	return (line);
}

char	*duplicate_stash(char **stash)
{
	char	*line;

	line = NULL;
	if (*stash)
	{
		line = ft_strdup(*stash);
		free(*stash);
		*stash = NULL;
	}
	return (line);
}

char	*stash_has_nl(char **stash, char *buffer)
{
	char	*line;
	char	*old_stash;

	if (*stash && ft_strchr(*stash, '\n'))
	{
		line = ft_strchr2(*stash, '\n');
		old_stash = *stash;
		*stash = ft_strdup(old_stash + ft_strlen(line));
		free(old_stash);
		free(buffer);
		return (line);
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	char		*line;

	buffer = malloc(BUFFER_SIZE + 1);
	line = NULL;
	if (!buffer)
		return (NULL);
	line = stash_has_nl(&stash, buffer);
	if (line)
		return (line);
	line = duplicate_stash(&stash);
	line = read_and_build_line(fd, buffer, line, &stash);
	free(buffer);
	if (!line || *line == '\0')
	{
		free(line);
		free(stash);
		stash = NULL;
		return (NULL);
	}
	return (line);
}
