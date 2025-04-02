/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:55:14 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/02 12:26:41 by alfokin          ###   ########.fr       */
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
		free(read_line);
	}
}
