/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:17:56 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/27 13:28:54 by mlitvino         ###   ########.fr       */
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

int	ft_check_closing_quotes(char *word)
{
	int	i;
	int	quote;
	int	back_slash;

	back_slash = 0;
	quote = 0;
	i = -1;
	while (word[++i])
	{
		back_slash = count_bachslashes(word, &i, back_slash);
		if (quote == 0 && word[i] == 34)
			quote = ft_get_first_double_quotes(word, &i, &back_slash);
		else if (quote == 0 && word[i] == '\'')
			quote = ft_get_first_single_quotes(word, &i, &back_slash);
		else if (quote == 2 && word[i] == 34)
			ft_get_close_double_quotes(word, &i, &back_slash, &quote);
		else if (quote == 1 && word[i] == '\'')
			ft_get_close_single_quotes(&i, &back_slash, &quote);
		else
			back_slash = 0;
	}
	return (quote_return(quote));
}

int	ft_check_backslash(char *word)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (word[i])
		i++;
	if (word[i - 1] == '\\')
	{
		i--;
		while (word[i] == '\\')
		{
			count++;
			i--;
		}
		if ((count % 2) != 0)
			return (1);
		else
			return (0);
	}
	else
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
		|| token->type == LESS || token->type == DOUBLE_LESS)
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
