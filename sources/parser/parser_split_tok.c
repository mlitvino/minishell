/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_split_tok.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:58:05 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/11 17:22:42 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*add_subtoken(t_token **orig_list, char *content, int type)
{
	t_token	*token_list;

	token_list = *orig_list;
	if (!content)
		return (NULL);
	if (!token_list)
	{
		token_list = (t_token *)malloc(sizeof(t_token));
		*orig_list = token_list;
	}
	else
	{
		while (token_list->next != NULL)
			token_list = token_list->next;
		token_list->next = (t_token *)malloc(sizeof(t_token));
		token_list = token_list->next;
	}
	if (!token_list)
		return (free(content), NULL);
	token_list->index = 0;
	token_list->type = type;
	token_list->value = content;
	token_list->next = NULL;
	return (token_list);
}

char	*subvalue(char *str, int *curr_i)
{
	char	*result;
	char	*part;
	char	*tmp;

	result = ft_strdup("");
	while (result && str[*curr_i] && str[*curr_i] != ' ')
	{
		part = extract_substr(str, curr_i);
		if (!part)
			return (free(result), NULL);
		tmp = result;
		result = ft_strjoin(tmp, part);
		free(tmp);
		free(part);
	}
	return (result);
}

t_token	*split_token(t_data *data, t_token *token, int *empty_flag)
{
	int		curr_i;
	t_token	*new_token_list;
	char	*new_value;
	char	*expnd_str;

	curr_i = 0;
	new_token_list = NULL;
	expnd_str = expnd_token_value(data, token, empty_flag, &curr_i);
	while (expnd_str[curr_i])
	{
		new_value = subvalue(expnd_str, &curr_i);
		while (expnd_str[curr_i] && expnd_str[curr_i] == ' ')
			curr_i++;
		if (add_subtoken(&new_token_list, new_value, token->type) == NULL)
		{
			free(expnd_str);
			ft_destoy_token_list(new_token_list);
			return (NULL);
		}
	}
	return (free(expnd_str), new_token_list);
}

void	*expand_tokens_list(t_data *data, t_token **tokens)
{
	t_token	*current;
	t_token	*temp_prev;
	t_token	*new_tokens;
	int		empty_flag;

	current = (*tokens)->next;
	temp_prev = *tokens;
	while (current->type != NEWLINE)
	{
		empty_flag = 0;
		if (current->type == DOUBLE_LESS)
		{
			temp_prev = current->next;
			current = current->next->next;
			continue ;
		}
		new_tokens = split_token(data, current, &empty_flag);
		if (!new_tokens && empty_flag == 0)
			return (NULL);
		join_tokens(&current, &temp_prev, new_tokens, empty_flag);
	}
	return (tokens);
}
