/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:27:10 by mipinhei          #+#    #+#             */
/*   Updated: 2025/04/17 15:57:23 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_line(int fd, char *stash, char *buf)
{
	static int		byte_read;

	if (!byte_read)
		byte_read = 1;
	while (byte_read > 0 && ft_strchr(stash, 10) == 0)
	{
		byte_read = read(fd, buf, BUFFER_SIZE);
		stash = ft_strjoin(stash, buf);
		if (!stash)
			return (NULL);
	}
	return (stash);
}

char	*write_line(char *stash)
{
	char	*line;
	size_t	i;
	size_t	len;

	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	line = malloc(sizeof(char) * (len + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (i < len + 2)
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*get_next_line(int fd)
{
	char		*buf;
	static char	*stash;
	char		*line;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	if (!stash)
	{
		stash = malloc(1);
		if (!stash)
			return (NULL);
		stash[0] = '\0';
	}
	stash = read_line(fd, stash, buf);
	line = write_line(stash);
	free(buf);
	return (line);
}
