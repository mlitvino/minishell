/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:55:14 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/28 00:41:44 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_builtin(t_data *data, char *read_line)
{
	char **args;
	int i;

	if (!read_line || *read_line == '\0')
		return ;
	args = ft_split(read_line, ' ');
	if (args == NULL)
		return ;
	if (args[0] == NULL) // пустая строка после split
	{
		free(args);
		return ;
	}
	if (ft_strcmp(args[0], EXIT_STR) == 0)
	{
		i = 0;
		while (args[i])
			free(args[i++]);
		free(args);
		cmd_exit();
		return ;
	}
	else if (ft_strcmp(args[0], ECHO_STR) == 0)
		cmd_echo(args);
	else if (ft_strcmp(args[0], ENV_STR) == 0)
		cmd_env(data->env);
	else if (ft_strcmp(args[0], EXPORT_STR) == 0)
	{
		if (args[1] == NULL)
			cmd_env(data->env);
		else
		{
			i = 0;
			while (args[++i])
				cmd_export(data, args[i]);
		}
	}
	else if (ft_strcmp(args[0], PWD_STR) == 0)
	{
		cmd_pwd(data);
	}
	else if (ft_strcmp(args[0], CD_STR) == 0)
	{
		if (args[1] == NULL)
		{
			char *home = getenv("HOME");
			if (home)
				cmd_cd(data, home);
			else
				fprintf(stderr, "minishell: cd: HOME not set\n");
		}
		else if (args[2] != NULL)
		{
			fprintf(stderr, "minishell: cd: too many arguments\n");
		}
		else
		{
			cmd_cd(data, args[1]);
		}
	}
	else if (ft_strcmp(args[0], UNSET_STR) == 0)
	{
		i = 1;
		while (args[i])
		{
			cmd_unset(data, args[i]);
			i++;
		}
	}
	else
		printf("Command not found: %s\n", args[0]);
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	read_input(int argc, char *argv[], char *env[])
{
	char		*read_line;
	int			status;
	t_data		data;
	t_token		*token;
	t_cmd_list	*cmd_list;

	cpy_env(env, &data);
	while (1)
	{
		read_line = readline("minishell$ ");
		add_history(read_line);
		token = ft_lexer(read_line);
		status = 0;
		cmd_list = ft_parser(token, &status);
		if (cmd_list)
		{
			execute_cmd_list(&data, cmd_list, env);
			ft_destroy_ast(cmd_list);
		}
		else
			is_builtin(&data, read_line);
		ft_destroy_token_list(token);
		free(read_line);
	}
}
