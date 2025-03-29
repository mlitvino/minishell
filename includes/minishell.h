/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:29:19 by mlitvino          #+#    #+#             */
/*   Updated: 2025/03/29 11:08:54 by mlitvino         ###   ########.fr       */
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
	int		mod; // echo 123 > file - overwrite file. echo 123 >> file - add text to end
}	t_file;

typedef struct s_cmd_tab
{
	char	*cmd1;
	char	**pipes_cmd;
	t_file	*infile;
	t_file	*outfile;
}	t_cmd_tab;

typedef struct s_data
{
	t_cmd_tab	**cmd_flow; /* (infile < cmd1 | cmd 2 | cmd3 > outfile) - 1 flow
							(cmd1 | cmd2) && (cmd1 > outfile) - 2 flows*/
}	t_data;

// readline.c
void	read_input(int argc, char *argv[], char *env[]);
char	*readline(const char *prompt);

// cmd_exit.c
void	cmd_exit(void);

#endif
