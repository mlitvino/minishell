/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:37:29 by mlitvino          #+#    #+#             */
/*   Updated: 2024/12/09 12:52:30 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putint(int n, char spec)
{
	int		len;
	long	res;

	len = 0;
	res = n;
	if (spec == 'c')
		return (ft_putchar(n));
	if (spec == 'x' || spec == 'X')
		return (ft_puthex(n, spec));
	if (res < 0)
	{
		len += ft_putchar('-');
		res = -res;
	}
	if (res >= 10)
		len += ft_putint(res / 10, spec);
	len += ft_putchar((res % 10) + '0');
	return (len);
}
