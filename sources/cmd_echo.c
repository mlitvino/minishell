/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:35:42 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/18 14:10:43 by mlitvino         ###   ########.fr       */
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

void	cmd_echo(t_data *data, t_args *args)
{
	if (is_new_line(args->value) == 1)
	{
		args = args->next;
		while (args)
		{
			printf("%s", args->value);
			if (args->next)
				printf(" ");
			args = args->next;
		}
		printf("\n");
	}
	else
	{
		args = args->next->next;
		while (args)
		{
			printf("%s", args->value);
			if (args->next)
				printf(" ");
			args = args->next;
		}
	}
}
