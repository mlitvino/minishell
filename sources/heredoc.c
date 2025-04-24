/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:06:58 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/24 13:37:04 by mlitvino         ###   ########.fr       */
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
			clean_all(data, FAILURE, strerror(errno)); // change strerror
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

t_redir	*get_next_heredoc(t_data *data)
{
	static t_pipe_line	*curr_pipeline = NULL;
	static t_simple_cmd	*curr_cmd = NULL;
	static t_redir		*curr_redir = NULL;

	if (!curr_pipeline)
		curr_pipeline = data->cmd_list->childs;
	while (curr_pipeline)
	{
		if (!curr_cmd)
			curr_cmd = curr_pipeline->child;
		while(curr_cmd)
		{
			if (!curr_redir)
				curr_redir = curr_cmd->redirections;
			while (curr_redir)
				if (curr_redir->type == RE_DOUBLE_LESS)
					return (curr_redir);
			curr_cmd = curr_cmd->next;
		}
		curr_pipeline = curr_pipeline->next;
	}
	curr_pipeline = NULL;
	curr_cmd = NULL;
	curr_redir = NULL;
	return (NULL);
}

int	check_create_heredoc(t_data *data, t_pipe_line *pipeline)
{
	pid_t			heredoc_pid;
	t_redir			*heredoc;

	while (1)
	{
		heredoc = get_next_heredoc(data);
		if (heredoc)
		{
			if (signal(SIGINT, SIG_IGN) == SIG_ERR)
				clean_all(data, FAILURE, "minishell: func signal failed\n");
			create_heredoc(data, heredoc);
			heredoc_pid = fork();
			if (heredoc_pid == -1)
				clean_all(data, FAILURE, NULL);
			if (heredoc_pid == 0)
				fill_heredoc(data, heredoc);
			else
				waitpid(0, &pipeline->exit_status, 0);
			if (pipeline->exit_status != SUCCESS)
				return (pipeline->exit_status);
		}
		else
			return (SUCCESS);
	}
	return (SUCCESS);
}

void	unlink_heredoc(t_data *data, t_pipe_line *pipeline)
{
	t_redir *heredoc;

	while (1)
	{
		heredoc = get_next_heredoc(data);
		if (heredoc)
		{
			unlink(heredoc->file_name);
		}
		else
		{
			return ;
		}
	}
}
