/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:06:58 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/22 15:01:30 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hd_sig_hanlder(int sig)
{
		printf("\n");
		rl_replace_line("", 0);
		rl_redisplay();
		rl_on_new_line();
		printf("UWU"); // del
		g_signal_received = 1;
}

void	fill_heredoc(t_data *data, t_redir *heredoc)
{
	char	*delim;
	char	*input;

	delim = heredoc->delim;
	signal(SIGINT, hd_sig_hanlder);
	while (1)
	{
		input = readline("> ");
		// handling Crtl+C
		if (g_signal_received == 1)
		{
			// clean?
			g_signal_received = 0;
		}
		if (!input || ft_strcmp(input, delim) == 0)
		{
			if (!input)
				printf("bash: warning: here-document delimited \
					by end-of-file (wanted `%s')", heredoc->delim);
			return ;
		}
		heredoc->fd = open(heredoc->file_name, O_WRONLY);
		ft_putstr_fd(input, heredoc->fd);
		close(heredoc->fd);
	}
}

void	create_heredoc(t_data *data, t_redir *heredoc)
{
	int		i;
	char	*file_id;
	char	*file_name;
	int		hd_fd;

	i = 1;
	chdir("/tmp/");
	// err check
	while (1)
	{
		file_id = ft_itoa(i);
		// NUL check
		file_name = ft_strjoin("temp-hd-", file_id);
		// NUL check
		hd_fd = open(file_name, O_RDWR | O_CREAT | O_EXCL, 0644);
		// err check
		if (hd_fd == -1 && errno == EEXIST)
			i++;
		else
			break ;
	}
	free(file_id);
	close(hd_fd);
	heredoc->file_name = file_name;
}

void	check_create_heredoc(t_data *data, t_pipe_line *pipeline)
{
	t_simple_cmd	*cmd;
	t_redir			*redir;

	while (pipeline)
	{
		cmd = pipeline->child;
		while(cmd)
		{
			redir = cmd->redirections;
			while (redir)
			{
				if (redir->type == RE_DOUBLE_LESS)
				{
					if (signal(SIGINT, SIG_IGN) == SIG_ERR)
						clean_all(data, EXIT_FAILURE, "Error: func signal failed\n");
					create_heredoc(data, redir);
					fill_heredoc(data, redir);
					init_sigs();
				}
				redir = redir->next;
			}
			cmd = cmd->next;
		}
		pipeline = pipeline->next;
	}
}

void	unlink_heredoc(t_data *data, t_pipe_line *pipeline)
{
	t_simple_cmd	*cmd;
	t_redir			*redir;

	while (pipeline)
	{
		cmd = pipeline->child;
		while(cmd)
		{
			redir = cmd->redirections;
			while (redir)
			{
				if (redir->type == RE_DOUBLE_LESS)
				{
					unlink(redir->file_name);
				}
				redir = redir->next;
			}
			cmd = cmd->next;
		}
		pipeline = pipeline->next;
	}
}
