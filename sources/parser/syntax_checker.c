/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:17:56 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/02 00:33:16 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quote_return(int quote)
{
	if (quote != 0)
		return (1);
	else
		return (0);
}

int ft_check_closing_quotes(char *word)
{
	int in_double;
	int in_single;
	int i;

	i = -1;
	in_double = 0;
	in_single = 0;
	while (word[++i] != '\0')
		ft_check_quotes(&in_double, &in_single, word, &i);
	if (in_double || in_single)
		return (1);
	return (0);
}

int	ft_check_word_semi_pipe_redir(t_token *tokens_list, t_token *token,
		int *status)
{
	int	result;

	result = 0;
	if (token->type == NONE)
		result = check_first_token(tokens_list, token->next, status);
	else if (token->type == GREAT || token->type == DOUBLE_GREAT
		|| token->type == LESS)
		result = check_redir(tokens_list, token->next, status);
	else if (token->type == PIPE)
		result = check_token_next_pipe(tokens_list, token->next, status);
	else if (token->type == SEMI)
		result = check_token_next_semi(tokens_list, token->next, status);
	else if (token->type == WORD)
		result = check_word_token(tokens_list, token, status);
	return (result);
}

int	ft_syntax_check(t_token *tokens_list, int *status)
{
	int		result;
	t_token	*tmp;

	result = 0;
	tmp = tokens_list;
	while (tmp->type != NEWLINE)
	{
		if (tmp->type == WORD && tmp->next->type == NEWLINE)
		{
			result = check_last_word_token(tokens_list, tmp, status);
			if (result)
				break ;
		}
		else
		{
			result = ft_check_word_semi_pipe_redir(tokens_list, tmp, status);
			if (result)
				break ;
		}
		tmp = tmp->next;
	}
	return (result);
}
