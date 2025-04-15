/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:06:58 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/11 15:00:03 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hd_sig_hanlder(int sig)
{
		printf("\n");
		rl_replace_line("", 0);
		rl_redisplay();
		rl_on_new_line();
		exit(1);
}

void	create_temp_hd(t_data *data, t_file *infile)
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
	infile->path_name = file_name;
}

void	heredoc(t_data *data, char **argv, t_file *infile)
{
	char	*delim;
	char	*input;

	delim = argv[2];
	signal(SIGINT, hd_sig_hanlder);
	while (1)
	{
		input = readline("> ");
		if (!input)
			return ;
		if (ft_strcmp(input, delim) == 0)
			return ;
		infile->fd = open(infile->path_name, O_WRONLY);
		ft_putstr_fd(input, infile->fd);
		close(infile->fd);
	}
}
