/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:49:00 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/01 16:15:38 by mlitvino         ###   ########.fr       */
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
	prev = *list;
	while (temp)
	{
		sign_i = 0;
		compare_sign = (char *)temp->content;
		while (var[sign_i] && compare_sign[sign_i]
			&& var[sign_i] == compare_sign[sign_i])
		sign_i++;
		if (!var[sign_i] && (!compare_sign[sign_i]
			|| compare_sign[sign_i] == '='))
		{
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
