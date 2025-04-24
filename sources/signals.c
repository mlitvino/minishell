/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:38:12 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/24 13:41:53 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	sig_handler(int	sig, siginfo_t *info, void *context)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_signal_received = 1;
	}
}

void	init_sigs(t_data *data)
{
	struct sigaction	sa;
	struct sigaction	ig;
	int					return_code;

	return_code = 0;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	return_code |= sigemptyset(&sa.sa_mask);
	return_code |= sigaddset(&sa.sa_mask, SIGINT);
	return_code |= sigaction(SIGINT, &sa, NULL);
	if (return_code != 0 || signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("minishell: init_sigs");
		clean_all(data, FAILURE, NULL);
	}
}
