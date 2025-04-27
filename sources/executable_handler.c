/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:12:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/28 01:16:05 by alfokin          ###   ########.fr       */
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
		return;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return;
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

static int	is_builtin_cmd(char *name)
{
	return (!ft_strcmp(name, EXIT_STR)
		|| !ft_strcmp(name, ECHO_STR)
		|| !ft_strcmp(name, ENV_STR)
		|| !ft_strcmp(name, EXPORT_STR)
		|| !ft_strcmp(name, PWD_STR)
		|| !ft_strcmp(name, CD_STR)
		|| !ft_strcmp(name, UNSET_STR));
}

static char	**build_argv(t_simple_cmd *cmd)
{
	int		count;
	t_args	*arg;
	char 	**argv;

	count = 1;
	arg = cmd->args;
	while (arg)
	{
		count++;
		arg = arg->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return NULL;
	argv[0] = cmd->command;
	arg = cmd->args;
	for (int i = 1; arg; arg = arg->next, ++i)
		argv[i] = arg->value;
	argv[count] = NULL;
	return argv;
}

static void	run_builtin(t_data *data, t_simple_cmd *cmd)
{
	char	**argv;
	int		i;

	argv = build_argv(cmd);
	if (!argv)
		return ;
	if (!ft_strcmp(cmd->command, EXIT_STR))
		cmd_exit();
	else if (!ft_strcmp(cmd->command, ECHO_STR))
		cmd_echo(argv);
	else if (!ft_strcmp(cmd->command, ENV_STR))
		cmd_env(data->env);
	else if (!ft_strcmp(cmd->command, EXPORT_STR))
	{
		if (argv[1] == NULL)
			cmd_env(data->env);
		else
		{
			i = 1;
			while (argv[i])
				cmd_export(data, argv[i++]);
		}
	}
	else if (!ft_strcmp(cmd->command, PWD_STR))
		cmd_pwd(data);
	else if (!ft_strcmp(cmd->command, CD_STR))
	{
		if (argv[1] == NULL)
		{
			char *home = getenv("HOME");
			if (home)
				cmd_cd(data, home);
			else
				fprintf(stderr, "minishell: cd: HOME not set\n");
		}
		else if (argv[2] != NULL)
			fprintf(stderr, "minishell: cd: too many arguments\n");
		else
			cmd_cd(data, argv[1]);
	}
	else if (!ft_strcmp(cmd->command, UNSET_STR))
	{
		i = 1;
		while (argv[i])
			cmd_unset(data, argv[i++]);
	}
	free(argv);
}

void	execute_cmd_list(t_data *data, t_cmd_list *cmd_list, char *envp[])
{
	t_pipe_line *pl = cmd_list->childs;
	while (pl)
	{
		t_simple_cmd *cmd = pl->child;
		while (cmd)
		{
			if (cmd->command)
			{
				if (is_builtin_cmd(cmd->command))
					run_builtin(data, cmd);
				else
					is_executable(cmd->command, envp);
			}
			cmd = cmd->next;
		}
		pl = pl->next;
	}
}
