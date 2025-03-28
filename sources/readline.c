/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:55:14 by mlitvino          #+#    #+#             */
/*   Updated: 2025/03/28 22:29:14 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_input(int argc, char *argv[], char *env[])
{
	char	*read_line;

	while (1)
	{
		read_line = readline("minishell$ ");
		add_history(read_line);
		if (ft_strcmp(read_line, "exit") == 0)
			cmd_exit();

		if (read_line)
			pipex(read_line);

		free(read_line);
	}
}
