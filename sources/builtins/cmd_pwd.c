/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:52:40 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/23 13:40:16 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_pwd(t_data *data, t_args *args)
{
	t_list	*temp;

	(void)args;
	temp = data->env;
	while (temp)
	{
		if (ft_strncmp(temp->content, "PWD=", 4) == 0)
		{
			if (printf("%s\n", ft_strchr(temp->content, '=') + 1) < SUCCESS)
				return (FAILURE);
			break ;
		}
		temp = temp->next;
	}
	return (SUCCESS);
}
