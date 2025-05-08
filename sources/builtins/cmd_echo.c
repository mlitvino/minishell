/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:35:42 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/08 13:00:32 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_args	*is_option(t_args *args, int *option)
{
	int	i;

	while (args)
	{
		i = 0;
		if (!args->value[i])
			return (args);
		while (args->value[i])
		{
			if (args->value[i] == '-' && args->value[i + 1] == 'n'
				&& args->value[i + 2] != '-')
			{
				while (args->value[++i] == 'n')
					;
			}
			else
				break ;
		}
		if (!args->value[i])
			*option = SUCCESS;
		else
			return (args);
		args = args->next;
	}
	return (NULL);
}

int	print_args(t_args *args)
{
	while (args)
	{
		if (printf("%s", args->value) < SUCCESS)
			return (FAILURE);
		if (args->next)
			if (printf(" ") < SUCCESS)
				return (FAILURE);
		args = args->next;
	}
	return (SUCCESS);
}

int	cmd_echo(t_data *data, t_args *args)
{
	int	exit_code;
	int	option;

	(void)data;
	exit_code = 0;
	option = FAILURE;
	args = is_option(args, &option);
	if (option == SUCCESS)
	{
		exit_code |= print_args(args);
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	else if (option == FAILURE)
	{
		exit_code |= print_args(args);
		exit_code |= printf("\n");
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
