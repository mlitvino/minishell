/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:06:58 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/09 15:23:59 by mlitvino         ###   ########.fr       */
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

void	create_temp_hd(t_data *data)
{
	int	i;

	i = 0;
}

void	heredoc(t_data *data, char **argv)
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

	}
}
