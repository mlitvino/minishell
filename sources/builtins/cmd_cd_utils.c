/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 17:00:41 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/11 18:26:31 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_path(t_data *data, t_args *args)
{
	char	*path;

	path = NULL;
	if (args)
		path = ft_strdup(args->value);
	else if (!args)
		path = get_home_path(data);
	if (!path)
		return (FAILURE);
	if (*path == '\0')
		return (free(path), SUCCESS);
	if (chdir(path) != SUCCESS)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		free(path);
		return (FAILURE);
	}
	free(path);
	return (SUCCESS);
}
