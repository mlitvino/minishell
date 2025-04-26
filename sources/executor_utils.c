/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:34:26 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/26 16:06:13 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_childs(t_data *data, t_simple_cmd *lst_cmd)
{
	int	status;

	status = SUCCESS;
	if (lst_cmd->exit_code == SUCCESS)
	{
		while (waitpid(lst_cmd->cmd_pid, &status, 0) != -1 && errno != ECHILD)
			;
		if (WIFEXITED(status))
			data->exit_var = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->exit_var = 128 + WTERMSIG(status);
		else
			data->exit_var = 1;
	}
	else
		data->exit_var = lst_cmd->exit_code;
	while (waitpid(0, 0, 0) != -1 && errno != ECHILD)
		;
}
