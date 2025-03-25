/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:35:25 by mlitvino          #+#    #+#             */
/*   Updated: 2024/12/09 13:04:00 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_init_buf(char **buf_tab, int *buf_len)
{
	int	i;

	*buf_len = 0;
	if (buf_tab != NULL)
	{
		while (buf_tab[*buf_len] != NULL)
			(*buf_len)++;
		return (buf_tab);
	}
	*buf_len = 1;
	buf_tab = (char **)malloc(sizeof(char *) * (*buf_len + 1));
	if (buf_tab == NULL)
		return (free_buf(&buf_tab, *buf_len), NULL);
	i = 0;
	while (i < *buf_len)
	{
		buf_tab[i] = (char *)malloc(1);
		if (buf_tab[i] == NULL)
			return (free_buf(&buf_tab, i), NULL);
		buf_tab[i][0] = '\0';
		i++;
	}
	buf_tab[i] = NULL;
	return (buf_tab);
}

char	**ft_realloc_buf_tab(char **old_tab, int *old_size, int new_size)
{
	char	**new_tab;
	int		i;

	new_tab = (char **)malloc(sizeof(char *) * ((new_size * 2) + 1));
	if (new_tab == NULL)
		return (free_buf(&old_tab, *old_size), NULL);
	i = 0;
	while (i < *old_size)
	{
		new_tab[i] = old_tab[i];
		old_tab[i] = NULL;
		i++;
	}
	while (i < new_size * 2)
	{
		new_tab[i] = (char *)malloc(1);
		if (new_tab[i] == NULL)
			return (free_buf(&new_tab, i), free_buf(&old_tab, *old_size), NULL);
		new_tab[i][0] = '\0';
		i++;
	}
	new_tab[i] = NULL;
	free_buf(&old_tab, *old_size);
	*old_size = new_size * 2;
	return (new_tab);
}

char	*free_buf(char ***buf_tab, int buf_len)
{
	int	i;

	i = 0;
	if (*buf_tab == NULL)
		return (NULL);
	while (i <= buf_len)
	{
		if ((*buf_tab)[i])
		{
			free((*buf_tab)[i]);
			(*buf_tab)[i] = NULL;
		}
		i++;
	}
	free(*buf_tab);
	(*buf_tab) = NULL;
	return (NULL);
}

char	*ft_read(char ***buf_tab, int fd, int tab_len, int *buf_len)
{
	int	read_byte;
	int	new_len;

	read_byte = read(fd, &(*buf_tab)[fd][*buf_len], BUFFER_SIZE);
	*buf_len += read_byte;
	while (read_byte > 0)
	{
		if (ft_strchr_buf((*buf_tab)[fd], buf_len) > 0)
			return (ft_sub_buf((*buf_tab)[fd], buf_len, buf_tab, tab_len));
		new_len = *buf_len;
		(*buf_tab)[fd] = ft_realloc_buf((*buf_tab)[fd], buf_len);
		if ((*buf_tab)[fd] == NULL)
			return (free_buf(buf_tab, tab_len), NULL);
		read_byte = read(fd, &(*buf_tab)[fd][new_len], BUFFER_SIZE);
	}
	if (read_byte < 0)
		return (free_buf(buf_tab, tab_len), NULL);
	if ((*buf_len)-- > 0)
	{
		if (ft_strchr_buf((*buf_tab)[fd], buf_len) > 0)
			return (ft_sub_buf((*buf_tab)[fd], buf_len, buf_tab, tab_len));
		return (ft_sub_buf((*buf_tab)[fd], buf_len, buf_tab, tab_len));
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	**buf_tab = NULL;
	char		*new_line;
	int			tab_len;
	int			buf_len;

	buf_tab = ft_init_buf(buf_tab, &tab_len);
	if (buf_tab != NULL && tab_len <= fd)
		buf_tab = ft_realloc_buf_tab(buf_tab, &tab_len, fd + 1);
	if (fd < 0 || BUFFER_SIZE <= 0 || buf_tab == NULL)
		return (free_buf(&buf_tab, tab_len), NULL);
	buf_tab[fd] = ft_realloc_buf(buf_tab[fd], 0);
	if (buf_tab[fd] == NULL)
		return (free_buf(&buf_tab, tab_len), NULL);
	buf_len = ft_strlen_buf(buf_tab[fd]);
	new_line = ft_read(&buf_tab, fd, tab_len, &buf_len);
	buf_len = tab_len;
	while (buf_tab != NULL && buf_len-- >= 0)
		if (ft_strlen_buf(buf_tab[buf_len + 1]) > 0)
			return (new_line);
	free_buf(&buf_tab, tab_len);
	return (new_line);
}
