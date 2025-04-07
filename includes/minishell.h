/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:29:19 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/07 14:08:04 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>

# define STDIN 0
# define STDOUT 1
#define STDERR 2 // do we need it?

typedef struct s_pipe
{
	long	pipe[2];
}	t_pipe;

typedef struct s_file
{
	char	*path_name;
	int		fd;
	int		mod;
}	t_file;

typedef struct s_cmd
{
	char	*cmd_path;
	char	*cmd_name;
	char	*optns;
	int		wildcard;
	char	**arg_list; // cmd_name + optns + args
	t_file	*infile;
	t_file	*outfile;
}	t_cmd;

typedef struct s_cmd_tab
{
	t_cmd	*cmds;
	int		cmd_count;
	int		exit_code;
	t_pipe	*pipes;
}	t_cmd_tab;

typedef struct s_data
{
	t_cmd_tab	*cmd_flows;
	t_list		*env;
	t_list		*local_vars;
}	t_data;

// readline.c
void	read_input(int argc, char *argv[], char *env[]);
char	*readline(const char *prompt);

//cmd_cd.c
void	cmd_cd(t_data *data, char *path);

//cmd_echo.c
int		is_new_line(char *option);
void	cmd_echo(char **argv);

//cmd_env.c
void	cpy_env(char *sys_env[], t_data *data);
void	cmd_env(t_list *env);

//cmd_export.c
void	add_replce_var(t_list **linked_list, char *arg);
void	cmd_export(t_data *data, char *arg);

//cmd_pwd.c
void	cmd_pwd(t_data *data);

//cmd_unset.c
void	cmd_unset(t_data *data, char *arg);

// cmd_exit.c
void	cmd_exit(void);

#endif
