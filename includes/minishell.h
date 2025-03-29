/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:29:19 by mlitvino          #+#    #+#             */
/*   Updated: 2025/03/29 00:04:40 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>

# include <fcntl.h>
# include <errno.h>

# include <sys/wait.h>

// readline.c
void	read_input(int argc, char *argv[], char *env[]);
char	*readline(const char *prompt);

// cmd_exit.c
void	cmd_exit(void);

// pipe.c
void	pipex(char *read_line);

#endif
