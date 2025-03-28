/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/03/28 14:12:45 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipex(char *argv[])
{
	// Check whether we have permissions and printf accord error
	access("test", F_OK | R_OK | W_OK | X_OK);
	perror("access");

	// Get file fd, stdin fd
	int file = open("te", O_RDWR);
	int tempin = dup(1);

	// Redirect stdin > file, write phare, recover stdin
	dup2(file, 1);
	write(1, "nomer\n", 6);
	dup2(tempin, 1);

	close(file);
	close(tempin);
}
