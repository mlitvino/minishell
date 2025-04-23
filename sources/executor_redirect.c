/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:57:15 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/23 18:36:12 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe	*init_pipes(t_data *data, int	pipes_count)
{
	t_pipe	*pipes;
	int		i;
	int		p[2];

	pipes = malloc(sizeof(t_pipe) * pipes_count);
	if (!pipes)
		clean_all(data, FAILURE, "minishell: pipe: malloc failed\n");
	i = 0;
	while (i < pipes_count)
	{
		if (pipe(pipes[i].pipe) != 0)
		{
			close_pipes(pipes, i);
			clean_all(data, FAILURE, "minishell: pipes creation failed\n");
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	close_pipes(t_pipe *pipes, int pipes_count)
{
	int	i;

	i = 0;
	while (pipes && i < pipes_count)
	{
		close(pipes[i].pipe[STDIN]);
		close(pipes[i].pipe[STDOUT]);
		i++;
	}
	free(pipes);
	pipes = NULL;
}

void	restart_fd(t_data *data, t_simple_cmd *cmd)
{
	int	exit_code;

	exit_code = 0;
	exit_code |= dup2(cmd->std_fd[STDIN], STDIN);
	exit_code |= dup2(cmd->std_fd[STDOUT], STDOUT);
	if (exit_code != SUCCESS)
	{
		perror("minishell: dup2");
		clean_all(data, FAILURE, NULL);
	}
	close(cmd->std_fd[STDIN]);
	close(cmd->std_fd[STDOUT]);
	cmd->std_fd[STDIN] = -1;
	cmd->std_fd[STDOUT] = -1;
}

void	redirect_files(t_data *data, t_simple_cmd *cmd, t_redir *redir)
{
	if (redir->type == RE_DOUBLE_LESS || redir->type == RE_LESS)
		redir->fd = open(redir->file_name, O_RDONLY);
	else if (redir->type == RE_GREAT)
		redir->fd = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == RE_DOUBLE_GREAT)
		redir->fd = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->fd == -1)
	{
		perror("minishell: open");
		//clean_all(data, FAILURE, NULL);
	}
	if (redir->type == RE_LESS || redir->type == RE_DOUBLE_LESS)
		cmd->exit_code |= dup2(redir->fd, STDIN);
	else if (redir->type == RE_GREAT || redir->type == RE_DOUBLE_GREAT)
		cmd->exit_code |= dup2(redir->fd, STDOUT);
	if (cmd->exit_code != 0)
	{
		perror("minishell: dup2");
		//clean_all(data, FAILURE, NULL);
	}
}

void	redirect(t_data *data, t_simple_cmd *cmd, t_redir *redirs)
{
	cmd->std_fd[STDIN] = dup(STDIN);
	cmd->std_fd[STDOUT] = dup(STDOUT);
	if (cmd->std_fd[STDIN] != 0 || cmd->std_fd[STDOUT] != 0)
	{
		perror("minishell: dup");
		clean_all(data, FAILURE, NULL);
	}
	if (cmd->cmd_i != 0)
		cmd->exit_code |= dup2(cmd->pipes[cmd->cmd_i - 1].pipe[STDIN], STDIN);
	if (cmd->cmd_i != cmd->cmd_count - 1)
		cmd->exit_code |= dup2(cmd->pipes[cmd->cmd_i].pipe[STDOUT], STDOUT);
	if (cmd->exit_code != 0)
	{
		perror("minishell: dup2");
		//clean_all(data, FAILURE, NULL);
	}
	while (redirs)
	{
		redirect_files(data, cmd, redirs);
		close(redirs->fd);
		redirs = redirs->next;
	}
}
