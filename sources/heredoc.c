/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:06:58 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/08 21:18:42 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc(t_data *data, char **argv)
{
	char	*delim;
	char	*input;
	int		pipefd[2];

	pipe(pipefd);
	delim = argv[2];
	signal(SIGINT, SIG_DFL);
	input = readline("> ");
	while (ft_strcmp(input, delim) != 0)
	{
		input = readline("> ");
	}
}
