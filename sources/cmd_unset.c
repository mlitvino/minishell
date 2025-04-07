/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:49:00 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/04 23:18:08 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_var(t_list **list, char *var)
{
	t_list	*temp;
	t_list	*prev;
	int		equals_i;
	char	*equals_sign;

	temp = *list;
	while (temp)
	{
		equals_sign = ft_strchr(temp->content, '=');
		equals_i = equals_sign - (char *)temp->content;
		if (ft_strncmp(temp->content, var, equals_i) == 0)
		{
			if (temp == *list)
				*list = temp->next;
			else
				prev->next = temp->next;
			ft_lstdelone(temp, free);
			break ;
		}
		prev = temp;
		temp = temp->next;
	}
}

void	cmd_unset(t_data *data, char *var)
{
	delete_var(&data->local_vars, var);
	delete_var(&data->env, var);
}
