/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:33:33 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/02 00:24:10 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_first_token(t_token *tokens_list, t_token *first_token, int *status)
{
	int	result;

	result = 0;
	if (first_token->type == PIPE || first_token->type == SEMI)
	{
		ft_print_systax_error(first_token);
		ft_destroy_token_list(tokens_list);
		result = 1;
		*status = 258;
	}
	else if (first_token->type == NEWLINE)
	{
		ft_destroy_token_list(tokens_list);
		*status = 0;
		result = 1;
	}
	return (result);
}

int	check_token_next_pipe(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (token->type == PIPE || token->type == SEMI)
	{
		ft_print_systax_error(token);
		ft_destroy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	else if (token->type == NEWLINE)
	{
		ft_putstr_fd("[SYNTAX ERROR] multiple line not allowed\n", 1);
		ft_destroy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	return (result);
}

int	check_last_word_token(t_token *tokens_list, t_token *token, int *status)
{
	int result;
	char *new_input;

	result = 0;
	if (ft_check_closing_quotes(token->value))
	{
		while (1)
		{
			ft_putstr_fd("dequote > ", 1);
			new_input = get_next_line(0);
			if (new_input == NULL)
			{
				ft_putstr_fd("unexpected EOF while looking for matching quote\n", 2);
				ft_destroy_token_list(tokens_list);
				*status = 258;
				result = 1;
				break ;
			}
			token->value = ft_strjoin(token->value, "\n");
			token->value = ft_strjoin(token->value, new_input);
			free(new_input);
			if (ft_check_closing_quotes(token->value) == 0)
				break ;
		}
		if (!result)
		{
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->index = token->index + 1;
			token->next->type = NEWLINE;
			token->next->value = ft_strdup("newline");
			token->next->next = NULL;
		}
	}
	return (result);
}

int	check_word_token(t_token *tokens_list, t_token *token, int *status)
{
	if (ft_check_closing_quotes(token->value))
	{
		ft_putstr_fd("[SYNTAX ERROR] unclosed quote in token\n", 2);
		ft_destroy_token_list(tokens_list);
		*status = 258;
		return (1);
	}
	return (0);
}

int	check_token_next_semi(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (token->type == PIPE || token->type == SEMI)
	{
		ft_print_systax_error(token);
		ft_destroy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	return (result);
}
