/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:14:50 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/17 14:36:06 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	join_paste_var(t_data *data, char *key_var, char *var_value)
{
	char	*new_var;
	char	*temp;

	temp = ft_strjoin(key_var, "=");
	if (!temp)
		return (perror("minishell: cd: malloc"), FAILURE);
	new_var = ft_strjoin(temp, var_value);
	free(temp);
	free(var_value);
	if (!new_var)
	{
		perror("minishell: cd: malloc");
		return (FAILURE);
	}
	if (find_var(data->env, key_var, NULL) != NULL)
		if (add_replce_var(&data->env, new_var) == NULL)
			return (free(new_var), perror("minishell: cd: malloc"), FAILURE);
	return (SUCCESS);
}

int	update_pwd(t_data *data)
{
	char	*cwd;
	char	*data_pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: cd");
		return (FAILURE);
	}
	free(data->pwd);
	data_pwd = ft_strdup(cwd);
	if (!data_pwd)
	{
		free(cwd);
		perror("minishell: malloc");
		return (FAILURE);
	}
	data->pwd = data_pwd;
	if (join_paste_var(data, "PWD", cwd) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	update_oldpwd(t_data *data, t_list *env)
{
	char	*var_value;
	t_list	*env_var;

	env_var = find_var(env, "PWD", NULL);
	if (!env_var || ft_strchr(env_var->content, '=') == NULL)
	{
		delete_var(&env, "OLDPWD");
		return (SUCCESS);
	}
	var_value = ft_strdup(ft_strchr(env_var->content, '=') + 1);
	if (!var_value)
		return (perror("minishell: cd: malloc"), FAILURE);
	if (join_paste_var(data, "OLDPWD", var_value) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

char	*get_home_path(t_data *data)
{
	t_list	*var;
	char	*path;

	var = find_var(data->env, "HOME", NULL);
	if (var && ft_strchr(var->content, '=') != NULL)
	{
		path = ft_strdup(ft_strchr(var->content, '=') + 1);
		if (!path)
		{
			perror("minishell: cd: malloc");
			return (NULL);
		}
		return (path);
	}
	else
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	}
	return (NULL);
}

int	cmd_cd(t_data *data, t_args *args)
{
	(void)data;
	if (args && args->next)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2),
			FAILURE);
	else
		if (set_path(data, args) == FAILURE)
			return (FAILURE);
	if (update_oldpwd(data, data->env) != SUCCESS)
		return (FAILURE);
	if (update_pwd(data) != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}
