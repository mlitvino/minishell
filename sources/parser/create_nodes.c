/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_nodes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:49:54 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/07 13:46:49 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_list	*init_cmd_list(void)
{
	t_cmd_list	*cmd_list;

	cmd_list = NULL;
	cmd_list = (t_cmd_list *)malloc(sizeof(t_cmd_list));
	if (!cmd_list)
		return (NULL);
	cmd_list->childs = NULL;
	cmd_list->pipe_line_count = 0;
	return (cmd_list);
}

t_simple_cmd	*ft_init_simple_cmd(void)
{
	t_simple_cmd	*cmd;

	cmd = (t_simple_cmd *)malloc(sizeof(t_simple_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->command = NULL;
	cmd->next = NULL;
	cmd->args = NULL;
	cmd->redirections = NULL;
	cmd->inside_quotes = 0;
	cmd->exit_code = SUCCESS;
	cmd->pathname = NULL;
	cmd->cmd_i = 0;
	cmd->cmd_count = 0;
	cmd->builtin_arr = NULL;
	cmd->cmd_pid = -1;
	cmd->std_fd[STDIN] = -1;
	cmd->std_fd[STDOUT] = -1;
	return (cmd);
}

t_simple_cmd	*ft_insert_command_and_arguments(t_simple_cmd **cmd,
		t_token *token)
{
	t_args	*tmp;

	tmp = NULL;
	if ((*cmd)->command == NULL)
		(*cmd)->command = ft_strdup(token->value);
	else
	{
		if ((*cmd)->args == NULL)
			(*cmd)->args = ft_create_arg(token->value);
		else
		{
			tmp = ft_create_arg(token->value);
			ft_insert_arg((*cmd)->args, tmp);
		}
	}
	return (*cmd);
}

t_simple_cmd	*ft_create_simple_cmd(t_token **tokens)
{
	t_simple_cmd	*cmd;
	int				r;

	r = 0;
	cmd = ft_init_simple_cmd();
	while ((*tokens)->type != PIPE && (*tokens)->type != SEMI
		&& (*tokens)->type != NEWLINE)
	{
		if ((*tokens)->type == GREAT || (*tokens)->type == DOUBLE_GREAT
			|| (*tokens)->type == LESS || (*tokens)->type == DOUBLE_LESS)
		{
			cmd->redirections = ft_insert_redir(cmd->redirections, tokens, r);
			r++;
		}
		else if ((*tokens)->type == WORD)
		{
			cmd = ft_insert_command_and_arguments(&cmd, *tokens);
			(*tokens) = (*tokens)->next;
		}
		else
			(*tokens) = (*tokens)->next;
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
