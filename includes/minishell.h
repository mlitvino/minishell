/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:29:19 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/11 17:26:37 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*----------------------------LIST OF INCLUDES--------------------------------*/
# define _POSIX_C_SOURCE 200000L
# include "libft.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termcap.h>
# include <termios.h>

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

/*-------------------------------EXIT_CODES-----------------------------------*/

# define SUCCESS 0
# define FAILURE 1
# define MISUSE 2
# define CRIT_ERR 3
# define CMD_NOT_EXEC 126
# define CMD_NOT_FOUND 127
# define TERM_SIGINT 130
# define OUT_RANGE 255

/*---------------------------------TOKENS-------------------------------------*/
typedef enum e_token_type
{
	NONE,
	WORD,
	SEMI,
	GREAT,
	DOUBLE_GREAT,
	LESS,
	DOUBLE_LESS,
	PIPE,
}								t_token_type;

typedef struct s_token
{
	int							index;
	t_token_type				type;
	char						*value;
	struct s_token				*next;
}								t_token;

/*---------------------------------PARSER-------------------------------------*/

typedef struct s_data			t_data;

typedef enum e_redir_type
{
	RE_GREAT,
	RE_DOUBLE_GREAT,
	RE_LESS,
	RE_DOUBLE_LESS,
}								t_redir_type;

typedef struct s_pipe
{
	int							pipe[2];
	int							count;
}								t_pipe;

typedef struct s_redir
{
	int							index;
	t_redir_type				type;
	int							inside_quotes;
	char						*file_name;
	struct s_redir				*next;

	int							fd;
	int							existing;
	char						*delim;
}								t_redir;

typedef struct s_args
{
	int							inside_quotes;
	char						*value;
	struct s_args				*next;
}								t_args;

typedef struct s_builtin
{
	char						*name;
	int							(*func)(t_data *data, t_args *args);
}								t_builtin;

typedef struct s_simple_cmd
{
	int							inside_quotes;
	char						*command;
	t_args						*args;
	t_redir						*redirections;
	struct s_simple_cmd			*next;

	int							exit_code;
	char						*pathname;
	int							cmd_i;
	int							cmd_count;
	t_builtin					*builtin_arr;
	int							std_fd[2];
	pid_t						cmd_pid;
}								t_simple_cmd;

typedef struct s_pipe_line
{
	int							simple_cmd_count;
	struct s_pipe_line			*next;
	t_simple_cmd				*child;

	int							exit_status;
	pid_t						*pid_last_cmd;
}								t_pipe_line;

typedef struct s_cmd_list
{
	int							pipe_line_count;
	t_pipe_line					*childs;
}								t_cmd_list;

typedef struct s_data
{
	t_cmd_list					*cmd_list;
	t_builtin					*builtin_arr;
	t_pipe						*pipes;

	t_list						*env;

	int							exit_var;

	char						*read_line;
}								t_data;

/*!!!!-------------------MAIN PART---------------------!!!!*/

extern volatile sig_atomic_t	g_signal_received;

// inits.c
void							init_sigs(t_data *data);
t_pipe							*init_pipes(t_data *data, int cmd_count);
void							init_builtins(t_data *data);
void							init_data(t_data *data, char **sys_env);

// expand.c
char							*find_var_value(t_data *data, char *env_var);
int								get_i_end_token(char *str, char token);
char							*substr_value(t_data *data, char *var,
									int i);
char							*expand_var(t_data *data, char *var);
char							*expand_str(t_data *data, char *orig_str,
									char *new_str, int skip_quot);

// heredoc_trim.c
int								is_valid(char *char_ptr);
char							*trim_delim(t_redir *heredoc);

// heredoc.c
void							fill_heredoc(t_data *data, t_redir *heredoc);
void							create_heredoc(t_data *data, t_redir *heredoc);
int								check_create_heredoc(t_data *data,
									t_redir *heredoc);
int								unlink_heredoc(t_data *data, t_redir *heredoc);

// heredoc_utils.c
char							*check_hd_input(t_data *data, t_redir *heredoc,
									char *input);
int								map_heredoc(t_data *data,
									int (*func)(t_data *data,
										t_redir *heredoc));
int								bzero_existing(t_data *data, t_redir *heredoc);

// quotes.c
void							check_quote_flg(char *str, int *quote_flg);
char							*trim_quotes(t_data *data, char *str);
char							*get_next_piece(t_data *data, char *orig_str,
									int *orig_i);
char							*trim_expand(t_data *data, char *orig_str);
int								check_quots_expand(t_data *data,
									t_simple_cmd *cmd);

// signals.c
void							hd_sig_hanlder(int sig);
void							sig_handler(int sig, siginfo_t *info,
									void *context);

// utils_clean.c
void							print_strs_fd(char *s1, char *s2, char *s3,
									int fd);
void							free_argv(char **argv);
void							free_redir(t_redir *redir);
void							free_args(t_args *args);
void							free_cmds(t_simple_cmd *cmd);
void							free_cmd_list(t_data *data,
									t_cmd_list *cmd_list);
int								clean_all(t_data *data, int exit_code,
									char *err_message);

// utils.c
int								args_size(t_args *args);
char							**convrt_args_to_argv(t_args *args,
									char *cmd_name);
char							**convrt_lst_to_argv(t_list *lst);
int								is_builtin(t_builtin *arr, char *cmd_name);

/*------------------------------EXECUTOR--------------------------------------*/

// check_cmd.c
void							check_empty(t_simple_cmd *cmd);
t_simple_cmd					*init_null_cmd(t_simple_cmd **curr_cmd);
void							check_cmd(t_data *data, t_simple_cmd *curr_cmd,
									int cmd_count, int i);

// executor_utils.c
int								wait_get_exitcode(t_data *data,
									pid_t child_pid);
void							wait_childs(t_data *data,
									t_simple_cmd *lst_cmd);

// executor.c
void							execve_cmd(t_data *data, t_simple_cmd *cmd,
									int builtin_i);
int								exec_simpl_cmd(t_data *data, t_simple_cmd *cmd);
void							exec_pipeline(t_data *data,
									t_pipe_line *pipeline, int cmd_count);
int								executor(t_data *data, t_cmd_list *cmd_list);

// redirect.c
void							close_pipes(t_data *data, int pipes_count);
void							restart_fd(t_data *data, t_simple_cmd *cmd);
int								redirect_files(t_redir *redir);
int								redirect(t_data *data, t_simple_cmd *cmd,
									t_redir *redirs);

// search_exec.c
int								check_access(t_simple_cmd *cmd);
int								check_path_dirs(t_data *data, t_simple_cmd *cmd,
									char **path_tab);
int								search_exec(t_data *data, t_simple_cmd *cmd);

// search_utils.c
void							join_pathname(t_data *data, t_simple_cmd *cmd,
									char *path_str);
char							**create_path_tab(t_data *data);

/*------------------------------BUILTINS--------------------------------------*/
// cmd_cd_utils.c
int								set_path(t_data *data, t_args *args);
// cmd_cd.c
int								join_paste_var(t_data *data, char *key_var,
									char *var_value);
int								update_pwd(t_data *data);
int								update_oldpwd(t_data *data, t_list *env);
char							*get_home_path(t_data *data);
int								cmd_cd(t_data *data, t_args *args);
// cmd_echo.c
t_args							*is_option(t_args *args, int *option);
int								print_args(t_args *args);
int								cmd_echo(t_data *data, t_args *args);
// cmd_env.c
void							init_env(char *sys_env[], t_data *data);
int								cmd_env(t_data *data, t_args *args);
// cmd_exit.c
char							exit_atoi(t_data *data, char *str);
int								cmd_exit(t_data *data, t_args *args);
// cmd_export.c
t_list							*find_var(t_list *list, char *var,
									t_list **prev);
t_list							*add_replce_var(t_list **list, char *new_var);
int								check_export_arg(t_args *args, int *exit_code);
int								cmd_export(t_data *data, t_args *args);
// cmd_pwd.c
int								cmd_pwd(t_data *data, t_args *args);
// cmd_unset.c
void							delete_var(t_list **list, char *var);
int								cmd_unset(t_data *data, t_args *args);

/*----------------------------------PARSER------------------------------------*/
t_cmd_list						*ft_parser(t_data *data,
									t_token *tokens_list, int *status);
t_redir							*ft_create_redir(t_token **tokens, int index);
t_redir							*ft_insert_redir(t_redir *redir,
									t_token **tokens, int index);
void							ft_insert_arg(t_args *head,
									t_args *current_args);
t_args							*ft_create_arg(char *value);
char							*extract_substr(char *str, int *curr_i);
void							*expand_tokens_list(t_data *data,
									t_token **tokens);
void							*expnd_token_value(t_data *data, t_token *token,
									int *empty_flag, int *curr_i);
void							join_tokens(t_token **current,
									t_token **temp_prev,
									t_token *new_tokens, int empty_flag);

/*------------------------------SYNTAX CHECKER--------------------------------*/
int								ft_syntax_check(t_token *tokens_list,
									int *status);
int								ft_check_word_semi_pipe_redir(
									t_token *tokens_list,
									t_token *token, int *status);
int								ft_check_backslash(char *word);
int								ft_check_closing_quotes(char *word);
int								quote_return(int quote);

/*-------------------------------TOKEN CHECKER--------------------------------*/
int								check_first_token(t_token *tokens_list,
									t_token *first_token, int *status);
int								check_token_next_pipe(t_token *tokens_list,
									t_token *token, int *status);
int								check_last_word_token(t_token *tokens_list,
									t_token *token, int *status);
int								check_word_token(t_token *tokens_list,
									t_token *token, int *status);
int								check_token_next_semi(t_token *tokens_list,
									t_token *token, int *status);

/*----------------------------CONT TOKEN CHECKER------------------------------*/
int								check_redir(t_token *tokens_list,
									t_token *token, int *status);
void							ft_print_systax_error(t_token *token);

/*--------------------------------WORD CHECKER--------------------------------*/
int								count_bachslashes(char *word, int *index,
									int back_slash);
int								ft_get_first_double_quotes(char *word,
									int *index, int *back_slash);
int								ft_get_first_single_quotes(char *word,
									int *index, int *back_slash);
void							ft_get_close_double_quotes(char *word,
									int *index, int *back_slash, int *quote);
void							ft_get_close_single_quotes(int *index,
									int *back_slash, int *quote);

/*-------------------------------CREATE ASTERIKS------------------------------*/
t_pipe_line						*ft_init_pipe_line(void);
t_pipe_line						*reset_pipe_line_head(t_pipe_line **pipe_line,
									t_simple_cmd *head);
t_pipe_line						*ft_create_pieline(t_token **tokens);
void							*ft_create_next_pipeline(t_token *tokens_list,
									t_cmd_list **head);
t_cmd_list						*ft_create_asteriks(t_token *tokens_list);

/*--------------------------------CREATE NODES--------------------------------*/
t_cmd_list						*init_cmd_list(void);
t_simple_cmd					*ft_init_simple_cmd(void);
t_simple_cmd					*ft_insert_command_and_arguments(
									t_simple_cmd **cmd, t_token *token);
t_simple_cmd					*ft_create_simple_cmd(t_token **tokens);
void							ft_insert_simple_cmd(t_simple_cmd *head,
									t_simple_cmd *current_cmd);

/*-------------------------------DESTROY NODES--------------------------------*/
void							ft_destroy_redir(t_redir *redis);
void							ft_destroy_simple(t_simple_cmd *cmd);
void							ft_destroy_pipe_line(t_pipe_line *pipe_line);
void							ft_destroy_ast(t_cmd_list *cmd_list);

/*-----------------------------------LEXER------------------------------------*/
t_token							*ft_lexer(char *line);
void							*ft_get_word(t_token *tokens_list, char *line,
									int *table);
void							*get_space_pipe_semi_redir(t_token *tokens_list,
									char *line, int *j, int *index);
void							ft_destoy_token_list(t_token *tokens_list);
void							print_tokens(t_token *tokens_list);
void							*add_token(t_token *token_list,
									t_token_type type, char *content,
									int index);
char							*ft_get_words(char *line, int *j, char *word,
									int *quoting);
char							*get_no_quoting_word(char *line, int *i);

// test.c
# define TEST "Delete"
void							show_token(t_token *token);
void							show_arg(t_args *args);
void							show_redir(t_redir *redir);
void							show_cmd(t_simple_cmd *cmd);
void							show_cmd_list(t_cmd_list *list);

#endif
