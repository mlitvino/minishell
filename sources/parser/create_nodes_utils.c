/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_nodes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:59:19 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/07 16:59:57 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_list	*init_cmd_list(void)
{
	t_cmd_list	*cmd_list;

	cmd_list = NULL;
	cmd_list = (t_cmd_list *)malloc(sizeof(t_cmd_list));
	if (!cmd_list)
		return (NULL);
	cmd_list->childs = NULL;
	cmd_list->pipe_line_count = 0;
	return (cmd_list);
}

t_simple_cmd	*ft_init_simple_cmd(void)
{
	t_simple_cmd	*cmd;

	cmd = (t_simple_cmd *)malloc(sizeof(t_simple_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->command = NULL;
	cmd->next = NULL;
	cmd->args = NULL;
	cmd->redirections = NULL;
	cmd->inside_quotes = 0;
	cmd->exit_code = SUCCESS;
	cmd->pathname = NULL;
	cmd->cmd_i = 0;
	cmd->cmd_count = 0;
	cmd->builtin_arr = NULL;
	cmd->cmd_pid = -1;
	cmd->std_fd[STDIN] = -1;
	cmd->std_fd[STDOUT] = -1;
	return (cmd);
}

void	clean_cmds(t_simple_cmd *cmd)
{
	t_redir	*redir;
	t_args	*args;
	void	*prev_ptr;

	while (cmd)
	{
		free_args(cmd->args);
		free_redir(cmd->redirections);
		if (cmd->std_fd[STDIN] != -1)
			close(cmd->std_fd[STDIN]);
		if (cmd->std_fd[STDOUT] != -1)
			close(cmd->std_fd[STDOUT]);
		free(cmd->command);
		cmd->command = NULL;
		free(cmd->pathname);
		cmd->pathname = NULL;
		prev_ptr = cmd;
		cmd = cmd->next;
		free(prev_ptr);
	}
}
