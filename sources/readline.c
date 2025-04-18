/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:55:14 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/18 16:59:47 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_builtin *arr, char	*cmd_name)
{
	int	i;

	i = 0;
	while (arr && arr[i].name)
	{
		if (ft_strcmp(arr[i].name, cmd_name) == 0)
			return (i);
		i++;
	}
	return (-1);
}

t_builtin	*set_builtins(t_data *data)
{
	t_builtin	*arr;
	int			i;

	arr = malloc(sizeof(t_builtin) * 8);
	// nul check
	arr[0].name = CD_STR;
	arr[0].func = cmd_cd;
	arr[1].name = ECHO_STR;
	arr[1].func = cmd_echo;
	arr[2].name = ENV_STR;
	arr[2].func = cmd_env;
	arr[3].name = EXIT_STR;
	arr[3].func = cmd_exit;
	arr[4].name = EXPORT_STR;
	arr[4].func = cmd_export;
	arr[5].name = PWD_STR;
	arr[5].func = cmd_pwd;
	arr[6].name = UNSET_STR;
	arr[6].func = cmd_unset;
	arr[6].name = NULL;
	arr[6].func = NULL;

	return (arr);
}

void	read_input(int argc, char *argv[], char *env[])
{
	t_data data;

	data.builtin_arr = set_builtins(&data);
	cpy_env(env, &data);
	while (1)
	{
		data.read_line = readline("minishell$ ");
		if (!data.read_line)
		{
			printf("EOF\n");
			exit(0);
		}

		add_history(data.read_line);

		t_token *token = ft_lexer(data.read_line);
		show_token(token);

		int status = 0;
		data.cmd_list = ft_parser(token, &status);
		show_cmd_list(data.cmd_list);

		printf("\nEXECUTOR:\n"); // del

		executor(&data, data.cmd_list);

		free(data.read_line);
	}
}
