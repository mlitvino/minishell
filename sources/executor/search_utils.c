/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:18:41 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/02 14:27:04 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_pathname(t_data *data, t_simple_cmd *cmd, char *path_str)
{
	char	*temp;

	temp = ft_strjoin("/", cmd->command);
	if (!temp)
		clean_all(data, FAILURE, "minishell: command: malloc failed\n");
	cmd->pathname = ft_strjoin(path_str, temp);
	free(temp);
	if (!cmd->pathname)
		clean_all(data, FAILURE, "minishell: command: malloc failed\n");
}

char	**create_path_tab(t_data *data)
{
	char	**path_tab;
	char	*path_value;

	path_value = expand_var(data, "$PATH");
	if (!path_value)
		clean_all(data, FAILURE, "minishell: command: malloc failed\n");
	path_tab = ft_split(path_value, ':');
	free(path_value);
	if (!path_tab)
		clean_all(data, FAILURE, "minishell: command: malloc failed\n");
	return (path_tab);
}
