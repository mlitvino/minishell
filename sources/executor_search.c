/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_search.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:12:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/22 12:38:35 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_path_dirs(t_data *data, t_simple_cmd *cmd)
{
		char	*path_value;
		char	**path_tab;

		path_value = expand_var(data, "PATH=");
		path_tab = ft_split(path_value, ':');
		if (!path_tab)
		{
			perror("malloc");
			return (1);
		}
		int i = 0;
		while (path_tab[i])
		{
			cmd->pathname = ft_strjoin("/", cmd->command);
			if (!cmd->pathname)
			{
				perror("malloc");
				return (1);
			}
			cmd->pathname = ft_strjoin(path_tab[i], cmd->pathname);
			if (!cmd->pathname)
			{
				perror("malloc");
				return (1);
			}
			if (access(cmd->pathname, F_OK) == 0)
			{
				if (access(cmd->pathname, X_OK) == 0)
					return (0);
				else
					return (1);
			}
			i++;
		}
		return (127);
}

int	search_exec(t_data *data, t_simple_cmd *cmd)
{
	if (ft_strchr(cmd->command, '/') != NULL)
	{

		if (access(cmd->command, X_OK) != 0)
			cmd->exit_code = 1;
	}
	else
	{
		cmd->exit_code = check_path_dirs(data, cmd);
		if (cmd->exit_code == 127)
		{
			ft_putstr_fd(cmd->command, 2);
			ft_putstr_fd(": command not found\n", 2);
		}
	}
	return (cmd->exit_code);
}
