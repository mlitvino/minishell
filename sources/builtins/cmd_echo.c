/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:35:42 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/01 15:49:28 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_new_line(t_data *data, t_args *args)
{
	char	*option;
	char	*new_arg;

	if (!args)
		return (SUCCESS);
	option = args->value;
	if (ft_strncmp(option, "-n", 2) != 0)
		return (SUCCESS);
	option += 2;
	while (*option)
	{
		if (*option != 'n')
			break ;
		option++;
	}
	if ((*option && *option != ' ') || (*option == ' ' && !*(option + 1)))
		return (SUCCESS);
	new_arg = ft_substr(option, 1, ft_strlen(option));
	if (!new_arg)
		clean_all(data, FAILURE, "minishell: echo: malloc failed\n");
	free(args->value);
	args->value = new_arg;
	return (FAILURE);
}

int	print_args(t_data *data, t_args *args, int newlne)
{
	if (args)
	{
		if (printf("%s", args->value) < SUCCESS)
			return (FAILURE);
		if (newlne != FAILURE && args->next)
			if (printf(" ") < SUCCESS)
				return (FAILURE);
		args = args->next;
	}
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
	int	newlne;

	exit_code = 0;
	newlne = is_new_line(data, args);
	if (!args || newlne == SUCCESS)
	{
		exit_code |= print_args(data, args, newlne);
		exit_code |= printf("\n");
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	else
	{
		exit_code |= print_args(data, args, newlne);
		if (exit_code < SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
