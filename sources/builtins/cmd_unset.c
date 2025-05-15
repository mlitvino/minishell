/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:49:00 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/15 14:28:01 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_var_value(t_data *data, char *env_var)
{
	t_list	*temp;

	temp = find_var(data->env, env_var, NULL);
	if (!temp || ft_strchr(temp->content, '=') == NULL)
		return (NULL);
	else
		return (ft_strchr(temp->content, '=') + 1);
}

static void	join_delete_var(t_list **list, t_list *prev, t_list *temp)
{
	if (prev == NULL)
		*list = temp->next;
	else
		prev->next = temp->next;
	ft_lstdelone(temp, free);
}

void	delete_var(t_list **list, char *var)
{
	t_list	*temp;
	t_list	*prev;
	int		sign_i;
	char	*compare_sign;

	temp = *list;
	prev = NULL;
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
			join_delete_var(list, prev, temp);
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
