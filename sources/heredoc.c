/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:06:58 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/27 20:50:01 by mlitvino         ###   ########.fr       */
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
		{
			perror("minishell: temp heredoc");//dell
			clean_all(data, FAILURE, NULL); // change strerror
		}
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
			break ;
		else if (heredoc->fd == -1 && errno != EEXIST)
		{
			perror("minishell: heredoc: open");
			clean_all(data, FAILURE, NULL);
		}
		i++;
	}
	heredoc->existing = 1;
	close(heredoc->fd);
}

void	fork_heredoc(t_data *data, t_redir *heredoc, int *exit_status)
{
	pid_t	heredoc_pid;

	heredoc_pid = fork();
	if (heredoc_pid == -1)
	{
		perror("minishell: heredoc: fork");
		clean_all(data, FAILURE, NULL);
	}
	if (heredoc_pid == 0)
	{
		map_heredoc(data, bzero_existing);
		fill_heredoc(data, heredoc);
		clean_all(data, SUCCESS, NULL);
	}
	else
	{
		wait_get_exitcode(data, heredoc_pid);
	}
	if (data->exit_var == FAILURE)
		clean_all(data, FAILURE, NULL);
	init_sigs(data);
}

int	check_create_heredoc(t_data *data, t_redir *heredoc)
{
	heredoc->delim = heredoc->file_name;
	heredoc->file_name = NULL;
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		clean_all(data, FAILURE, "minishell: func signal failed\n");
	create_heredoc(data, heredoc);
	fork_heredoc(data, heredoc, &data->exit_var);
	if (data->exit_var == TERM_SIGINT)
		return (TERM_SIGINT);
	init_sigs(data);
	return (SUCCESS);
}

int	unlink_heredoc(t_data *data, t_redir *heredoc)
{
	if (heredoc->existing == 1)
	{
		unlink(heredoc->file_name);
	}
	return (SUCCESS);
}


