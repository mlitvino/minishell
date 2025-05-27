/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:51:13 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/27 21:56:59 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_i_end_quot(char *str, char token)
{
	int		i;
	char	*temp;

	i = 0;
	temp = ft_strchr(str + 1, token);
	if (temp)
		i = temp - str + 1;
	else
	{
		while (str[i] && ft_isspace(str[i]) != 1)
			i++;
	}
	return (i);
}
