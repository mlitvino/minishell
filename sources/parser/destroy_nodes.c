/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_nodes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:56:18 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/14 00:18:39 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_destroy_redir(t_redir *redis)
{
	t_redir	*current_redi;
	t_redir	*parent_redi;

	parent_redi = redis;
	while (parent_redi)
	{
		current_redi = parent_redi;
		parent_redi = parent_redi->next;
		free(current_redi->file_name);
		free(current_redi);
	}
}

void	ft_destroy_simple(t_simple_cmd *cmd)
{
	t_args	*head;
	t_args	*current_arg;

	head = NULL;
	current_arg = NULL;
	if (cmd->command)
		free(cmd->command);
	head = cmd->args;
	while (head != NULL)
	{
		current_arg = head;
		head = head->next;
		free(current_arg->value);
		current_arg->value = NULL;
		free(current_arg);
	}
	current_arg = NULL;
	if (cmd->redirections)
		ft_destroy_redir(cmd->redirections);
	cmd->redirections = NULL;
	free(cmd);
}

void	ft_destroy_pipe_line(t_pipe_line *pipe_line)
{
	t_simple_cmd	*current_cmd;
	t_simple_cmd	*parent_cmd;

	parent_cmd = pipe_line->child;
	while (parent_cmd)
	{
		current_cmd = parent_cmd;
		parent_cmd = parent_cmd->next;
		ft_destroy_simple(current_cmd);
		current_cmd = NULL;
	}
	free(pipe_line);
	pipe_line = NULL;
}

void	ft_destroy_ast(t_command_list *cmd_list)
{
	t_pipe_line	*current_pipe;
	t_pipe_line	*parent_pipe;

	parent_pipe = cmd_list->childs;
	while (parent_pipe)
	{
		current_pipe = parent_pipe;
		parent_pipe = parent_pipe->next;
		ft_destroy_pipe_line(current_pipe);
		current_pipe = NULL;
	}
	free(cmd_list);
}
