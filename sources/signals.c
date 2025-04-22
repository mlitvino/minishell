/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:38:12 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/22 15:11:50 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	sig_handler(int	sig, siginfo_t *info, void	*context)
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

int	init_sigs(void)
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
	signal(SIGQUIT, SIG_IGN);
	if (return_code != 0 || signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (FAILURE);
	return (SUCCESS);
}
