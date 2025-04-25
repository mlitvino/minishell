/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:29:45 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/25 15:59:12 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	exit_atoi(const char *str)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	if (str == NULL)
		return (0);
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign *= -1;
	if (!(*str >= '0' && *str <= '9'))
		return (-1);
	while (*str >= '0' && *str <= '9')
		res = res * 10 + (*str++ - '0');
	while (ft_isspace(*str))
		str++;
	if (*str)
		return (-1);
	return (res * sign);
}

int	cmd_exit(t_data *data, t_args *args)
{
	int	exit_code;

	exit_code = SUCCESS;
	ft_putstr_fd("exit\n", 1); // change to STDERR later
	if (!args)
		clean_all(data, exit_code, NULL);
	else if (args)
	{
		exit_code = (int)exit_atoi(args->value);
		if (exit_code == -1)
		{
			ft_putstr_fd("bash: exit: ", 2);
			ft_putstr_fd(args->value, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			clean_all(data, MISUSE, NULL);
		}
		exit_code = (unsigned char)exit_code;
	}
	if (args->next)
	{
		exit_code = FAILURE;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	}
	return (exit_code);
}
