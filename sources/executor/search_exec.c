/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:12:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/08 13:00:00 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_access(t_simple_cmd *cmd)
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
		print_strs_fd("minishell: ", cmd->pathname, ": Is a directory\n", 2);
		cmd->exit_code = CMD_NOT_EXEC;
		close(file_fd);
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
	char	**orig_tab;

	orig_tab = path_tab;
	while (*path_tab)
	{
		join_pathname(data, cmd, *path_tab);
		if (access(cmd->pathname, F_OK) == SUCCESS)
		{
			if (check_access(cmd) == SUCCESS)
				return (SUCCESS);
			else
			{
				free_argv(orig_tab);
				clean_all(data, cmd->exit_code, NULL);
			}
		}
		free(cmd->pathname);
		cmd->pathname = NULL;
		path_tab++;
	}
	return (CMD_NOT_FOUND);
}

int	search_exec(t_data *data, t_simple_cmd *cmd)
{
	char	**path_tab;

	(void)data;
	if (ft_strchr(cmd->command, '/') != NULL)
	{
		cmd->pathname = ft_strdup(cmd->command);
		if (!cmd->pathname)
			perror("minishell: malloc");
		if (check_access(cmd) != SUCCESS)
			clean_all(data, cmd->exit_code, NULL);
	}
	else
	{
		path_tab = create_path_tab(data);
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
