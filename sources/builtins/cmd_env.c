/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:43:02 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/06 17:03:51 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_env(char *sys_env[], t_data *data)
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
			free(content);
			free(new_node);
			clean_all(data, FAILURE, "minishell: malloc failed\n");
		}
		ft_lstadd_back(&new_env, new_node);
		i++;
	}
	data->env = new_env;
}

int	cmd_env(t_data *data, t_args *args)
{
	t_list	*env;

	env = data->env;
	if (args)
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		return (FAILURE);
	}
	while (env)
	{
		if (ft_strchr((char *)env->content, '=') != NULL)
			if (printf("%s\n", (char *)env->content) < SUCCESS)
				return (FAILURE);
		env = env->next;
	}
	return (SUCCESS);
}
