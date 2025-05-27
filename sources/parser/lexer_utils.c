/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:12:41 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/27 21:45:40 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_no_quoting_word(char *line, int *i)
{
	char	*word;
	int		j;

	j = *i;
	while (line[j] && !ft_strrchr("\t '\"<>;|", line[j]))
		j++;
	word = ft_substr(line, *i, j - *i);
	*i = j;
	return (word);
}

void	ft_destoy_token_list(t_token *tokens_list)
{
	t_token	*tmp;

	while (tokens_list)
	{
		free(tokens_list->value);
		tokens_list->value = NULL;
		tmp = tokens_list;
		tokens_list = tokens_list->next;
		free(tmp);
	}
}
