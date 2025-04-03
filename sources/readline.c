/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:55:14 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/03 13:24:53 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_close_fd(t_cmd *cmd, t_cmd_tab *cmd_flow, int cmd_i)
{
	if (cmd->infile)
	{
		cmd->infile->fd = open(cmd->infile->path_name, O_RDONLY);
		dup2(cmd->infile->fd, STDIN);
		close(cmd->infile->fd);
	}
	else if (cmd_i != 0)
	{
		dup2(cmd_flow->pipes[cmd_i - 1].pipe[STDIN], STDIN);
	}


	if (cmd->outfile)
	{
		cmd->outfile->fd = open(cmd->outfile->path_name, O_WRONLY);
		dup2(cmd->outfile->fd, STDOUT);
		close(cmd->outfile->fd);
	}
	else if (cmd_flow->cmd_count != 1)
	{
		dup2(cmd_flow->pipes[cmd_i - 1].pipe[STDOUT], STDOUT);
	}

	int i = 0;
	while (i < cmd_flow->cmd_count - 1)
	{
		close(cmd_flow->pipes[i].pipe[STDIN]);
		close(cmd_flow->pipes[i].pipe[STDOUT]);
	}
}

void	run_cmd(t_cmd *cmd, t_cmd_tab *cmd_flow, int cmd_i)
{
	char	*env[0]; // test

	redirect_close_fd(cmd, cmd_flow, cmd_i);
	execve(cmd->cmd_path, cmd->arg_list, env);
	//err check
	exit(1);
}

int	pipex(t_cmd_tab *cmd_flow)
{
	int		i;
	pid_t	chld_pid;
	int		status;

	cmd_flow->cmd_count = 3; //test

	//pipes init
	cmd_flow->pipes = malloc(sizeof(t_pipe) * cmd_flow->cmd_count - 1);
	int i = 0;
	while (i < cmd_flow->cmd_count - 1)
	{
		pipe(cmd_flow->pipes[i].pipe);
		i++;
	}

	i = 0;
	while (i < cmd_flow->cmd_count)
	{
		chld_pid = fork();
		if (chld_pid == -1)
		{
			//err check
		}
		if (chld_pid == 0)
		{
			run_cmd(&cmd_flow->cmds[i], cmd_flow, i);
		}
		else
		{

		}
		i++;
	}
	while (waitpid(0, &status, 0) != -1)
		{ }; // check exit status child
	return (cmd_flow->exit_code);
}

void	read_input(int argc, char *argv[], char *env[])
{
	char	*read_line;

	while (1)
	{
		read_line = readline("minishell$ ");
		add_history(read_line);
		if (ft_strcmp(read_line, "exit") == 0)
			cmd_exit();

		// if (read_line)
		// 	pipex(read_line, env);
		t_cmd_tab cmd_flow;
		pipex(&cmd_flow);

		free(read_line);
	}
}
