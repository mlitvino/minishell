/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:14:50 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/23 14:32:22 by mlitvino         ###   ########.fr       */
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

int	update_oldpwd(t_data *data, t_list *env)
{
	char	*var_value;
	char	*new_var;
	t_list	*env_var;

	var_value = NULL;
	// while (env)
	// {
	// 	if (ft_strncmp(env->content, "PWD=", 4) == 0)
	// 	{
	// 		var_value = ft_strdup(ft_strchr(env->content, '=') + 1);
	// 		if (!var_value)
	// 		{
	// 			perror("minishell: cd: malloc");
	// 			return (FAILURE);
	// 		}
	// 	}
	// 	env = env->next;
	// }
	env_var = find_var(env, "PWD=", NULL);
	if (!env_var)
		env_var = find_var(data->local_vars, "PWD=", NULL);
	if (!env_var)
	{
		delete_var(&env, "OLDPWD=");
		delete_var(&data->local_vars, "OLDPWD=");
		return (SUCCESS);
	}
	var_value = ft_strdup(ft_strchr(env_var->content, '=') + 1);
	new_var = ft_strjoin("OLDPWD=", var_value);
	if (!var_value || !new_var)
	{
		
	}
	if (add_replce_var(&data->env, new_var) == NULL)
	{

	}

}

int	cd_home(t_data *data, t_args *args)
{
	t_list	*temp;
	char	*path;

	temp = data->env;
	while (temp)
	{
		if (ft_strncmp(temp->content, "HOME=", 5) == 0)
		{
			path = ft_strdup(ft_strchr(temp->content, '=') + 1);
			if (!path)
			{
				perror("minishell: cd: malloc");
				return (FAILURE);
			}
			return (SUCCESS);
		}
		temp = temp->next;
	}
	ft_putstr_fd("minishell: cd: HOME not set", 2);
	return (FAILURE);
}

int	cmd_cd(t_data *data, t_args *args)
{
	char	*path;

	path = args->value;
	if (!path)
		if (cd_home(data, args) != SUCCESS)
			return (FAILURE);
	if (*path == '\0');
		return (SUCCESS);
	if (chdir(path) != SUCCESS)
	{
		perror("minishell: cd:");
		return (FAILURE);
	}
	update_oldpwd(data, data->env);
	update_pwd(data);
	return (SUCCESS);
}
