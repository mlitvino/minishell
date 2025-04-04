/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:55:14 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/04 18:09:13 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_input(int argc, char *argv[], char *env[])
{
	char	*read_line;
	t_data	data;

	while (1)
	{
		read_line = readline("minishell$ ");
		add_history(read_line);

		if (ft_strcmp(read_line, "exit") == 0)
			cmd_exit();

		if (ft_strncmp(read_line, "echo", 4) == 0)
		{
			char	**tab = ft_split(read_line, ' ');
			cmd_echo(tab);
		}

		if (ft_strncmp(read_line, "env", 3) == 0)
		{
			cpy_env(env, &data);
			cmd_env(data.env);
		}

		free(read_line);
	}
}
