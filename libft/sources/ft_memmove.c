/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:54:14 by mlitvino          #+#    #+#             */
/*   Updated: 2024/11/04 13:50:46 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*ptr_dest;
	char	*ptr_src;

	ptr_dest = (char *)dest;
	ptr_src = (char *)src;
	if (dest <= src)
	{
		while (n--)
		{
			*ptr_dest++ = *ptr_src++;
		}
	}
	else if (dest > src)
	{
		ptr_dest += n - 1;
		ptr_src += n - 1;
		while (n--)
		{
			*ptr_dest-- = *ptr_src--;
		}
	}
	return (dest);
}
