/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_trim.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:47:25 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/08 12:50:28 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid(char *char_ptr)
{
	if (*char_ptr == '\'' || *char_ptr == '\"')
		return (FAILURE);
	if (*char_ptr == '$' && (*(char_ptr + 1) == '\''
		|| *(char_ptr + 1) == '\"'))
		return (FAILURE);
	return (SUCCESS);
}

char	*trim_delim(t_redir *heredoc)
{
	char	*new_delim;
	int		len;
	int		i;

	if (ft_strchr(heredoc->delim, '\'') != NULL
		|| ft_strchr(heredoc->delim, '\"') != NULL)
		heredoc->inside_quotes = 1;
	len = 0;
	i = 0;
	while (heredoc->delim[i])
		if (is_valid(&heredoc->delim[i++]) == SUCCESS)
			len++;
	new_delim = malloc(sizeof(char) * (len + 1));
	if (!new_delim)
		return (NULL);
	new_delim[len] = '\0';
	while (len >= 0 && i >= 0)
	{
		if (is_valid(&heredoc->delim[i]) == SUCCESS)
			new_delim[len--] = heredoc->delim[i];
		i--;
	}
	free(heredoc->delim);
	heredoc->delim = new_delim;
	return (new_delim);
}
