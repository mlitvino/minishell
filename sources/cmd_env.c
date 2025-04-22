/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:43:02 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/22 14:37:18 by mlitvino         ###   ########.fr       */
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
		new_node = ft_lstnew(content);
		if (!content || !new_node)
		{
			ft_putstr_fd("Error: malloc failed\n", 2);
			free(content);
			free(new_node);
			ft_lstclear(&new_env, free);
			exit(1);
		}
		ft_lstadd_back(&new_env, new_node);
		i++;
	}
	data->env = new_env;
}

void	cmd_env(t_data *data, t_args *args)
{
	t_list	*env;

	env = data->env;
	while (env)
	{
		printf("%s\n", (char *)env->content);
		env = env->next;
	}
}
