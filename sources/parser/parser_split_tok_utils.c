/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_split_tok_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 17:05:45 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/11 17:23:53 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_substr(char *str, int *curr_i)
{
	int		start;
	char	*part;

	part = NULL;
	if (str[*curr_i] == '\'' || str[*curr_i] == '\"')
	{
		start = *curr_i;
		*curr_i += get_i_end_token(&str[*curr_i], str[*curr_i]);
		part = ft_substr(str, start, *curr_i - start);
	}
	else
	{
		start = *curr_i;
		while (str[*curr_i] && str[*curr_i] != ' ' && str[*curr_i] != '\''
			&& str[*curr_i] != '\"')
			*curr_i += 1;
		part = ft_substr(str, start, *curr_i - start);
	}
	return (part);
}

void	*expnd_token_value(t_data *data, t_token *token,
		int *empty_flag, int *curr_i)
{
	char	*expnd_str;

	expnd_str = expand_str(data, token->value, ft_strdup(""), 1);
	if (!expnd_str)
		return (NULL);
	if (!*expnd_str)
		*empty_flag = 1;
	while (expnd_str[*curr_i] && expnd_str[*curr_i] == ' ')
		(*curr_i)++;
	return (expnd_str);
}

void	join_tokens(t_token **current, t_token **temp_prev,
			t_token *new_tokens, int empty_flag)
{
	t_token	*last_new;

	if (empty_flag == 1)
	{
		*current = (*current)->next;
		free((*temp_prev)->next->value);
		free((*temp_prev)->next);
		(*temp_prev)->next = *current;
	}
	else
	{
		(*temp_prev)->next = new_tokens;
		last_new = new_tokens;
		while (last_new->next)
			last_new = last_new->next;
		last_new->next = (*current)->next;
		free((*current)->value);
		free(*current);
		*current = last_new->next;
		*temp_prev = last_new;
	}
}
