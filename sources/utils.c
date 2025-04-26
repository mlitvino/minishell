/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:30:49 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/26 19:56:02 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	args_size(t_args *args)
{
	int	size;

	size = 0;
	while (args)
	{
		args = args->next;
		size++;
	}
	return (size);
}

char	**convrt_args_to_argv(t_args *args, char *cmd_name)
{
	char	**argv;
	int		size;
	int		i;

	size = args_size(args);
	argv = malloc(sizeof(char *) * (size + 2));
	if (!argv)
		return (NULL);
	i = 0;
	argv[i++] = cmd_name;
	while (i < size + 1)
	{
		argv[i] = ft_strdup(args->value);
		if (!argv[i])
			return (free_argv(argv), NULL);
		args = args->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

char	**convrt_lst_to_argv(t_list *lst)
{
	char	**argv;
	int		size;
	int		i;

	size = ft_lstsize(lst);
	argv = malloc(sizeof(char *) * (size + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (i < size)
	{
		argv[i] = ft_strdup(lst->content);
		if (!argv[i])
			return (free_argv(argv), NULL);
		lst = lst->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

char	*expand_var(t_data *data, char *key_var)
{
	char	*value_var;
	t_list	*temp;

	temp = find_var(data->local_vars, key_var, NULL);
	if (!temp)
		temp = find_var(data->env, key_var, NULL);
	if (temp)
	{
		value_var = strchr(temp->content, '=') + 1;
		return (value_var);
	}
	return (NULL);
}

int	is_builtin(t_builtin *arr, char *cmd_name)
{
	int	i;

	i = 0;
	while (arr && arr[i].name)
	{
		if (ft_strcmp(arr[i].name, cmd_name) == 0)
			return (i);
		i++;
	}
	return (-1);
}
