/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:29:19 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/18 18:11:33 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*----------------------------LIST OF INCLUDES--------------------------------*/
# define _POSIX_C_SOURCE 200000L
# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <signal.h>
# include <dirent.h>
# include <fcntl.h>

# include <termios.h>
# include <term.h>
# include <curses.h>
# include <termcap.h>
# include <sys/ioctl.h>

/*----------------------------BUILTIN STRING KEY------------------------------*/
# define CD_STR "cd"
# define ECHO_STR "echo"
# define ENV_STR "env"
# define EXIT_STR "exit"
# define EXPORT_STR "export"
# define PWD_STR "pwd"
# define UNSET_STR "unset"

/*----------------------------MEGA BEUTIFIER MOD------------------------------*/
# define GREEN "\e[1;32m"
# define RESET "\e[0m"
# define RED "\e[1;91m"
# define CYAN "\e[1;36m"
# define YELLOW "\e[1;33m"
# define PURPLE "\e[1;35m"
# define BLUE "\e[1;34m"

/*------------------------DEFAULT STREAMS DEFFINIITON-------------------------*/
# define STDIN 0
# define STDOUT 1
# define STDERR 2

/*---------------------------------TOKENS-------------------------------------*/
typedef enum e_token_type{
	NONE,
	WORD,
	SEMI,
	GREAT,
	DOUBLE_GREAT,
	LESS,
	DOUBLE_LESS,
	PIPE,
}			t_token_type;

typedef struct s_token
{
	int				index;
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}				t_token;

/*---------------------------------PARSER-------------------------------------*/

typedef struct s_data t_data;

typedef enum e_redir_type
{
	RE_GREAT,
	RE_DOUBLE_GREAT,
	RE_LESS,
	RE_DOUBLE_LESS,
}			t_redir_type;

typedef struct s_pipe
{
	int		pipe[2];
	int		count;
}	t_pipe;

typedef struct s_redir
{
	int				index;
	t_redir_type	type;
	int				inside_quotes;
	char			*file_name;
	struct s_redir	*next;

	int				fd;
	char			*delim;
}				t_redir;

typedef struct s_args
{
	int				inside_quotes;
	char			*value;
	struct s_args	*next;
}				t_args;

typedef struct s_builtin
{
	char	*name;
	void	(*func)(t_data *data, t_args *args);
}	t_builtin;

typedef struct s_simple_cmd
{
	int					inside_quotes;
	char				*command;
	t_args				*args;
	t_redir				*redirections;
	struct s_simple_cmd	*next;

	t_pipe				*pipes;
	int					cmd_i;
	int					cmd_count;
	t_list		*env;
	t_list		*local_vars;
	t_builtin	*builtin_arr;
	int					std_fd[2];
}				t_simple_cmd;

typedef struct s_pipe_line
{
	int					simple_cmd_count;
	struct s_pipe_line	*next;
	t_simple_cmd		*child;

	int					*exit_status;
	pid_t				*pid_last_cmd;
}				t_pipe_line;

typedef struct s_cmd_list
{
	int					pipe_line_count;
	t_pipe_line			*childs;
}				t_cmd_list;

typedef struct s_data
{
	t_cmd_list	*cmd_list;
	t_builtin	*builtin_arr;

	t_list		*env;
	t_list		*local_vars;

	char		*read_line;
}	t_data;



/*!!!!!!!!!!!!!!!!!!!!!!!---------------------------------MAIN PART-------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

extern volatile sig_atomic_t g_signal_received;

// typedef struct s_file
// {
// 	char	*path_name;
// 	int		fd;
// 	int		mod;
// }	t_file;

// typedef struct s_cmd
// {
// 	char	*cmd_path;
// 	char	*cmd_name;
// 	int		cmd_num;
// 	char	*optns;
// 	int		wildcard;
// 	int		is_builtin;
// 	char	**arg_list; // cmd_name + optns + args
// 	t_file	*infile;
// 	t_file	*outfile;
// }	t_cmd;

// typedef struct s_cmd_tab
// {
// 	t_cmd	*cmds;
// 	int		cmd_count;
// 	int		exit_code;
// 	t_pipe	*pipes;
// }	t_cmd_tab;

// readline.c
int			is_builtin(t_builtin *arr, char	*cmd_name);
t_builtin	*set_builtins(t_data *data);
void		read_input(int argc, char *argv[], char *env[]);

// heredoc.c
void	hd_sig_hanlder(int sig);
void	fill_heredoc(t_redir *heredoc);
void	create_heredoc(t_redir *heredoc);
void	check_create_heredoc(t_pipe_line *pipeline);
void	unlink_heredoc(t_pipe_line *pipeline);

// signals.c
void	sig_handler(int	sig, siginfo_t *info, void	*context);
void	init_sigs(t_data *data);

// utils.c
void	clean_all(t_data *data);
char	**convrt_args_to_argv(t_args *args);
char	**convrt_lst_to_argv(t_list *lst);
char	*expand_var(t_data *data, char *var);

/*------------------------------EXECUTOR--------------------------------------*/
void	is_executable(const char *name, t_data *data);

// executor_redirect.c
t_pipe	*init_pipes(int	cmd_count);
void	close_pipes(t_pipe *pipes, int pipes_count);
void	restart_fd(t_simple_cmd *cmd, int *std_fd);
void	redirect(t_simple_cmd *cmd, t_redir *redirs);

// executor.c
void	run_cmd(t_simple_cmd *cmd);
void	exec_simpl_cmd(t_data *data, t_simple_cmd *cmd, pid_t *pid_last_cmd);
void	exec_pipeline(t_data *data, t_pipe_line *pipeline, int cmd_count);
int		executor(t_data *data, t_cmd_list *cmd_list);

/*------------------------------BUILTINS--------------------------------------*/
// cmd_cd.c
int			is_new_line(char *option);
void		update_oldpwd(t_data *data);
void		cmd_cd(t_data *data, t_args *args);
// cmd_echo.c
int			is_new_line(char *option);
void		cmd_echo(t_data *data, t_args *args);
// cmd_env.c
void		cpy_env(char *sys_env[], t_data *data);
void		cmd_env(t_data *data, t_args *args);
// cmd_exit.c
void		cmd_exit(t_data *data, t_args *args);
// cmd_export.c
t_list		*find_var(t_list **list, char *var, t_list **prev);
void		add_replce_var(t_list **linked_list, char *arg);
void		cmd_export(t_data *data, t_args *args);
// cmd_pwd.c
void		cmd_pwd(t_data *data, t_args *args);
// cmd_unset.c
void		delete_var(t_list **list, char *var);
void		cmd_unset(t_data *data, t_args *args);

/*----------------------------------PARSER------------------------------------*/
t_cmd_list	*ft_parser(t_token *tokens_list, int *status);
t_redir		*ft_create_redir(t_token **tokens, int index);
t_redir		*ft_insert_redir(t_redir *redir, t_token **tokens, int index);
void		ft_insert_arg(t_args *head, t_args *current_args);
t_args		*ft_create_arg(char *value);

/*------------------------------SYNTAX CHECKER--------------------------------*/
int			ft_syntax_check(t_token *tokens_list, int *status);
int			ft_check_word_semi_pipe_redir(t_token *tokens_list, t_token *token,
				int *status);
int			ft_check_backslash(char *word);
int			ft_check_closing_quotes(char *word);
int			quote_return(int quote);

/*-------------------------------TOKEN CHECKER--------------------------------*/
int			check_first_token(t_token *tokens_list, t_token *first_token,
				int *status);
int			check_token_next_pipe(t_token *tokens_list, t_token *token,
				int *status);
int			check_last_word_token(t_token *tokens_list, t_token *token,
				int *status);
int			check_word_token(t_token *tokens_list, t_token *token, int *status);
int			check_token_next_semi(t_token *tokens_list, t_token *token,
				int *status);

/*----------------------------CONT TOKEN CHECKER------------------------------*/
int			check_redir(t_token *tokens_list, t_token *token, int *status);
void		ft_print_systax_error(t_token *token);

/*--------------------------------WORD CHECKER--------------------------------*/
int			count_bachslashes(char *word, int *index, int back_slash);
int			ft_get_first_double_quotes(char *word, int *index, int *back_slash);
int			ft_get_first_single_quotes(char *word, int *index, int *back_slash);
void		ft_get_close_double_quotes(char *word, int *index, int *back_slash,
				int *quote);
void		ft_get_close_single_quotes(int *index, int *back_slash,
				int *quote);

/*-------------------------------CREATE ASTERIKS------------------------------*/
t_pipe_line	*ft_init_pipe_line(void);
t_pipe_line	*reset_pipe_line_head(t_pipe_line **pipe_line, t_simple_cmd *head);
t_pipe_line	*ft_create_pieline(t_token **tokens);
void		ft_create_next_pipeline(t_token *tokens_list, t_cmd_list **head);
t_cmd_list	*ft_create_asteriks(t_token *tokens_list);

/*--------------------------------CREATE NODES--------------------------------*/
t_cmd_list		*init_cmd_list(void);
t_simple_cmd	*ft_init_simple_cmd(void);
t_simple_cmd	*ft_insert_command_and_arguments(t_simple_cmd **cmd,
					t_token *token);
t_simple_cmd	*ft_create_simple_cmd(t_token **tokens);
void			ft_insert_simple_cmd(t_simple_cmd *head,
					t_simple_cmd *current_cmd);

/*-------------------------------DESTROY NODES--------------------------------*/
void		ft_destroy_redir(t_redir *redis);
void		ft_destroy_simple(t_simple_cmd *cmd);
void		ft_destroy_pipe_line(t_pipe_line *pipe_line);
void		ft_destroy_ast(t_cmd_list *cmd_list);

/*-----------------------------------LEXER------------------------------------*/
t_token			*ft_lexer(char *line);
void			ft_get_word(t_token *tokens_list, char *line, int *table);
void			get_space_pipe_semi_redir(t_token *tokens_list,
					char *line, int *j, int *index);
void			ft_destoy_token_list(t_token *tokens_list);
void			print_tokens(t_token *tokens_list);
void			add_token(t_token *token_list, t_token_type type,
					char *content, int index);
char			*ft_get_words(char *line, int *j, char *word, int *quoting);

// test.c
#    define TEST "Delete"
void	show_token(t_token *token);
void	show_arg(t_args *args);
void	show_redir(t_redir *redir);
void	show_cmd(t_simple_cmd *cmd);
void	show_cmd_list(t_cmd_list *list);

#endif
