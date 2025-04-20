/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:20:23 by mlitvino, t       #+#    #+#             */
/*   Updated: 2025/04/20 15:41:29 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *env[])
{
	read_input(argc, argv, env);

	// printf("START\n");
	// system("ls -l /proc/self/fd");

	// int in = dup(STDIN);
	// int out = dup(STDOUT);

	// printf("\n\nTEST\n\n");
	// system("ls -l /proc/self/fd");

	// int test = open("out", O_RDWR);
	// dup2(test, STDOUT);
	// close(test);

	// int test2 = open("out2", O_RDWR);
	// dup2(test2, STDOUT);
	// close(test2);

	// dup2(out, STDOUT);

	// close(in);
	// close(out);

	// printf("END\n");
	// system("ls -l /proc/self/fd");

	return (0);
}
