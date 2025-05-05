/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:35:42 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/05 18:08:30 by mlitvino         ###   ########.fr       */
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

int	get_i_newline(char *str)
{
	int	i;
	int	optn_flg;

	i = 0;
	optn_flg = 0;
	while (str[i])
	{
		if (str[i] == '-' && str[i + 1] == 'n')
			while (str[++i] == 'n')
				optn_flg |= 1;
		else if (ft_isspace(str[i]) == 1)
			while (ft_isspace(str[++i]) == 1)
				{ }
		else
			break ;
	}
	if (optn_flg == 1 && !str[i])
		return (-1);
	else if (optn_flg == 1 && str[i])
		return (i);
	return (optn_flg);
}

int	check_split_arg(t_data *data, t_args *args)
{
	t_args	*temp;
	int		newlne;
	int		res;

	res = get_i_newline(args->value);
	temp = args;
	newlne = res;
	if (temp && (newlne != -1 || newlne != 0))
	{
		// trim_extra_optn
		temp = args->next;
		get_i_newline(temp->value);
	}
	return (res);
}

int	cmd_echo(t_data *data, t_args *args)
{
	int		exit_code;
	int		newlne;

	exit_code = 0;
	newlne = 0;
	if (args)
		newlne = check_split_arg(data, args);
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
