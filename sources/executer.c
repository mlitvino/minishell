/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/16 16:35:33 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_close_fd(t_cmd *cmd, t_cmd_tab *cmd_flow, int cmd_i)
{
	if (cmd->infile)
	{
		cmd->infile->fd = open(cmd->infile->path_name, O_RDONLY);
		dup2(cmd->infile->fd, STDIN);
		close(cmd->infile->fd);
	}
	else if (cmd_i != 0)
	{
		dup2(cmd_flow->pipes[cmd_i - 1].pipe[STDIN], STDIN);
	}


	if (cmd->outfile)
	{
		cmd->outfile->fd = open(cmd->outfile->path_name, O_WRONLY);
		dup2(cmd->outfile->fd, STDOUT);
		close(cmd->outfile->fd);
	}
	else if (cmd_flow->cmd_count != 1)
	{
		dup2(cmd_flow->pipes[cmd_i - 1].pipe[STDOUT], STDOUT);
	}

	int i = 0;
	while (i < cmd_flow->cmd_count - 1)
	{
		close(cmd_flow->pipes[i].pipe[STDIN]);
		close(cmd_flow->pipes[i].pipe[STDOUT]);
	}
}

void	run_cmd(t_cmd *cmd, t_cmd_tab *cmd_flow, int cmd_i)
{
	char	*env[0]; // test

	redirect_close_fd(cmd, cmd_flow, cmd_i);
	execve(cmd->cmd_path, cmd->arg_list, env);
	//err check
	exit(1);
}

void	exec_simpl_cmd(t_simple_cmd *cmd)
{
	pid_t	chld_pid;

	// opens files
	// check existing cmd
	// execve builtin
	chld_pid = fork();
	if (chld_pid == -1)
	{
		//err check
	}
	if (chld_pid == 0)
	{
		run_command();
	}
	else
	{
	}
}

void	exec_pipeline(t_pipe_line *pipeline)
{
	t_simple_cmd	*curr_cmd;
	int				*pipes;
	int				i;

	// pipes = pipes_init
	curr_cmd = pipeline->child;
	i = 0;
	while (i < pipeline->simple_cmd_count)
	{
		curr_cmd->cmd_i = i;
		curr_cmd->pipes = pipes;
		exec_simpl_cmd(curr_cmd);
		i++;
	}
	// wait
	// close pipes
}

int	executer(t_cmd_list *cmd_list)
{
	t_pipe_line	*pipeline;

	pipeline = cmd_list->childs;
	check_create_heredoc(pipeline);
	while (pipeline)
	{
		exec_pipeline(pipeline);
		pipeline = pipeline->next;
	}
	unlink_heredoc(pipeline);
}
