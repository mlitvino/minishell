/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:52:40 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/07 13:00:13 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_pwd(t_data *data)
{
	t_list	*temp;

	temp = data->env;
	while (temp)
	{
		if (ft_strncmp(temp->content, "PWD=", 4) == 0)
		{
			printf("%s\n", ft_strchr(temp->content, '=') + 1);
			break ;
		}
		temp = temp->next;
	}
}
