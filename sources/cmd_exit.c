/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:29:45 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/23 13:31:34 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_exit(t_data *data, t_args *args)
{
	clean_all(data, SUCCESS, "exit\n");
	return (SUCCESS);
}
