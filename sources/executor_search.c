/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_search.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:12:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/23 19:31:46 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_access(t_data *data, char *pathname)
{
	if (access(pathname, F_OK) != SUCCESS)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(pathname);
		clean_all(data, CMD_NOT_FOUND, NULL);
	}
	if (access(pathname, X_OK) != SUCCESS)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(pathname);
		clean_all(data, CMD_NOT_EXEC, NULL);
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
			check_access(data, cmd->pathname);
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
		check_access(data, cmd->command);
	}
	else
	{
		path_value = expand_var(data, "PATH=");
		path_tab = ft_split(path_value, ':');
		if (!path_tab)
			clean_all(data, FAILURE, "minishell: command: malloc failed\n");
		check_path_dirs(data, cmd, path_tab);
	}
	return (SUCCESS);
}
