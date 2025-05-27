/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:33:33 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/27 21:52:29 by mlitvino         ###   ########.fr       */
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
		ft_destoy_token_list(tokens_list);
		result = 1;
		*status = MISUSE;
	}
	else if (first_token->type == NEWLINE)
	{
		ft_destoy_token_list(tokens_list);
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
		ft_destoy_token_list(tokens_list);
		*status = MISUSE;
		result = 1;
	}
	else if (token->type == NEWLINE)
	{
		ft_putstr_fd("[SYNTAX ERROR] multiple line not allowed\n", 2);
		ft_destoy_token_list(tokens_list);
		*status = MISUSE;
		result = 1;
	}
	return (result);
}

int	check_last_word_token(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (!result && ft_check_closing_quotes(token->value))
	{
		ft_putstr_fd("[SYNTAX ERROR] multiple line not allowed\n", 2);
		ft_destoy_token_list(tokens_list);
		*status = MISUSE;
		result = 1;
	}
	return (result);
}

int	check_word_token(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (ft_check_closing_quotes(token->value))
	{
		ft_putstr_fd("[SYNTAX ERROR] multiple line not allowed\n", 2);
		ft_destoy_token_list(tokens_list);
		*status = MISUSE;
		result = 1;
	}
	return (result);
}

int	check_token_next_semi(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (token->type == PIPE || token->type == SEMI)
	{
		ft_print_systax_error(token);
		ft_destoy_token_list(tokens_list);
		*status = MISUSE;
		result = 1;
	}
	return (result);
}
