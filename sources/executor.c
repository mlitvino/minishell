/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/26 18:18:46 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_cmd(t_data *data, t_simple_cmd *cmd)
{
	char	**argv;
	char	**env;

	argv = convrt_args_to_argv(cmd->args, cmd->command);
	env = convrt_lst_to_argv(data->env);
	if (!env || !argv)
	{
		free_argv(env);
		free_argv(argv);
		ft_putstr_fd(cmd->command, 2);
		clean_all(data, FAILURE, ": malloc failed\n");
	}
	execve(cmd->pathname, argv, env);
	perror("execve");
	clean_all(data, FAILURE, NULL);
}

int	exec_simpl_cmd(t_data *data, t_simple_cmd *cmd)
{
	int		builtin_i;

	builtin_i = is_builtin(cmd->builtin_arr, cmd->command);
	if (builtin_i != -1)
		cmd->exit_code = cmd->builtin_arr[builtin_i].func(data, cmd->args);
	else
	{
		cmd->cmd_pid = fork();
		if (cmd->cmd_pid == -1)
		{
			perror("minishell: fork");
			return (FAILURE);
		}
		if (cmd->cmd_pid == 0)
		{
			close(cmd->std_fd[STDIN]);
			cmd->std_fd[STDIN] = -1;
			close(cmd->std_fd[STDOUT]);
			cmd->std_fd[STDIN] = -1;
			close_pipes(data, cmd->cmd_count - 1);
			if (search_exec(data, cmd) == SUCCESS)
				execve_cmd(data, cmd);
		}
	}
	return (SUCCESS);
}

void	exec_pipeline(t_data *data, t_pipe_line *pipeline, int cmd_count)
{
	t_simple_cmd	*curr_cmd;
	int				i;

	if (cmd_count > 1)
		init_pipes(data, cmd_count - 1);
	curr_cmd = pipeline->child;
	i = 0;
	while (i < pipeline->simple_cmd_count)
	{
		curr_cmd->builtin_arr = data->builtin_arr;
		curr_cmd->cmd_count = cmd_count;
		curr_cmd->cmd_i = i;
		curr_cmd->exit_code = redirect(data, curr_cmd, curr_cmd->redirections);
		if (curr_cmd->exit_code == SUCCESS)
			exec_simpl_cmd(data, curr_cmd);
		restart_fd(data, curr_cmd);
		if (curr_cmd->next)
			curr_cmd = curr_cmd->next;
		i++;
	}
	wait_childs(data, curr_cmd);
	close_pipes(data, cmd_count - 1);
}

int	executor(t_data *data, t_cmd_list *cmd_list)
{
	t_pipe_line	*pipeline;

	pipeline = cmd_list->childs;
	if (check_create_heredoc(data, pipeline) != SUCCESS)
		clean_all(data, pipeline->exit_status, "minishell: heredoc failed");
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
