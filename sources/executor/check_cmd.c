/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:13:19 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/26 17:23:31 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_empty(t_simple_cmd *cmd)
{
	void	*temp;
	t_args	*args;

	while (cmd->command[0] == '\0' && cmd->inside_quotes == 0 && cmd->args)
	{
		free(cmd->command);
		cmd->command = cmd->args->value;
		cmd->inside_quotes = cmd->args->inside_quotes;
		temp = cmd->args->next;
		free(cmd->args);
		cmd->args = temp;
	}
	args = cmd->args;
	while (args && args->value[0] == '\0' && args->inside_quotes == 0)
	{
		temp = args->next;
		free(args->value);
		free(args);
		args = temp;
	}
	cmd->args = args;
}

t_simple_cmd	*init_null_cmd(t_simple_cmd **curr_cmd)
{
	if (!(*curr_cmd)->command)
	{
		(*curr_cmd)->command = ft_strdup("");
		if (!(*curr_cmd)->command)
			return (NULL);
	}
	return (*curr_cmd);
}

void	check_cmd(t_data *data, t_simple_cmd *curr_cmd, int cmd_count, int i)
{
	if (init_null_cmd(&curr_cmd) == NULL)
		clean_all(data, FAILURE, "minishell: malloc failed\n");
	curr_cmd->builtin_arr = data->builtin_arr;
	curr_cmd->cmd_count = cmd_count;
	curr_cmd->cmd_i = i;
	curr_cmd->exit_code = SUCCESS;
	curr_cmd->cmd_pid = -1;
	check_quots_expand(data, curr_cmd);
	check_empty(curr_cmd);
}
