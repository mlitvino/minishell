/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:55:14 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/26 19:43:25 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_builtins(t_data *data)
{
	data->builtin_arr = malloc(sizeof(t_builtin) * 8);
	if (!data->builtin_arr)
		clean_all(data, FAILURE, "minishell: malloc failed\n");
	data->builtin_arr[0].name = CD_STR;
	data->builtin_arr[0].func = cmd_cd;
	data->builtin_arr[1].name = ECHO_STR;
	data->builtin_arr[1].func = cmd_echo;
	data->builtin_arr[2].name = ENV_STR;
	data->builtin_arr[2].func = cmd_env;
	data->builtin_arr[3].name = EXIT_STR;
	data->builtin_arr[3].func = cmd_exit;
	data->builtin_arr[4].name = EXPORT_STR;
	data->builtin_arr[4].func = cmd_export;
	data->builtin_arr[5].name = PWD_STR;
	data->builtin_arr[5].func = cmd_pwd;
	data->builtin_arr[6].name = UNSET_STR;
	data->builtin_arr[6].func = cmd_unset;
	data->builtin_arr[7].name = NULL;
	data->builtin_arr[7].func = NULL;
}

void	init_data(t_data *data, char **sys_env)
{
	data->cmd_list = NULL;
	data->local_vars = NULL;
	data->env = NULL;
	data->exit_var = 0;
	data->pipes = NULL;
	init_sigs(data);
	init_builtins(data);
	init_env(sys_env, data);
}
