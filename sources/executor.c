/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/23 18:47:03 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cmd(t_data *data, t_simple_cmd *cmd)
{
	char	**argv;
	char	**env;

	argv = convrt_args_to_argv(cmd->args, cmd->command);
	env = convrt_lst_to_argv(data->env);
	if (!env || !argv)
	{
		free_argv(env);
		free_argv(argv);
		ft_putstr_fd(cmd->cmd_count, 2);
		clean_all(data, FAILURE, ": malloc failed\n");
	}
	execve(cmd->pathname, argv, env);
	perror("execve");
	clean_all(data, FAILURE, NULL);
}

int	exec_simpl_cmd(t_data *data, t_simple_cmd *cmd)
{
	pid_t	chld_pid;
	int		builtin_i;

	builtin_i = is_builtin(cmd->builtin_arr, cmd->command);
	if (builtin_i != -1)
		cmd->exit_code = cmd->builtin_arr[builtin_i].func(data, cmd->args);
	else
	{
		chld_pid = fork();
		if (chld_pid == -1)
		{
			perror("minishell: fork");
			cmd->exit_code = CRIT_ERR;
			return (CRIT_ERR);
		}
		if (chld_pid == 0)
		{
			close(cmd->std_fd[STDIN]);
			close(cmd->std_fd[STDOUT]);
			close_pipes(cmd->pipes, cmd->cmd_count - 1);
			if (search_exec(data, cmd) == SUCCESS)
				run_cmd(data, cmd);
		}
	}
	return (cmd->exit_code);
}

void	exec_pipeline(t_data *data, t_pipe_line *pipeline, int cmd_count)
{
	t_simple_cmd	*curr_cmd;
	t_pipe			*pipes;
	int				i;

	pipes = NULL;
	if (cmd_count > 1)
		pipes = init_pipes(data, cmd_count - 1);
	curr_cmd = pipeline->child;
	i = 0;
	while (i < pipeline->simple_cmd_count)
	{
		curr_cmd->builtin_arr = data->builtin_arr;
		curr_cmd->cmd_count = cmd_count;
		curr_cmd->cmd_i = i;
		curr_cmd->pipes = pipes;
		curr_cmd->exit_code = 0;
		redirect(data, curr_cmd, curr_cmd->redirections);
		if (exec_simpl_cmd(data, curr_cmd) == CRIT_ERR)
			break ;
		restart_fd(data, curr_cmd);
		curr_cmd = curr_cmd->next;
		i++;
	}
	while (waitpid(0, 0, 0) != -1)
		{ }
	close_pipes(pipes, cmd_count - 1);
	if (curr_cmd->exit_code == CRIT_ERR)
		clean_all(data, FAILURE, NULL);
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
