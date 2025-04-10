/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:55:14 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/11 15:09:02 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_builtin(t_data *data, char *read_line)
{
	if (ft_strcmp(read_line, "exit") == 0)
	{
		cmd_exit(data);
	}

	if (ft_strncmp(read_line, "echo", 4) == 0)
	{
		char	**tab = ft_split(read_line, ' ');
		cmd_echo(tab);

		for (int i = 0; tab[i]; i++)
			free(tab[i]);
		free(tab);
	}

	if (ft_strncmp(read_line, "env", 3) == 0)
	{
		cmd_env(data->env);
	}

	if (ft_strncmp(read_line, "export", 6) == 0)
	{
		char *export_arg = ft_substr(read_line, 7, ft_strlen(read_line));
		cmd_export(data, export_arg);

		free(export_arg);
	}

	if (ft_strncmp(read_line, "pwd", 3) == 0)
	{
		cmd_pwd(data);
	}

	if (ft_strncmp(read_line, "cd", 2) == 0)
	{
		char	**tab = ft_split(read_line, ' ');
		cmd_cd(data, tab[1]);

		for (int i = 0; tab[i]; i++)
			free(tab[i]);
		free(tab);
	}

	if (ft_strncmp(read_line, "hd", 2) == 0)
	{
		t_file *infile = malloc(sizeof(t_file));
		char	**tab = ft_split(read_line, ' ');

		create_temp_hd(data, infile);
		printf("HERE\n"); // DEL
		if (fork() == 0)
		{
			heredoc(data, tab, infile);
			exit(0);
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			while (waitpid(0, 0, 0) != -1)
				{ }
			init_sigs(data);
			unlink(infile->path_name);
		}

		for (int i = 0; tab[i]; i++)
			free(tab[i]);
		free(tab);
		free(infile);
	}
}

void	read_input(int argc, char *argv[], char *env[])
{
	t_data	data;

	data.local_vars = NULL;
	init_sigs(&data);
	cpy_env(env, &data);
	while (1)
	{
		data.read_line = readline("minishell$ ");
		if (!data.read_line)
		{
			printf("exit\n");
			cmd_exit(&data);
		}

		add_history(data.read_line);

		is_builtin(&data, data.read_line);

		free(data.read_line);
	}
}
