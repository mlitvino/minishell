/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/06 17:53:17 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_cmd(t_data *data, t_simple_cmd *cmd, int builtin_i)
{
	char	**argv;
	char	**env;
	int		exit_code;

	if (builtin_i != -1)
	{
		exit_code = cmd->builtin_arr[builtin_i].func(data, cmd->args);
		clean_all(data, exit_code, NULL);
	}
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
	int	builtin_i;

	if (!cmd->command[0] && cmd->inside_quotes == 0)
		return (SUCCESS);
	builtin_i = is_builtin(cmd->builtin_arr, cmd->command);
	if (builtin_i == -1 || cmd->cmd_count > 1)
	{
		cmd->cmd_pid = fork();
		if (cmd->cmd_pid == -1)
			return (perror("minishell: fork"), FAILURE);
		if (cmd->cmd_pid == 0)
		{
			close(cmd->std_fd[STDIN]);
			cmd->std_fd[STDIN] = -1;
			close(cmd->std_fd[STDOUT]);
			cmd->std_fd[STDIN] = -1;
			close_pipes(data, cmd->cmd_count - 1);
			if (builtin_i != -1 || search_exec(data, cmd) == SUCCESS)
				execve_cmd(data, cmd, builtin_i);
		}
	}
	else
		cmd->exit_code = cmd->builtin_arr[builtin_i].func(data, cmd->args);
	return (SUCCESS);
}

void	exec_pipeline(t_data *data, t_pipe_line *pipeline, int cmd_count)
{
	t_simple_cmd	*curr_cmd;
	int				i;

	init_pipes(data, cmd_count - 1);
	curr_cmd = pipeline->child;
	i = 0;
	while (i < pipeline->simple_cmd_count)
	{
		check_cmd(data, curr_cmd, cmd_count, i);
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
	int			exit_code;

	pipeline = cmd_list->childs;
	exit_code = map_heredoc(data, check_create_heredoc);
	while (exit_code != TERM_SIGINT && pipeline)
	{
		exec_pipeline(data, pipeline, pipeline->simple_cmd_count);
		pipeline = pipeline->next;
	}
	if (exit_code == TERM_SIGINT)
		data->exit_var = TERM_SIGINT;
	map_heredoc(data, unlink_heredoc);
	return (SUCCESS);
}
