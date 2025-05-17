/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:34:26 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/17 14:22:17 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_get_exitcode(t_data *data, pid_t child_pid)
{
	int	status;

	status = SUCCESS;
	while (waitpid(child_pid, &status, 0) != -1 && errno != ECHILD)
		;
	if (g_signal_received == 1)
	{
		data->exit_var = 128 + SIGINT;
		g_signal_received = 0;
	}
	else if (WIFEXITED(status))
		data->exit_var = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_var = 128 + WTERMSIG(status);
	else
		data->exit_var = FAILURE;
	if (data->exit_var - 128 == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", 2);
	else if (data->exit_var - 128 == SIGINT)
		printf("\n");
	return (data->exit_var);
}

void	wait_childs(t_data *data, t_simple_cmd *lst_cmd)
{
	signal(SIGINT, SIG_IGN);
	if (lst_cmd->cmd_pid != -1)
	{
		wait_get_exitcode(data, lst_cmd->cmd_pid);
	}
	else
		data->exit_var = lst_cmd->exit_code;
	while (waitpid(0, 0, 0) != -1 && errno != ECHILD)
		;
	init_sigs(data);
}
