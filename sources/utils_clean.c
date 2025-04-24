/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:45:53 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/24 18:34:33 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
	{
		free(argv[i]);
		argv[i] = NULL;
		i++;
	}
	free(argv);
	argv = NULL;
}

void	free_redir(t_redir *redir)
{
	void	*prev_ptr;

	while (redir)
	{
		if (redir->type == RE_DOUBLE_LESS)
			unlink(redir->file_name);

		free(redir->file_name);
		redir->file_name = NULL;

		free(redir->delim);
		redir->delim = NULL;

		prev_ptr = redir;
		redir = redir->next;
		free(prev_ptr);
	}
}

void	free_args(t_args *args)
{
	void	*prev_ptr;

	while (args)
	{
		free(args->value);
		args->value = NULL;

		prev_ptr = args;
		args = args->next;
		free(prev_ptr);
	}
}

void	free_cmd_list(t_cmd_list *cmd_list)
{
	t_pipe_line		*pipeline;
	t_simple_cmd	*cmd;
	void			*prev_ptr;

	pipeline = cmd_list->childs;
	while (pipeline)
	{
		cmd = pipeline->child;
		while (cmd)
		{
			free_args(cmd->args);
			free_redir(cmd->redirections);

			if (cmd->std_fd[STDIN] != -1)
				close(cmd->std_fd[STDIN]);
			if (cmd->std_fd[STDOUT] != -1)
				close(cmd->std_fd[STDOUT]);

			free(cmd->command);
			cmd->command = NULL;

			free(cmd->pathname);
			cmd->pathname = NULL;

			close_pipes(cmd->pipes, cmd->cmd_count - 1);

			prev_ptr = cmd;
			cmd = cmd->next;
			free(prev_ptr);
		}
		prev_ptr = pipeline;
		pipeline = pipeline->next;
		free(prev_ptr);
	}
	free(cmd_list);
}

int	clean_all(t_data *data, int	exit_code, char *err_message)
{
	while (waitpid(0, 0, 0) != -1)
		{ }

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
	exit(exit_code);
}
