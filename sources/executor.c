/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/18 15:13:01 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cmd(t_simple_cmd *cmd)
{
	char	**argv;
	char	**env;

	//argv = convrt_lst_to_argv();
	// err check
	env = convrt_lst_to_argv(NULL); // change
	// err check
	execve(cmd->command, argv, env);
	//err check
	exit(1);
}

void	exec_simpl_cmd(t_simple_cmd *cmd, pid_t *pid_last_cmd)
{
	pid_t	chld_pid;

	redirect(cmd, cmd->redirections);
	if (is_builtin(NULL, cmd->command) == 1)
		{//run_builtin(cmd);
		}
	chld_pid = fork();
	if (chld_pid == -1)
	{
		//err check
	}
	if (chld_pid == 0)
	{
		close_pipes(cmd->pipes, cmd->cmd_count - 1);
		// check existing cmd
		run_cmd(cmd);
	}
	else
	{
		*pid_last_cmd = chld_pid;
	}
	//restrat_fd();
}

void	exec_pipeline(t_pipe_line *pipeline, int cmd_count)
{
	t_simple_cmd	*curr_cmd;
	t_pipe			*pipes;
	int				i;

	pipes = NULL;
	if (cmd_count > 1)
		pipes = init_pipes(cmd_count - 1);
	curr_cmd = pipeline->child;
	i = 0;
	while (i < pipeline->simple_cmd_count)
	{
		curr_cmd->cmd_count = cmd_count;
		curr_cmd->cmd_i = i;
		curr_cmd->pipes = pipes;
		exec_simpl_cmd(curr_cmd, pipeline->pid_last_cmd);
		i++;
	}
	waitpid(*pipeline->pid_last_cmd, pipeline->exit_status, 0);
	while (waitpid(0, 0, 0) != -1)
		{ }
	close_pipes(pipes, cmd_count - 1);
}

int	executer(t_cmd_list *cmd_list)
{
	t_pipe_line	*pipeline;

	pipeline = cmd_list->childs;
	check_create_heredoc(pipeline);
	while (pipeline)
	{
		exec_pipeline(pipeline, pipeline->simple_cmd_count);
		pipeline = pipeline->next;
	}
	unlink_heredoc(pipeline);

	return (0);
}
