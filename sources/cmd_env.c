/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:43:02 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/03 17:15:18 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cpy_env(char *sys_env[], t_data *data)
{
	int			count;
	int			i;
	size_t		len;
	char		**new_env;

	count = 0;
	while (sys_env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	// NUL check
	new_env[count] = NULL;
	i = 0;
	while (i < count)
	{
		len = ft_strlen(sys_env[i]);
		new_env[i] = malloc(sizeof(char) * (len + 1));
		// NUL check
		ft_strlcpy(new_env[i], sys_env[i], len);
		i++;
	}
	data->env = new_env;
}

void	cmd_env(char *env[])
{
	while (*env)
		printf("%s\n", *env++);
}
