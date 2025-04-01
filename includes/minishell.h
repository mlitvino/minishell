/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:29:19 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/01 12:33:00 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_file
{
	char	*path_name;
	int		fd;
	int		mod;
}	t_file;

typedef struct s_cmd
{
	char	*cmd_name;
	char	*optns;
	int		wildcard;
}	t_cmd;

typedef struct s_cmd_tab
{
	t_cmd	*cmds;
	t_file	*infile;
	t_file	*outfile;
	int		exit_code;
}	t_cmd_tab;

typedef struct s_data
{
	t_cmd_tab	*cmd_flow;
}	t_data;

// readline.c
void	read_input(int argc, char *argv[], char *env[]);
char	*readline(const char *prompt);

// cmd_exit.c
void	cmd_exit(void);

#endif
