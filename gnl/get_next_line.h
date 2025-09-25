/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raelhara <raelhara@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-24 13:28:56 by raelhara          #+#    #+#             */
/*   Updated: 2025-05-24 13:28:56 by raelhara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

char	*get_next_line(int fd);
char	*ft_strdup(char *src);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strchr2(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*read_and_build_line(int fd, char *buffer, char *line, char **stash);
char	*duplicate_stash(char **stash);
char	*stash_has_nl(char **stash, char *buffer);
#endif