/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:54:30 by mlitvino          #+#    #+#             */
/*   Updated: 2024/10/31 17:28:27 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	unsigned int	j;
	char			*s3;
	size_t			s3_size;

	i = 0;
	j = 0;
	s3_size = ft_strlen(s1) + ft_strlen(s2) + 1;
	s3 = malloc(sizeof(char) * s3_size);
	if (s3 == NULL)
		return (NULL);
	while (i < s3_size && s1[i])
	{
		s3[i] = s1[i];
		i++;
	}
	while (i < s3_size && s2[j])
	{
		s3[i] = s2[j];
		i++;
		j++;
	}
	s3[i] = '\0';
	return (s3);
}
