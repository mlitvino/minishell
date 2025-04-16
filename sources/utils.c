/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:30:49 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/16 18:02:04 by mlitvino         ###   ########.fr       */
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

t_redir	*find_redir(t_pipe_line *pipeline, t_redir_type find_type)
{
	static t_pipe_line	*current_pipeline = NULL;
	static t_simple_cmd	*current_cmd = NULL;
	static t_redir		*current_redir = NULL;

	if (current_pipeline == NULL)
	{
		
	}
	current_pipeline = NULL;
	current_cmd = NULL;
	current_redir = NULL;
}
