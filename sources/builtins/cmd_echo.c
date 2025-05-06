/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:35:42 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/06 16:47:23 by mlitvino         ###   ########.fr       */
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
				while (args->value[++i] == 'n')
					{ }
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

int	print_args(t_data *data, t_args *args, int newlne)
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
	int		exit_code;
	int		option;

	exit_code = 0;
	option = FAILURE;
	args = is_option(args, &option);
	if (option == SUCCESS)
	{
		exit_code |= print_args(data, args, option);
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	else if (option == FAILURE)
	{
		exit_code |= print_args(data, args, option);
		exit_code |= printf("\n");
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
