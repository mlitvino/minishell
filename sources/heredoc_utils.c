/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:48:09 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/27 20:48:44 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bzero_existing(t_data *data, t_redir *heredoc)
{
	heredoc->existing = 0;
}

int	map_heredoc(t_data *data, int (*func)(t_data *data, t_redir *heredoc))
{
	t_pipe_line		*curr_pipeline;
	t_simple_cmd	*curr_cmd;
	t_redir			*curr_redir;
	//t_parent		*ex;
	//t_log			*ex;

	if (data->cmd_list)
		curr_pipeline = data->cmd_list->childs;
	else
		curr_pipeline = NULL;
	while (curr_pipeline)
	{
		curr_cmd = curr_pipeline->child;
		while (curr_cmd)
		{
			curr_redir = curr_cmd->redirections;
			while (curr_redir)
			{
				if (curr_redir->type == RE_DOUBLE_LESS)
					if (func(data, curr_redir) == TERM_SIGINT)
						return (TERM_SIGINT);
				curr_redir = curr_redir->next;
			}
			curr_cmd = curr_cmd->next;
		}
		curr_pipeline = curr_pipeline->next;
	}
	return (SUCCESS);
}
