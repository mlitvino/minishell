/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:35:42 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/24 17:52:18 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_new_line(char *option)
{
	if (!option)
		return (SUCCESS);
	if (ft_strncmp(option, "-n", 2) != 0)
		return (SUCCESS);
	option += 2;
	while (*option)
	{
		if (*option != 'n')
			return (SUCCESS);
		option++;
	}
	return (FAILURE);
}

int	print_content(t_data *data, char *content)
{
	if (content && ft_strcmp(content, "$?") == 0)
	{
		if (printf("%d", data->exit_var) < SUCCESS)
			return (FAILURE);
	}
	else
	{
		if (printf("%s", content) < SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	print_args(t_data *data, t_args *args)
{
	while (args)
	{
		// if (printf("%s", args->value) < SUCCESS)
		// 	return (FAILURE);
		if (print_content(data, args->value) != SUCCESS)
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

	exit_code = 0;
	if (!args || is_new_line(args->value) == SUCCESS)
	{
		exit_code |= print_args(data, args);
		exit_code |= printf("\n");
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	else
	{
		args = args->next;
		exit_code |= print_args(data, args);
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
