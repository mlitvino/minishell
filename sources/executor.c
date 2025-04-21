/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/21 19:14:24 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cmd(t_simple_cmd *cmd)
{
	char	**argv;
	char	**env;

	argv = convrt_args_to_argv(cmd->args, cmd->command);
	// err check
	env = convrt_lst_to_argv(cmd->env); // change
	// err check

	execve(cmd->pathname, argv, env);
	//err check
	perror("execve");
	exit(1);
}


int	check_path_dirs(t_data *data, t_simple_cmd *cmd)
{
		char	*path_value;
		char	**path_tab;

		path_value = expand_var(data, "PATH=");
		path_tab = ft_split(path_value, ':');
		if (!path_tab)
		{
			perror("malloc");
			return (1);
		}
		int i = 0;
		while (path_tab[i])
		{
			cmd->pathname = ft_strjoin("/", cmd->command);
			if (!cmd->pathname)
			{
				perror("malloc");
				return (1);
			}
			cmd->pathname = ft_strjoin(path_tab[i], cmd->pathname);
			if (!cmd->pathname)
			{
				perror("malloc");
				return (1);
			}
			if (access(cmd->pathname, F_OK) == 0)
			{
				if (access(cmd->pathname, X_OK) == 0)
					return (0);
				else
					return (1);
			}
			i++;
		}
		return (127);
}

int	search_exec(t_data *data, t_simple_cmd *cmd)
{
	if (ft_strchr(cmd->command, '/') != NULL)
	{

		if (access(cmd->command, X_OK) != 0)
			cmd->exit_code = 1;
	}
	else
	{
		cmd->exit_code = check_path_dirs(data, cmd);
		if (cmd->exit_code == 127)
		{
			ft_putstr_fd(cmd->command, 2);
			ft_putstr_fd(": command not found\n", 2);
		}
	}
	return (cmd->exit_code);
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
				run_cmd(cmd);
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
		pipes = init_pipes(cmd_count);
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
	check_create_heredoc(pipeline);
	while (pipeline)
	{
		exec_pipeline(data, pipeline, pipeline->simple_cmd_count);
		pipeline = pipeline->next;
	}
	unlink_heredoc(pipeline);
	return (0);
}
