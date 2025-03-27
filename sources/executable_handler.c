/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:12:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/03/27 16:20:17 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

/* !!! Figure out how to get rid of this. And why can't I allocate mem in run !!! */
#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

/* !!! No clue 'bout this. Thanks to stackoverflow. What should I do with this? !!! */
extern char **environ;

/* !!! Make it clean. Separate to different functions. Rid of and change forbidden functions !!! */
void	is_executable(const char *name)
{
	char	*cmd_path;
	char	*path_env;
	char	*paths;
	char	*token;
	char	full_path[PATH_MAX];
	char	*argv[2];
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
			paths = ft_strdup(path_env);
/* !!! Can't use strtok? Recreate? !!! */
			token = strtok(paths, ":");
			while (token)
			{
/* !!! Same as strtok !!! */
				snprintf(full_path, sizeof(full_path), "%s/%s", token, name);
				if (access(full_path, X_OK) == 0)
				{
					cmd_path = ft_strdup(full_path);
					break ;
				}
/* !!! USE OF FORBIDEN FUNCTION !!! */
				token = strtok(NULL, ":");
			}
			free(paths);
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
		execve(cmd_path, argv, environ);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
	free(cmd_path);
}
