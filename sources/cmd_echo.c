/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:35:42 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/03 14:00:56 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_new_line(char *option)
{
	if (ft_strncmp(option, "-n", 2) != 0)
		return (0);
	option += 2;
	while (*option)
	{
		if (*option != 'n')
			return (0);
		option++;
	}
	return (1);
}

void	cmd_echo(char **argv)
{
	// NUL argv check?
	(*argv)++;
	if (is_new_line(argv[1]) == 1)
	{
		while (++(*argv))
			printf("%s", *argv);
	}
	else
	{
		while (*argv)
			ft_putstr_fd((*argv)++, STDOUT);
	}
}
