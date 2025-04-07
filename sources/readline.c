/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:55:14 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/07 14:46:31 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_builtin(t_data *data, char *read_line)
{
	if (ft_strcmp(read_line, "exit") == 0)
	{
		cmd_exit();
	}

	if (ft_strncmp(read_line, "echo", 4) == 0)
	{
		char	**tab = ft_split(read_line, ' ');
		cmd_echo(tab);
	}

	if (ft_strncmp(read_line, "env", 3) == 0)
	{
		cmd_env(data->env);
	}

	if (ft_strncmp(read_line, "export", 6) == 0)
	{
		char *export_arg = ft_substr(read_line, 7, ft_strlen(read_line));
		cmd_export(data, export_arg);
	}

	if (ft_strncmp(read_line, "pwd", 3) == 0)
	{
		cmd_pwd(data);
	}

	if (ft_strncmp(read_line, "cd", 2) == 0)
	{
		char	**tab = ft_split(read_line, ' ');
		cmd_cd(data, tab[1]);
	}
}

void	read_input(int argc, char *argv[], char *env[])
{
	char	*read_line;
	t_data	data;

	cpy_env(env, &data);
	while (1)
	{
		read_line = readline("minishell$ ");
		add_history(read_line);

		is_builtin(&data, read_line);

		free(read_line);
	}
}
