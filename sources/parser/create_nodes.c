/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_nodes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:49:54 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/07 17:17:04 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*create_insert_arg(t_simple_cmd **cmd, t_token *token)
{
	t_args	*tmp;

	tmp = NULL;
	if ((*cmd)->args == NULL)
	{
		(*cmd)->args = ft_create_arg(token->value);
		if (!(*cmd)->args)
			return (NULL);
	}
	else
	{
		tmp = ft_create_arg(token->value);
		if (!tmp)
			return (NULL);
		ft_insert_arg((*cmd)->args, tmp);
	}
	return (*cmd);
}

t_simple_cmd	*ft_insert_command_and_arguments(t_simple_cmd **cmd,
		t_token *token)
{
	if ((*cmd)->command == NULL)
	{
		(*cmd)->command = ft_strdup(token->value);
		if (!(*cmd)->command)
			return (NULL);
	}
	else
	{
		if (create_insert_arg(cmd, token) == NULL)
			return (NULL);
	}
	return (*cmd);
}

static void	*fill_cmd(t_token **tokens, t_simple_cmd **cmd)
{
	int	r;

	r = 0;
	if ((*tokens)->type == GREAT || (*tokens)->type == DOUBLE_GREAT
		|| (*tokens)->type == LESS || (*tokens)->type == DOUBLE_LESS)
	{
		(*cmd)->redirections = ft_insert_redir((*cmd)->redirections, tokens, r);
		if (!(*cmd)->redirections)
			return (free_cmds(*cmd), NULL);
		r++;
	}
	else if ((*tokens)->type == WORD)
	{
		if (ft_insert_command_and_arguments(cmd, *tokens) == NULL)
			return (free_cmds(*cmd), NULL);
		(*tokens) = (*tokens)->next;
	}
	else
		(*tokens) = (*tokens)->next;
	return (tokens);
}

t_simple_cmd	*ft_create_simple_cmd(t_token **tokens)
{
	t_simple_cmd	*cmd;

	cmd = ft_init_simple_cmd();
	if (!cmd)
		return (NULL);
	while ((*tokens)->type != PIPE && (*tokens)->type != SEMI
		&& (*tokens)->type != NEWLINE)
	{
		if (fill_cmd(tokens, &cmd) == NULL)
			return (NULL);
	}
	return (cmd);
}

void	ft_insert_simple_cmd(t_simple_cmd *head, t_simple_cmd *current_cmd)
{
	t_simple_cmd	*tmp;

	tmp = head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = current_cmd;
}
