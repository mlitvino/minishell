/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_clean_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:25:09 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/15 14:25:31 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(t_redir *redir)
{
	void	*prev_ptr;

	while (redir)
	{
		free(redir->file_name);
		redir->file_name = NULL;
		free(redir->delim);
		redir->delim = NULL;
		prev_ptr = redir;
		redir = redir->next;
		free(prev_ptr);
	}
}

void	free_args(t_args *args)
{
	void	*prev_ptr;

	while (args)
	{
		free(args->value);
		args->value = NULL;
		prev_ptr = args;
		args = args->next;
		free(prev_ptr);
	}
}

void	free_cmds(t_simple_cmd *cmd)
{
	void	*prev_ptr;

	while (cmd)
	{
		free_args(cmd->args);
		free_redir(cmd->redirections);
		if (cmd->std_fd[STDIN] != -1)
			close(cmd->std_fd[STDIN]);
		if (cmd->std_fd[STDOUT] != -1)
			close(cmd->std_fd[STDOUT]);
		free(cmd->command);
		cmd->command = NULL;
		free(cmd->pathname);
		cmd->pathname = NULL;
		prev_ptr = cmd;
		cmd = cmd->next;
		free(prev_ptr);
	}
}
