/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:36:35 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/05 01:17:50 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_replce_var(t_list **list, char *var)
{
	t_list	*temp;
	t_list	*prev;
	char	*equals_sign;
	int		equals_i;

	temp = *list;
	while(temp)
	{
		equals_sign = ft_strchr(temp->content, '=');
		equals_i = equals_sign - (char *)temp->content;
		if (ft_strncmp(temp->content, var, equals_i + 1) == 0)
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
	temp = ft_lstnew(var);
	// NUL check
	ft_lstadd_back(list, temp);
}

void	cmd_export(t_data *data, char *var)
{
	char	*cpy_var1;
	char	*cpy_var2;

	cpy_var1 = ft_strdup(var);
	// NUL check
	cpy_var2 = ft_strdup(var);
	// NUL check
	add_replce_var(&data->local_vars, cpy_var1);
	add_replce_var(&data->env, cpy_var2);
}
