/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:57:15 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/22 19:05:07 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe	*init_pipes(int	pipes_count)
{
	t_pipe	*pipes;
	int		i;
	int		p[2];

	pipes = malloc(sizeof(t_pipe) * pipes_count);
	if (!pipes)
	{
		ft_putstr_fd("minishell: malloc failed\n", 2);
		return (NULL);
	}
	i = 0;
	while (i < pipes_count)
	{
		if (pipe(pipes[i].pipe) != 0)
		{
			ft_putstr_fd("minishell: pipes creation failed\n", 2);
			close_pipes(pipes, i);
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

void	restart_fd(t_simple_cmd *cmd)
{
	dup2(cmd->std_fd[STDIN], STDIN);
	dup2(cmd->std_fd[STDOUT], STDOUT);
	close(cmd->std_fd[STDIN]);
	close(cmd->std_fd[STDOUT]);
	cmd->std_fd[STDIN] = -1;
	cmd->std_fd[STDOUT] = -1;
}

void	redirect(t_simple_cmd *cmd, t_redir *redirs)
{
	cmd->std_fd[STDIN] = dup(STDIN);
	cmd->std_fd[STDOUT] = dup(STDOUT);
	if (cmd->cmd_i != 0)
	{
		dup2(cmd->pipes[cmd->cmd_i - 1].pipe[STDIN], STDIN);
	}
	if (cmd->cmd_i != cmd->cmd_count - 1)
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
			redirs->fd = open(redirs->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		if (redirs->type == RE_DOUBLE_GREAT)
		{
			redirs->fd = open(redirs->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		if (redirs->fd == -1)
		{
			// err check
		}


		if (redirs->type == RE_LESS || redirs->type == RE_DOUBLE_LESS)
		{
			dup2(redirs->fd, STDIN);
		}
		else if (redirs->type == RE_GREAT || redirs->type == RE_DOUBLE_GREAT)
		{
			dup2(redirs->fd, STDOUT);
		}
		close(redirs->fd);
		redirs = redirs->next;
	}
}
