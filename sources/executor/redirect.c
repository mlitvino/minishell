/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:57:15 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/22 13:41:19 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restart_fd(t_data *data, t_simple_cmd *cmd)
{
	int	exit_code;

	exit_code = 0;
	exit_code |= dup2(cmd->std_fd[STDIN], STDIN);
	exit_code |= dup2(cmd->std_fd[STDOUT], STDOUT);
	if (exit_code == -1)
	{
		perror("minishell: restart_fd: dup2");
		clean_all(data, FAILURE, NULL);
	}
	close(cmd->std_fd[STDIN]);
	close(cmd->std_fd[STDOUT]);
	cmd->std_fd[STDIN] = -1;
	cmd->std_fd[STDOUT] = -1;
}

int	open_redir(t_redir *redir)
{
	if (redir->type == RE_DOUBLE_LESS || redir->type == RE_LESS)
		redir->fd = open(redir->file_name, O_RDONLY);
	else if (redir->type == RE_GREAT)
		redir->fd = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == RE_DOUBLE_GREAT)
		redir->fd = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->file_name);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	redirect_files(t_redir *redir)
{
	int	exit_code;

	exit_code = SUCCESS;
	if ((redir->file_name[0] == '\0' && redir->inside_quotes == 0)
		|| ft_strchr(redir->file_name, ' '))
		return (ft_putstr_fd("minishell: amgiguous redirect\n", 2), FAILURE);
	if (open_redir(redir) == FAILURE)
		return (FAILURE);
	if (redir->type == RE_LESS || redir->type == RE_DOUBLE_LESS)
		exit_code |= dup2(redir->fd, STDIN);
	else if (redir->type == RE_GREAT || redir->type == RE_DOUBLE_GREAT)
		exit_code |= dup2(redir->fd, STDOUT);
	close(redir->fd);
	if (exit_code == -1)
		return (perror("minishell: dup2"), FAILURE);
	return (SUCCESS);
}

void	redirect_pipes(t_data *data, t_simple_cmd *cmd)
{
	if (cmd->cmd_i != 0)
	{
		if (dup2(data->pipes[cmd->cmd_i - 1].pipe[STDIN], STDIN) == -1)
			perror("minishell: dup2");
		else
		{
			close(data->pipes[cmd->cmd_i - 1].pipe[STDIN]);
			data->pipes[cmd->cmd_i - 1].pipe[STDIN] = -1;
		}
	}
	if (cmd->cmd_i != cmd->cmd_count - 1)
	{
		if (dup2(data->pipes[cmd->cmd_i].pipe[STDOUT], STDOUT) == -1)
			perror("minishell: dup2");
		else
		{
			close(data->pipes[cmd->cmd_i].pipe[STDOUT]);
			data->pipes[cmd->cmd_i].pipe[STDOUT] = -1;
		}
	}
}

int	redirect(t_data *data, t_simple_cmd *cmd, t_redir *redirs)
{
	cmd->std_fd[STDIN] = dup(STDIN);
	cmd->std_fd[STDOUT] = dup(STDOUT);
	if (cmd->std_fd[STDIN] == -1 || cmd->std_fd[STDOUT] == -1)
	{
		perror("minishell: redirect: dup");
		clean_all(data, FAILURE, NULL);
	}
	redirect_pipes(data, cmd);
	while (redirs)
	{
		if (redirect_files(redirs) != SUCCESS)
			return (FAILURE);
		redirs = redirs->next;
	}
	return (SUCCESS);
}
