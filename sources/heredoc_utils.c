/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:48:09 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/08 14:12:46 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_hd_input(t_data *data, t_redir *heredoc, char *input)
{
	char	*expnd_str;

	if (!input || ft_strcmp(input, heredoc->delim) == 0)
	{
		if (!input && g_signal_received == 0)
		{
			ft_putstr_fd("minishell: warning: here-document delimited \
				by end-of-file, wanted: ", 2);
			ft_putstr_fd(heredoc->delim, 2);
		}
		free(input);
		clean_all(data, g_signal_received, NULL);
	}
	if (heredoc->inside_quotes == 0)
		expnd_str = expand_str(data, input, ft_strdup(""));
	else
		expnd_str = ft_strdup(input);
	free(input);
	if (!expnd_str)
		clean_all(data, FAILURE, "minishell: heredoc: malloc failed\n");
	return (expnd_str);
}

int	bzero_existing(t_data *data, t_redir *heredoc)
{
	(void)data;
	heredoc->existing = 0;
	return (SUCCESS);
}

static int	find_hd(t_data *data, t_simple_cmd *cmd,
		t_redir *redir, int (*func)(t_data *, t_redir *))
{
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == RE_DOUBLE_LESS)
				if (func(data, redir) == TERM_SIGINT)
					return (TERM_SIGINT);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (SUCCESS);
}

int	map_heredoc(t_data *data, int (*func)(t_data *data, t_redir *heredoc))
{
	t_pipe_line		*pipeline;
	t_simple_cmd	*cmd;
	t_redir			*redir;

	pipeline = NULL;
	redir = NULL;
	cmd = NULL;
	if (data->cmd_list)
		pipeline = data->cmd_list->childs;
	while (pipeline)
	{
		cmd = pipeline->child;
		find_hd(data, cmd, redir, func);
		pipeline = pipeline->next;
	}
	return (SUCCESS);
}
