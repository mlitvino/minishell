/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:12:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/17 18:26:03 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

void	is_executable(const char *name, t_data *data)
{
	char	*cmd_path;
	char	*path_env;
	char	*full_path;
	char	**paths_array;
	int		i;

	cmd_path = NULL;
	if (ft_strchr(name, '/'))
		cmd_path = ft_strdup(name);
	else
	{
		path_env = expand_var(data, "PATH");
		if (path_env)
		{
			paths_array = ft_split(path_env, ':');
			if (paths_array)
			{
				i = 0;
				full_path = (char *)malloc(PATH_MAX);
				while (paths_array[i])
				{
					snprintf(full_path, PATH_MAX, "%s/%s", paths_array[i], name);
					if (access(full_path, X_OK) == 0)
					{
						cmd_path = ft_strdup(full_path);
						break;
					}
					i++;
				}
				free(full_path);
				i = 0;
				while (paths_array[i])
				{
					free(paths_array[i]);
					i++;
				}
				free(paths_array);
			}
		}
	}
	if (!cmd_path)
	{
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": command not found\n", 2);
		return ;
	}
}
