/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:12:41 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/07 13:35:48 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_no_quoting_word(char *line, int *i)
{
	char	*word;
	int		j;

	j = *i;
	while (line[j] && !ft_strrchr("\t '\"\\<>;|", line[j]))
		j++;
	word = ft_substr(line, *i, j - *i);
	*i = j;
	return (word);
}

void	ft_destoy_token_list(t_token *tokens_list)
{
	t_token	*tmp;

	tmp = NULL;
	while (tokens_list->type != NEWLINE)
	{
		tmp = tokens_list;
		tokens_list = tokens_list->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
	if (tokens_list->type == NEWLINE)
	{
		free(tokens_list->value);
		tokens_list->value = NULL;
		free(tokens_list);
		tokens_list = NULL;
	}
}
