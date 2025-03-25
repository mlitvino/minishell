/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:59:36 by mlitvino          #+#    #+#             */
/*   Updated: 2024/12/09 13:03:48 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero_buf(char *buffer, int size)
{
	if (buffer == NULL)
		return ;
	while (size > 0)
	{
		buffer[size - 1] = '\0';
		size--;
	}
}

int	ft_strlen_buf(char *buffer)
{
	int	len;

	len = 0;
	if (buffer == NULL)
		return (0);
	while (buffer[len])
		len++;
	return (len);
}

char	*ft_sub_buf(char *buffer, int *buf_len, char ***buf_tab, int tab_len)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	new_line = (char *)malloc(sizeof(char) * (*buf_len + 2));
	if (new_line == NULL)
		return (free_buf(buf_tab, tab_len), NULL);
	while (i <= *buf_len)
	{
		new_line[i] = buffer[i];
		i++;
	}
	new_line[i] = '\0';
	j = 0;
	while (buffer[i])
		buffer[j++] = buffer[i++];
	buffer[j] = '\0';
	return (new_line);
}

int	ft_strchr_buf(char *buffer, int *buf_len)
{
	int	i;

	i = 0;
	if (buffer == NULL || *buf_len <= 0)
		return (-1);
	while (i < *buf_len)
	{
		if (buffer[i] == '\n' || buffer[i] == '\0')
		{
			*buf_len = i;
			return (1);
		}
		i++;
	}
	return (-1);
}

char	*ft_realloc_buf(char *buffer, int *new_size)
{
	char	*new_buffer;
	int		i;
	int		buf_size;

	i = 0;
	buf_size = ft_strlen_buf(buffer);
	new_buffer = (char *)malloc(sizeof(char) * (buf_size + BUFFER_SIZE + 1));
	if (new_buffer == NULL)
		return (NULL);
	if (new_size != NULL)
		*new_size = buf_size + BUFFER_SIZE;
	while (i < buf_size)
	{
		new_buffer[i] = buffer[i];
		i++;
	}
	ft_bzero_buf(new_buffer + buf_size, BUFFER_SIZE + 1);
	return (free(buffer), new_buffer);
}
