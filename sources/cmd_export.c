/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:36:35 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/11 14:38:29 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*find_var(t_list **list, char *var, t_list **prev)
{
	t_list	*list_var;
	char	*equals_sign;
	int		equals_i;

	list_var = *list;
	while(list_var)
	{
		equals_sign = ft_strchr(list_var->content, '=');
		equals_i = equals_sign - (char *)list_var->content;
		if (ft_strncmp(list_var->content, var, equals_i + 1) == 0)
		{
			return (list_var);
		}
		if (prev)
			*prev = list_var;
		list_var = list_var->next;
	}
	return (NULL);
}

void	add_replce_var(t_list **list, char *new_var)
{
	t_list	*list_var;
	t_list	*prev;
	t_list	*temp;

	temp = ft_lstnew(new_var);
	// NUL check
	list_var = find_var(list, new_var, &prev);
	if (!list_var)
		ft_lstadd_back(list, temp);
	else
	{
		temp->next = list_var->next;
		if (list_var == *list)
			*list = temp;
		else
			prev->next = temp;
		ft_lstdelone(list_var, free);
	}
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
