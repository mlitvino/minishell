/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_search.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:12:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/26 18:16:19 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_access(t_data *data, t_simple_cmd *cmd)
{
	int	file_fd;

	if (access(cmd->pathname, F_OK) != SUCCESS)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->command);
		cmd->exit_code = CMD_NOT_FOUND;
		return (cmd->exit_code);
	}
	file_fd = open(cmd->pathname, O_RDONLY | __O_DIRECTORY);
	if (file_fd >= 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->pathname, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		cmd->exit_code = CMD_NOT_EXEC;
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
	char	*temp;

		while (*path_tab)
		{
			temp = ft_strjoin("/", cmd->command);
			if (!temp)
				clean_all(data, FAILURE, "minishell: command: malloc failed\n");
			cmd->pathname = ft_strjoin(*path_tab, temp);
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
		cmd->pathname = ft_strdup(cmd->command);
		if (!cmd->pathname)
			perror("minishell: malloc");
		if (!cmd->pathname || check_access(data, cmd) != SUCCESS)
			clean_all(data, cmd->exit_code, NULL);
	}
	else
	{
		path_value = expand_var(data, "PATH=");
		path_tab = ft_split(path_value, ':');
		if (!path_tab)
			clean_all(data, FAILURE, "minishell: command: malloc failed\n");
		cmd->exit_code = check_path_dirs(data, cmd, path_tab);
		free_argv(path_tab);
		if (cmd->exit_code == CMD_NOT_FOUND)
		{
			ft_putstr_fd(cmd->command, 2);
			clean_all(data, cmd->exit_code, ": command not found\n");
		}
	}
	return (SUCCESS);
}
