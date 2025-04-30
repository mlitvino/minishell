/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:35:42 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/30 18:23:12 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_new_line(t_data *data, t_args *args)
{
	char	*option;
	char	*new_arg;

	option = args->value;
	if (!option)
		return (SUCCESS);
	if (ft_strncmp(option, "-n", 2) != 0)
		return (SUCCESS);
	option += 2;
	while (*option)
	{
		if (*option != 'n')
			break ;
		option++;
	}
	if (*option && *option != ' ')
		return (SUCCESS);
	new_arg = ft_substr(option, 1, ft_strlen(option));
	if (!new_arg)
		clean_all(data, FAILURE, "minishell: echo: malloc failed\n");
	args->value = new_arg;
	printf("new_Arg (%s)\n", new_arg); // del
	return (FAILURE);
}

int	print_args(t_data *data, t_args *args)
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

	exit_code = 0;
	if (!args || is_new_line(data, args) == SUCCESS)
	{
		exit_code |= print_args(data, args);
		exit_code |= printf("\n");
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	else
	{
		
		exit_code |= print_args(data, args);
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
