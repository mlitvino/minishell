/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:43:02 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/04 22:57:54 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cpy_env(char *sys_env[], t_data *data)
{
	int			i;
	t_list		*new_env;
	t_list		*new_node;
	char		*content;

	i = 0;
	new_env = NULL;
	while (sys_env[i])
	{
		content = ft_strdup(sys_env[i]);
		// NUL check
		new_node = ft_lstnew(content);
		// NUL check
		ft_lstadd_back(&new_env, new_node);
		i++;
	}
	data->env = new_env;
}

void	cmd_env(t_list *env)
{
	while (env)
	{
		printf("%s\n", (char *)env->content);
		env = env->next;
	}
}
