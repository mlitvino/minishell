/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:36:35 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/07 17:22:35 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*find_var(t_list *list, char *var, t_list **prev)
{
	t_list	*list_var;
	char	*env_var;
	int		i;

	list_var = list;
	while (list_var)
	{
		i = 0;
		env_var = (char *)list_var->content;
		while (env_var[i] && var[i] && env_var[i] == var[i]
			&& env_var[i] != '=')
			i++;
		if (env_var[i] == var[i] || (!var[i] && env_var[i] == '=')
			|| (!env_var[i] && var[i] == '='))
			return (list_var);
		if (prev)
			*prev = list_var;
		list_var = list_var->next;
	}
	return (NULL);
}

t_list	*add_replce_var(t_list **list, char *new_var)
{
	t_list	*list_var;
	t_list	*prev;
	t_list	*temp;
	char	*key;

	temp = ft_lstnew(new_var);
	if (!temp)
		return (NULL);
	list_var = find_var(*list, new_var, &prev);
	if (!list_var)
		ft_lstadd_back(list, temp);
	else
	{
		temp->next = list_var->next;
		if (list_var == *list)
			*list = temp;
		else
			prev->next = temp;
		ft_lstdelone(list_var, free);
	}
	return (temp);
}

int	check_export_arg(t_args *args, int *exit_code) // SPACE CHECK
{
	char	*arg_value;

	arg_value = args->value;
	if (ft_isdigit(*arg_value) == 0 && (ft_isalpha(*arg_value) == 1
			|| *arg_value == '_'))
	{
		while (*arg_value)
		{
			if (*arg_value == '=')
				return (SUCCESS);
			if (ft_isdigit(*arg_value) == 0 && ft_isalpha(*arg_value) == 0
				&& *arg_value != '_')
				break ;
			arg_value++;
		}
		if (!*arg_value)
			return (SUCCESS);
	}
	*exit_code = 1;
	print_strs_fd("minishell: export: `", args->value,
		"': not a valid identifier\n", 2);
	return (FAILURE);
}

int	export_no_args(t_data *data)
{
	t_list	*env;

	env = data->env;
	while (env)
	{
		if (printf("declare -x ") < SUCCESS)
			return (FAILURE);
		if (printf("%s\n", (char *)env->content) < SUCCESS)
			return (FAILURE);
		env = env->next;
	}
	return (SUCCESS);
}

int	cmd_export(t_data *data, t_args *args)
{
	char	*cpy_var;
	int		exit_code;

	exit_code = SUCCESS;
	if (!args)
		exit_code = export_no_args(data);
	while (args)
	{
		if (check_export_arg(args, &exit_code) == SUCCESS)
		{
			cpy_var = ft_strdup(args->value);
			if (!cpy_var)
				return (perror("minishell: export: malloc"), FAILURE);
			if (add_replce_var(&data->env, cpy_var) == NULL)
				return (perror("minishell: export: malloc"), FAILURE);
		}
		args = args->next;
	}
	return (exit_code);
}
