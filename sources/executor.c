/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/18 15:54:00 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cmd(t_simple_cmd *cmd)
{
	char	**argv;
	char	**env;

	argv = convrt_args_to_argv(cmd->args);
	// err check
	env = convrt_lst_to_argv(cmd->env); // change
	// err check
	execve(cmd->command, argv, env);
	//err check
	exit(1);
}

void	exec_simpl_cmd(t_data *data, t_simple_cmd *cmd, pid_t *pid_last_cmd)
{
	pid_t	chld_pid;
	int		temp;

	redirect(cmd, cmd->redirections);
	temp = is_builtin(cmd->builtin_arr, cmd->command);
	if (temp != -1)
		cmd->builtin_arr[temp].func(data, cmd->args);
	else
	{
		chld_pid = fork();
		if (chld_pid == -1)
		{
			//err check
		}
		if (chld_pid == 0)
		{
			close(cmd->std_fd[STDIN]);
			close(cmd->std_fd[STDOUT]);
			close_pipes(cmd->pipes, cmd->cmd_count - 1);
			// check existing cmd
			run_cmd(cmd);
		}
		else
		{
			*pid_last_cmd = chld_pid;
		}
	}
	restart_fd(cmd);
}

void	exec_pipeline(t_data *data, t_pipe_line *pipeline, int cmd_count)
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
		// find better way
		curr_cmd->env = data->env;
		curr_cmd->local_vars = data->local_vars;
		curr_cmd->builtin_arr = data->builtin_arr;

		curr_cmd->cmd_count = cmd_count;
		curr_cmd->cmd_i = i;
		curr_cmd->pipes = pipes;
		exec_simpl_cmd(data, curr_cmd, pipeline->pid_last_cmd);
		i++;
	}
	waitpid(*pipeline->pid_last_cmd, pipeline->exit_status, 0);
	while (waitpid(0, 0, 0) != -1)
		{ }
	close_pipes(pipes, cmd_count - 1);
}

int	executer(t_data *data, t_cmd_list *cmd_list)
{
	t_pipe_line	*pipeline;

	pipeline = cmd_list->childs;
	check_create_heredoc(pipeline);
	while (pipeline)
	{
		exec_pipeline(data, pipeline, pipeline->simple_cmd_count);
		pipeline = pipeline->next;
	}
	unlink_heredoc(pipeline);

	return (0);
}
