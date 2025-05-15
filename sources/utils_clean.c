/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:45:53 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/15 14:25:29 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_strs_fd(char *s1, char *s2, char *s3, int fd)
{
	ft_putstr_fd(s1, fd);
	ft_putstr_fd(s2, fd);
	ft_putstr_fd(s3, fd);
}

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

void	free_cmd_list(t_data *data, t_cmd_list *cmd_list)
{
	t_pipe_line		*pipeline;
	t_simple_cmd	*cmd;
	void			*prev_ptr;

	if (!cmd_list)
		return ;
	pipeline = cmd_list->childs;
	while (pipeline)
	{
		cmd = pipeline->child;
		close_pipes(data, cmd->cmd_count - 1);
		free_cmds(cmd);
		prev_ptr = pipeline;
		pipeline = pipeline->next;
		free(prev_ptr);
	}
	free(cmd_list);
}

int	clean_all(t_data *data, int exit_code, char *err_message)
{
	while (waitpid(0, 0, 0) != -1)
		;
	map_heredoc(data, unlink_heredoc);
	free(data->read_line);
	data->read_line = NULL;
	ft_lstclear(&data->env, free);
	data->env = NULL;
	free(data->builtin_arr);
	data->builtin_arr = NULL;
	if (data->cmd_list)
	{
		free_cmd_list(data, data->cmd_list);
	}
	if (err_message)
		ft_putstr_fd(err_message, 2);
	exit(exit_code);
}
