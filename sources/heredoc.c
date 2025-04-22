/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:06:58 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/22 19:17:47 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hd_sig_hanlder(int sig)
{
		printf("\n");
		rl_replace_line("", 0);
		rl_redisplay();
		rl_on_new_line();
		g_signal_received = TERM_SIGINT;
		close(STDIN);
}

void	fill_heredoc(t_data *data, t_redir *heredoc)
{
	char	*input;

	if (signal(SIGINT, hd_sig_hanlder) == SIG_ERR)
		clean_all(data, FAILURE, "Error: func signal failed\n");
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, heredoc->delim) == 0)
		{
			if (!input && g_signal_received == 0)
			{
				dup2(STDOUT, STDERR);
				printf("minishell: warning: here-document delimited \
					by end-of-file (wanted `%s')", heredoc->delim);
			}
			clean_all(data, g_signal_received, NULL);
		}
		heredoc->fd = open(heredoc->file_name, O_WRONLY | O_APPEND);
		if (heredoc->fd == -1)
			clean_all(data, FAILURE, strerror(errno));
		ft_putendl_fd(input, heredoc->fd);
		close(heredoc->fd);
	}
}

void	create_heredoc(t_data *data, t_redir *heredoc)
{
	int		i;
	char	*file_id;

	i = 1;
	while (1)
	{
		file_id = ft_itoa(i);
		if (!file_id)
			clean_all(data, FAILURE, "minishell: malloc failed\n");
		heredoc->file_name = ft_strjoin("/tmp/temp-hd-", file_id);
		free(file_id);
		if (!heredoc->file_name)
			clean_all(data, FAILURE, "minishell: malloc failed\n");
		heredoc->fd = open(heredoc->file_name, O_RDWR | O_CREAT | O_EXCL, 0644);
		if (heredoc->fd != -1)
			break;
		else if (heredoc->fd == -1 && errno != EEXIST)
			clean_all(data, FAILURE, strerror(errno));
		i++;
	}
	close(heredoc->fd);
}

int	check_create_heredoc(t_data *data, t_pipe_line *pipeline)
{
	t_simple_cmd	*cmd;
	t_redir			*redir;
	pid_t			heredoc_pid;
	int				status;

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
						clean_all(data, FAILURE, "minishell: func signal failed\n");
					create_heredoc(data, redir);
					heredoc_pid = fork();
					if (heredoc_pid == -1)
						clean_all(data, FAILURE, NULL);
					if (heredoc_pid == 0)
						fill_heredoc(data, redir);
					else
						waitpid(0, &status, 0);
					if (status != 0)
						return (status);
				}
				redir = redir->next;
			}
			cmd = cmd->next;
		}
		pipeline = pipeline->next;
	}
	return (SUCCESS);
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
