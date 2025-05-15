/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:29:45 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/15 14:23:33 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_err(t_data *data)
{
	ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
	clean_all(data, MISUSE, NULL);
}

static unsigned long long	get_res(t_data *data, char **str_ptr, int sign)
{
	unsigned long long	threshold;
	int					digit;
	unsigned long long	res;
	char				*str;

	res = 0;
	str = *str_ptr;
	if (sign == 1)
		threshold = LLONG_MAX;
	else
		threshold = (unsigned long long)LLONG_MAX + 1;
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (res > threshold / 10 || (res == threshold / 10
				&& (unsigned long long)digit > threshold % 10))
			exit_err(data);
		res = res * 10 + digit;
		str++;
	}
	*str_ptr = str;
	return (res);
}

char	exit_atoi(t_data *data, char *str)
{
	unsigned long long	res;
	int					sign;

	res = 0;
	sign = 1;
	if (str == NULL)
		return (0);
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!(*str >= '0' && *str <= '9'))
		exit_err(data);
	res = get_res(data, &str, sign);
	while (ft_isspace(*str))
		str++;
	if (*str)
		exit_err(data);
	return ((long long)res * sign);
}

int	cmd_exit(t_data *data, t_args *args)
{
	int	exit_code;

	exit_code = SUCCESS;
	ft_putstr_fd("exit\n", 2);
	if (!args)
		clean_all(data, data->exit_var, NULL);
	else if (args)
	{
		exit_code = (unsigned char)exit_atoi(data, args->value);
	}
	if (args->next)
	{
		exit_code = FAILURE;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	}
	clean_all(data, exit_code, NULL);
	return (SUCCESS);
}
