/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:14:50 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/18 14:50:50 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(t_data *data)
{
	char	*new_pwd;
	char	*temp;

	temp = getcwd(NULL, 0);
	// NUL check
	new_pwd = ft_strjoin("PWD=", temp);
	// NUL check
	free(temp);
	add_replce_var(&data->env, new_pwd);
}

void	update_oldpwd(t_data *data)
{
	char	*var_value;
	char	*new_var;
	t_list	*temp;

	temp = data->env;
	while (temp)
	{
		if (ft_strncmp(temp->content, "PWD=", 4) == 0)
		{
			var_value = ft_strdup(ft_strchr(temp->content, '=') + 1);
			// NUL CHECK
		}
		temp = temp->next;
	}
	new_var = ft_strjoin("OLDPWD=", var_value);
	// NUL check
	add_replce_var(&data->env, new_var);
	// NUL check
}

void	cmd_cd(t_data *data, t_args *args)
{
	t_list	*temp;
	char	*path;

	path = args->value;
	if (!path)
	{
		temp = data->env;
		while (temp)
		{
			if (ft_strncmp(temp->content, "HOME=", 5) == 0)
			{
				path = ft_strdup(ft_strchr(temp->content, '=') + 1);
				// NUL CHECK
			}
			temp = temp->next;
		}
	}
	chdir(path);
	// err check
	update_oldpwd(data);
	update_pwd(data);
}
