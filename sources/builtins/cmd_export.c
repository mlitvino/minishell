/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:36:35 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/30 12:44:16 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*find_var(t_list *list, char *var, t_list **prev)
{
	t_list	*list_var;
	char	*equals_sign;
	int		equals_i;

	list_var = list;
	while (list_var)
	{
		equals_sign = ft_strchr(list_var->content, '=');
		equals_i = equals_sign - (char *)list_var->content;
		if (ft_strncmp(list_var->content, var, equals_i + 1) == 0)
		{
			return (list_var);
		}
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
	if (ft_isdigit(*arg_value) == 0
		&& (ft_isalpha(*arg_value) == 1 || *arg_value == '_'))
	{
		while (*arg_value)
		{
			if (*arg_value == '=')
				return (SUCCESS);
			if (ft_isdigit(*arg_value) == 0
				&& ft_isalpha(*arg_value) == 0
				&& *arg_value != '_')
				break ;
			arg_value++;
		}
		if (!*arg_value)
			return (MISUSE);
	}
	*exit_code = 1;
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(args->value, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (FAILURE);
}

int	cmd_export(t_data *data, t_args *args)
{
	char	*cpy_var;
	int		exit_code;

	exit_code = SUCCESS;
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
