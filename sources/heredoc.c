/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:06:58 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/11 16:56:28 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_heredoc(t_data *data, t_redir *heredoc)
{
	char	*input;

	if (signal(SIGINT, hd_sig_hanlder) == SIG_ERR)
		clean_all(data, FAILURE, "Error: func signal failed\n");
	while (1)
	{
		input = readline("> ");
		input = check_hd_input(data, heredoc, input);
		heredoc->fd = open(heredoc->file_name, O_WRONLY | O_APPEND);
		if (heredoc->fd == -1)
		{
			free(input);
			perror("minishell: heredoc file");
			clean_all(data, FAILURE, NULL);
		}
		ft_putendl_fd(input, heredoc->fd);
		free(input);
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
		file_id = ft_itoa(i++);
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
		free(heredoc->file_name);
	}
	heredoc->existing = 1;
	close(heredoc->fd);
}

void	fork_heredoc(t_data *data, t_redir *heredoc)
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
		signal(SIGINT, SIG_IGN);
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
	if (trim_delim(heredoc) == NULL)
		clean_all(data, FAILURE, "minishell: heredoc: malloc failed\n");
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		clean_all(data, FAILURE, "minishell: func signal failed\n");
	create_heredoc(data, heredoc);
	fork_heredoc(data, heredoc);
	if (data->exit_var == TERM_SIGINT)
		return (TERM_SIGINT);
	init_sigs(data);
	return (SUCCESS);
}

int	unlink_heredoc(t_data *data, t_redir *heredoc)
{
	(void)data;
	if (heredoc->existing == 1)
	{
		unlink(heredoc->file_name);
	}
	return (SUCCESS);
}
