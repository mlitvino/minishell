/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:45:53 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/23 13:26:15 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == RE_DOUBLE_LESS)
			unlink(redir->file_name);

		free(redir->file_name);
		redir->file_name = NULL;

		// now heredoc is not handled
		// free(redir->delim);
		// redir->delim = NULL;

		redir = redir->next;
	}
}

void	free_args(t_args *args)
{
	while (args)
	{
		free(args->value);
		args->value = NULL;
		args = args->next;
	}
}

void	free_cmd_list(t_cmd_list *cmd_list)
{
	t_pipe_line		*pipeline;
	t_simple_cmd	*cmd;

	pipeline = cmd_list->childs;
	while (pipeline)
	{
		cmd = pipeline->child;
		while (cmd)
		{
			free_args(cmd->args);
			free_redir(cmd->redirections);

			close(cmd->std_fd[STDIN]);
			close(cmd->std_fd[STDOUT]);

			free(cmd->command);
			cmd->command = NULL;

			free(cmd->pathname);
			cmd->pathname = NULL;

			close_pipes(cmd->pipes, cmd->cmd_count - 1);
			cmd = cmd->next;
		}
		pipeline = pipeline->next;
	}
	free(cmd_list);
}

int	clean_all(t_data *data, int	exit_code, char *err_message)
{
	free(data->read_line);
	data->read_line = NULL;

	ft_lstclear(&data->local_vars, free);
	data->local_vars = NULL;

	ft_lstclear(&data->env, free);
	data->env = NULL;

	free(data->builtin_arr);
	data->builtin_arr = NULL;

	if (data->cmd_list)
	{
		free_cmd_list(data->cmd_list);
	}
	if (err_message)
		ft_putstr_fd(err_message, 2);
	//else if (!err_message)
	//	ft_putstr_fd("minishell: malloc failed\n", 2);

	if (!err_message && exit_code == FAILURE && errno == SUCCESS)
	{
		// malloc message
	}

	exit(exit_code);
}
