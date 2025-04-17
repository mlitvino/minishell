/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/17 19:01:26 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe	*init_pipes(int	cmd_count)
{
	t_pipe	*pipes;
	int		pipes_count;
	int		i;

	pipes_count = cmd_count - 1;
	pipes = malloc(sizeof(t_pipe) * pipes_count);
	if (!pipes)
	{
		// null check
	}
	i = 0;
	while (i < pipes_count)
	{
		pipe(&pipes[i]);
		// err check
		i++;
	}
	return (pipes);
}

void	close_pipes(t_pipe *pipes, int pipes_count)
{
	int	i;

	i = 0;
	while (i < pipes_count)
	{
		close(pipes[i].pipe[STDIN]);
		close(pipes[i].pipe[STDOUT]);
		i++;
	}
	free(pipes);
}

void	redirect(t_simple_cmd *cmd, t_redir *redirs)
{
	if (cmd->cmd_i != 0)
	{
		dup2(cmd->pipes[cmd->cmd_i - 1].pipe[STDIN], STDIN);
	}
	if (cmd->cmd_count != 1)
	{
		dup2(cmd->pipes[cmd->cmd_i].pipe[STDOUT], STDOUT);
	}
	while (redirs)
	{
		if (redirs->type == RE_DOUBLE_LESS)
		{
			redirs->fd = open(redirs->file_name, O_RDONLY);
		}
		if (redirs->type == RE_LESS)
		{
			redirs->fd = open(redirs->file_name, O_RDONLY);
		}
		if (redirs->type == RE_GREAT)
		{
			redirs->fd = open(redirs->file_name, O_WRONLY);
		}
		if (redirs->type == RE_DOUBLE_GREAT)
		{
			redirs->fd = open(redirs->file_name, O_WRONLY | O_APPEND);
		}
		if (redirs->fd == -1)
		{
			// err check
		}


		if (redirs->type == RE_LESS || redirs->type == RE_DOUBLE_LESS)
		{
			dup2(redirs->fd, STDIN);
		}
		else if (redirs->type == RE_GREAT|| redirs->type == RE_DOUBLE_GREAT)
		{
			dup2(redirs->fd, STDOUT);
		}
		fclose(redirs->fd);
		redirs->next;
	}
	close_pipes(cmd->pipes, cmd->cmd_count - 1);
}

int	is_built(char *cmd_name) // change
{
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (1);
	return (0);
}

void	run_builtin(t_simple_cmd *cmd)
{

}

char	**convrt_lst_to_argv(t_list *lst)
{
	t_list	*temp;
	char	**argv;
	int		size;
	int		i;

	size = ft_lstsize(lst);
	temp = lst;
	argv = malloc(sizeof(char *) * (size + 1));
	// nul check
	i = 0;
	while (i < size)
	{
		argv[i] = ft_strdup(temp->content);
		if (!argv[i])
			//clean
		temp = temp->next;
		i++;
	}
	return (argv);
}

void	run_cmd(t_simple_cmd *cmd)
{
	char	**argv;
	char	**env;

	//argv = convrt_lst_to_argv();
	// err check
	env = convrt_lst_to_argv(env);
	// err check
	execve(cmd->command, argv, env);
	//err check
	exit(1);
}

void	exec_simpl_cmd(t_simple_cmd *cmd, pid_t *pid_last_cmd)
{
	pid_t	chld_pid;

	redirect(cmd, cmd->redirections);
	if (is_built(cmd->command) == 1)
		run_builtin(cmd);
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
	close_pipes(cmd->pipes, cmd->cmd_count - 1);
	restrat_fd();
}

void	exec_pipeline(t_pipe_line *pipeline, int cmd_count)
{
	t_simple_cmd	*curr_cmd;
	t_pipe			*pipes;
	int				i;

	if (cmd_count > 1)
		pipes = init_pipes(cmd_count - 1);
	curr_cmd = pipeline->child;
	i = 0;
	while (i < pipeline->simple_cmd_count)
	{
		curr_cmd->cmd_count = cmd_count;
		curr_cmd->cmd_i = i;
		curr_cmd->pipes = pipes;
		exec_simpl_cmd(curr_cmd, &pipeline->pid_last_cmd);
		i++;
	}
	waitpid(pipeline->pid_last_cmd, pipeline->exit_status, 0);
	while (waitpid(0, 0, 0) != -1)
		{ }
	if (cmd_count > 1)
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
}
