/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:20:23 by mlitvino, t       #+#    #+#             */
/*   Updated: 2025/04/27 21:04:55 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *env[])
{
	t_data	data;
	t_token	*tokens_list;

	init_data(&data, env);
	while (1)
	{
		data.read_line = readline("minishell$ ");
		if (!data.read_line)
			cmd_exit(&data, NULL);
		add_history(data.read_line);
		tokens_list = ft_lexer(data.read_line);
		show_token(tokens_list); // print

		int status = 0;
		data.cmd_list = ft_parser(tokens_list, &status);
		show_cmd_list(data.cmd_list); //print

		printf("\nEXECUTOR:\n"); // print
		if (data.cmd_list && data.cmd_list->childs)
			executor(&data, data.cmd_list);

		//printf("\nCLEAN\n"); // print
		free_cmd_list(&data, data.cmd_list);
		data.cmd_list = NULL;
		free(data.read_line);
		data.read_line = NULL;
	}
	return (SUCCESS);
}
