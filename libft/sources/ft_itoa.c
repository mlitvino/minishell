/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:35:50 by mlitvino          #+#    #+#             */
/*   Updated: 2024/11/06 20:29:46 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strlen_itoa(int n)
{
	int	str_len;

	str_len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = -n;
		str_len++;
	}
	while (n / 10 > 0)
	{
		str_len++;
		n /= 10;
	}
	if (n % 10 > 0)
		str_len++;
	return (str_len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		str_len;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	str_len = ft_strlen_itoa(n);
	str = (char *)malloc(sizeof(char) * (str_len + 1));
	if (str == NULL)
		return (NULL);
	ft_bzero(str, str_len + 1);
	if (n < 0)
	{
		str[0] = '-';
		n = -n;
	}
	while (str_len-- > 0 && str[str_len] != '-')
	{
		str[str_len] = (n % 10) + '0';
		if (str_len == 0)
			break ;
		n /= 10;
	}
	return (str);
}
