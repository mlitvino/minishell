/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:12:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/15 00:25:09 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

void	is_executable(const char *name, char *env[])
{
	char	*cmd_path;
	char	*path_env;
	char	*full_path;
	char	*argv[2];
	char	**paths_array;
	int		i;
	int		status;
	pid_t	pid;

	cmd_path = NULL;
	if (ft_strchr(name, '/'))
		cmd_path = ft_strdup(name);
	else
	{
		path_env = getenv("PATH");
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
		printf("Command not found: %s\n", name);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return ;
	}
	else if (pid == 0)
	{
		argv[0] = (char *)name;
		argv[1] = NULL;
		execve(cmd_path, argv, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
	free(cmd_path);
}
