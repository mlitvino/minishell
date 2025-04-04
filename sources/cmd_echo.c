/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:35:42 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/04 12:17:15 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_new_line(char *option)
{
	if (!option)
		return (1);
	if (ft_strncmp(option, "-n", 2) != 0)
		return (1);
	option += 2;
	while (*option)
	{
		if (*option != 'n')
			return (1);
		option++;
	}
	return (0);
}

void	cmd_echo(char **argv)
{
	// NUL argv check?

	if (is_new_line(*(argv + 1)) == 1)
	{
		argv++;
		while (*argv)
		{
			printf("%s", *argv);
			if (*(argv + 1))
				printf(" ");
			argv++;
		}
		printf("\n");
	}
	else
	{
		argv += 2;
		while (*argv)
		{
			printf("%s", *argv);
			if (*(argv + 1))
				printf(" ");
			argv++;
		}
	}
}
