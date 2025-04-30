/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:49:00 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/30 13:00:53 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_var(t_list **list, char *var)
{
	t_list	*temp;
	t_list	*prev;
	int		sign_i;
	char	*compare_sign;

	temp = *list;
	while (temp)
	{
		compare_sign = ft_strchr(temp->content, '=');
		if (!compare_sign)
			compare_sign = ft_strchr(temp->content, '\0');
		sign_i = compare_sign - (char *)temp->content;
		if (ft_strncmp(temp->content, var, sign_i) == 0)
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

int	cmd_unset(t_data *data, t_args *args)
{
	while (args)
	{
		delete_var(&data->env, args->value);
		args = args->next;
	}
	return (SUCCESS);
}
