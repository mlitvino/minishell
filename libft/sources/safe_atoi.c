/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_atoi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:11:44 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/23 17:11:53 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	safe_atoi(const char *str)
{
	long		res;
	int			sign;

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
		return ((long)INT_MAX + 1);
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str++ - '0');
		if (res > INT_MAX || res < INT_MIN)
			return ((long)INT_MAX + 1);
	}
	while (ft_isspace(*str))
		str++;
	if (*str)
		return ((long)INT_MAX + 1);
	return (res * sign);
}
