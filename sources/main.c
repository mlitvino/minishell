/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:20:23 by mlitvino, t       #+#    #+#             */
/*   Updated: 2025/05/22 13:22:16 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_input(t_data *data)
{
	data->read_line = readline("minishell$ ");
	if (g_signal_received == 1)
	{
		data->exit_var = 128 + SIGINT;
		g_signal_received = 0;
	}
	if (!data->read_line)
		cmd_exit(data, NULL);
	else if (*data->read_line)
		add_history(data->read_line);
}

int	main(int argc, char *argv[], char *env[])
{
	t_data	data;
	t_token	*tokens_list;
	int		check_code;

	(void)argc;
	(void)argv;
	init_data(&data, env);
	while (1)
	{
		get_input(&data);
		tokens_list = ft_lexer(&data, data.read_line, &check_code);
		if (check_code == SUCCESS)
		{
			if (!tokens_list)
				clean_all(&data, FAILURE, "minishell: malloc failed\n");
			data.cmd_list = ft_parser(&data, tokens_list, &data.exit_var);
			if (data.cmd_list && data.cmd_list->childs)
				executor(&data, data.cmd_list);
			free_cmd_list(&data, data.cmd_list);
		}
		free(data.read_line);
	}
	return (SUCCESS);
}
