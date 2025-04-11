/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:30:49 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/11 14:54:24 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var(t_data *data, char *var)
{
	char	*key_var;
	char	*value_var;
	t_list	*temp;

	key_var = ft_strjoin(var, "=");
	// NULL check
	temp = find_var(&data->local_vars, key_var, NULL);
	if (!temp)
		temp = find_var(&data->env, key_var, NULL);
	if (temp)
	{
		value_var = strchr(temp->content, '=') + 1;
		return (value_var);
	}
	return (NULL);
}
