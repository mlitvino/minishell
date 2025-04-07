/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:38:12 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/07 18:17:46 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	sig_handler(int	sig, siginfo_t *info, void	*context)
{
	if (sig == SIGINT)
	{
		printf("\n");
		printf("minishell$ ");
	}
}

void	init_sigs(t_data *data)
{
	struct sigaction	sa;
	struct sigaction	ig;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);

	sigaction(SIGINT, &sa, NULL);
	// err check
	signal(SIGQUIT, SIG_IGN);
}
