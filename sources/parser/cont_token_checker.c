/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cont_token_checker.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:58:16 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/14 00:18:27 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_systax_error(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 1);
	ft_putstr_fd(token->value, 1);
	ft_putstr_fd("'\n", 1);
}

int	check_redir(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (token->type != WORD)
	{
		ft_print_systax_error(token);
		ft_destoy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	return (result);
}
