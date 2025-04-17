/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:27:10 by mipinhei          #+#    #+#             */
/*   Updated: 2025/04/17 19:33:55 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_line(int fd, char *stash)
{
	int		byte_read;
	char	*buf;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	byte_read = 1;
	while (byte_read != 0 && ft_strchr(stash, 10) == 0)
	{
		byte_read = read(fd, buf, BUFFER_SIZE);
		buf[byte_read] = '\0';
		stash = ft_strjoin(stash, buf);
		if (!stash)
			return (NULL);
	}
	free(buf);
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
	while (stash[i] != '\n' && stash[i])
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*rest_stash(char *stash)
{
	char	*temp;
	size_t	len;
	size_t	i;

	len = 0;
	while (stash[len] != '\n')
		len++;
	temp = malloc(sizeof(char) * (len = 1));
	if (!temp)
		return (NULL);
	i = 0;
	while (stash[len + i] != '\0')
	{
		temp[i] = stash[len + i];
		i++;
	}
	temp[i] = '\0';
	free(stash);
	return (temp);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	if (!stash)
	{
		stash = malloc(1);
		stash[0] = '\0';
	}
	stash = read_line(fd, stash);
	if (!stash || *stash == '\0')
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = write_line(stash);
	stash = rest_stash(stash);
	return (line);
}
