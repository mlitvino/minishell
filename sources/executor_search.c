/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_search.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:12:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/24 16:48:02 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_access(t_data *data, t_simple_cmd *cmd)
{
	if (access(cmd->pathname, F_OK) != SUCCESS)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->command);
		cmd->exit_code = CMD_NOT_FOUND;
		return (cmd->exit_code);
	}
	if (access(cmd->pathname, X_OK) != SUCCESS)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->command);
		cmd->exit_code = CMD_NOT_EXEC;
		return (cmd->exit_code);
	}
	return (SUCCESS);
}

int	check_path_dirs(t_data *data, t_simple_cmd *cmd, char **path_tab)
{
		while (*path_tab)
		{
			cmd->pathname = ft_strjoin("/", cmd->command);
			if (!cmd->pathname)
				clean_all(data, FAILURE, "minishell: command: malloc failed\n");
			cmd->pathname = ft_strjoin(*path_tab, cmd->pathname);
			if (!cmd->pathname)
				clean_all(data, FAILURE, "minishell: command: malloc failed\n");
			if (access(cmd->pathname, F_OK) == SUCCESS)
			{
				if (check_access(data, cmd) == SUCCESS)
					return (SUCCESS);
				else
				{
					free_argv(path_tab);
					clean_all(data, cmd->exit_code, NULL);
				}
			}
			path_tab++;
		}
		return (CMD_NOT_FOUND);
}

int	search_exec(t_data *data, t_simple_cmd *cmd)
{
	char	*path_value;
	char	**path_tab;

	if (ft_strchr(cmd->command, '/') != NULL)
	{
		cmd->pathname = cmd->command;
		if (check_access(data, cmd) != SUCCESS)
			clean_all(data, cmd->exit_code, NULL);
	}
	else
	{
		path_value = expand_var(data, "PATH=");
		path_tab = ft_split(path_value, ':');
		if (!path_tab)
			clean_all(data, FAILURE, "minishell: command: malloc failed\n");
		check_path_dirs(data, cmd, path_tab);
		free_argv(path_tab);
	}
	return (SUCCESS);
}
