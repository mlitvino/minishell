/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:07:29 by mlitvino          #+#    #+#             */
/*   Updated: 2024/11/06 20:24:03 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s2;
	int		end;
	int		start;
	int		i;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = 0;
	while (s1[start] && ft_check(s1[start], set))
		start++;
	end = ft_strlen(s1) - 1;
	while (end >= start && ft_check(s1[end], set))
		end--;
	s2 = (char *)malloc(sizeof(char) * (end - start + 2));
	if (s2 == NULL)
		return (NULL);
	i = 0;
	while (start <= end)
	{
		s2[i++] = s1[start++];
	}
	s2[i] = '\0';
	return (s2);
}
