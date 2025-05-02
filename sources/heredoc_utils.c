/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:48:09 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/02 14:30:46 by mlitvino         ###   ########.fr       */
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
	expnd_str = expand_str(data, input, ft_strdup(""));
	free(input);
	if (!expnd_str)
		clean_all(data, FAILURE, "minishell: heredoc: malloc failed\n");
	return (expnd_str);
}

char	*trim_delim(t_data *data, t_redir *heredoc)
{
	char	*new_delim;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (heredoc->delim[i])
	{
		if (heredoc->delim[i] != '\'' && heredoc->delim[i] != '\"')
			len++;
		i++;
	}
	new_delim = malloc(sizeof(char) * (len + 1));
	if (!new_delim)
		return (NULL);
	len++;
	while (len >= 0 && i >= 0)
	{
		if (heredoc->delim[i] != '\'' && heredoc->delim[i] != '\"')
			new_delim[--len] = heredoc->delim[i];
		i--;
	}
	free(heredoc->delim);
	heredoc->delim = new_delim;
	return (new_delim);
}

int	bzero_existing(t_data *data, t_redir *heredoc)
{
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
}

int	map_heredoc(t_data *data, int (*func)(t_data *data, t_redir *heredoc))
{
	t_pipe_line		*pipeline;
	t_simple_cmd	*cmd;
	t_redir			*redir;

	pipeline = NULL;
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
