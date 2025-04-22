/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/22 19:12:18 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cmd(t_data *data, t_simple_cmd *cmd)
{
	char	**argv;
	char	**env;

	argv = convrt_args_to_argv(cmd->args, cmd->command);
	// err check
	env = convrt_lst_to_argv(data->env); // change
	// err check

	execve(cmd->pathname, argv, env);
	//err check
	perror("execve");
	exit(1);
}

void	exec_simpl_cmd(t_data *data, t_simple_cmd *cmd, pid_t *pid_last_cmd)
{
	pid_t	chld_pid;
	int		temp;

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
			if (search_exec(data, cmd) == 0)
			{
				run_cmd(data, cmd);
			}
		}
		else
		{

		}
	}
}

void	exec_pipeline(t_data *data, t_pipe_line *pipeline, int cmd_count)
{
	t_simple_cmd	*curr_cmd;
	t_pipe			*pipes;
	int				i;

	pipes = NULL;
	if (cmd_count > 1)
		pipes = init_pipes(cmd_count - 1);
	if (!pipes)
		clean_all(data, FAILURE, NULL);
	curr_cmd = pipeline->child;
	i = 0;
	while (i < pipeline->simple_cmd_count)
	{
		curr_cmd->builtin_arr = data->builtin_arr;
		curr_cmd->cmd_count = cmd_count;
		curr_cmd->cmd_i = i;
		curr_cmd->pipes = pipes;
		curr_cmd->exit_code = 0;

		redirect(curr_cmd, curr_cmd->redirections);
		exec_simpl_cmd(data, curr_cmd, pipeline->pid_last_cmd);
		restart_fd(curr_cmd);


		curr_cmd = curr_cmd->next;
		i++;
	}
	while (waitpid(0, 0, 0) != -1)
		{ }
	close_pipes(pipes, cmd_count - 1);
}

int	executor(t_data *data, t_cmd_list *cmd_list)
{
	t_pipe_line	*pipeline;

	pipeline = cmd_list->childs;
	pipeline->exit_status = check_create_heredoc(data, pipeline);
	if (pipeline->exit_status == FAILURE)
		clean_all(data, FAILURE, "minishell: heredoc failed");
	if (pipeline->exit_status != TERM_SIGINT)
	{
		while (pipeline)
		{
			exec_pipeline(data, pipeline, pipeline->simple_cmd_count);
			pipeline = pipeline->next;
		}
	}
	unlink_heredoc(data, pipeline);
	return (0);
}
