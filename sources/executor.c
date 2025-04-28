/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/28 19:43:22 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_cmd(t_data *data, t_simple_cmd *cmd)
{
	char	**argv;
	char	**env;

	/*
	void	*builtin_func;

	if (builtin_i != -1)
	{
		builtin_func = cmd->builtin_arr[builtin_i].func(data, cmd->args)
		clean_all(data, builtin_func, NULL);
	}

	*/
	argv = convrt_args_to_argv(cmd->args, cmd->command);
	env = convrt_lst_to_argv(data->env);
	if (!env || !argv)
	{
		free_argv(env);
		free_argv(argv);
		ft_putstr_fd(cmd->command, 2);
		clean_all(data, FAILURE, ": malloc failed\n");
	}
	execve(cmd->pathname, argv, env);
	perror("execve");
	clean_all(data, FAILURE, NULL);
}

int	exec_simpl_cmd(t_data *data, t_simple_cmd *cmd)
{
	int	builtin_i;

	builtin_i = is_builtin(cmd->builtin_arr, cmd->command);
	if (builtin_i != -1)
		cmd->exit_code = cmd->builtin_arr[builtin_i].func(data, cmd->args);
	else
	{
		cmd->cmd_pid = fork();
		if (cmd->cmd_pid == -1)
		{
			perror("minishell: fork");
			return (FAILURE);
		}
		if (cmd->cmd_pid == 0)
		{
			close(cmd->std_fd[STDIN]);
			cmd->std_fd[STDIN] = -1;
			close(cmd->std_fd[STDOUT]);
			cmd->std_fd[STDIN] = -1;
			close_pipes(data, cmd->cmd_count - 1);
			if (search_exec(data, cmd) == SUCCESS)
				execve_cmd(data, cmd);
		}
	}
	/*

	if (builtin_i == -1 || cmd->cmd_count > 1)
	{
		cmd->cmd_pid = fork();
		if (cmd->cmd_pid == -1)
		{
			perror("minishell: fork");
			return (FAILURE);
		}
		if (cmd->cmd_pid == 0)
		{
			close(cmd->std_fd[STDIN]);
			cmd->std_fd[STDIN] = -1;
			close(cmd->std_fd[STDOUT]);
			cmd->std_fd[STDIN] = -1;
			close_pipes(data, cmd->cmd_count - 1);
			if (builtin_i != -1 || search_exec(data, cmd) == SUCCESS)
				execve_cmd(data, cmd);
		}
	}
	else
	{
		cmd->exit_code = cmd->builtin_arr[builtin_i].func(data, cmd->args);
	}

	*/

	return (SUCCESS);
}

// char	*expand_var(t_data *data, char *str, int *curr_i)
// {
// 	char	*env_var;
// 	int		i;
// 	char	*value;

// 	if (ft_strncmp(str, "$?", 2) == 0)
// 		return (ft_itoa(data->exit_var));
// 	i = 1;
// 	if (ft_isdigit(str[i]) == 1)
// 		return (ft_stdup(""));
// 	while (str[i] && ft_isalnum(str[i]) == 1)
// 		i++;
// 	if (curr_i)
// 		*curr_i += i;
// 	env_var = ft_substr(str, 1, i);
// 	if (!env_var)
// 		return (NULL);
// 	if (find_var(data->env, env_var, NULL) == NULL) // fix find_var
// 		value = ft_strdup("");
// 	else
// 		value = ft_strdup(find_var(data->env, env_var, NULL)->content);
// 	free(env_var);
// 	return (value);
// }

char	*expand_var(t_data *data, char *str)
{
	char	*env_var;
	int		i;
	char	*value;

	i = 1;
	if (ft_strncmp(str, "$?", 2) == 0)
		return (ft_itoa(data->exit_var));
	if (ft_isdigit(str[i]) == 1)
		return (ft_stdup(""));
	while (str[i] && ft_isalnum(str[i]) == 1)
		i++;
	if (i == 1)
		return (ft_strdup("$"));
	env_var = ft_substr(str, 1, i);
	if (!env_var)
		return (NULL);
	if (find_var(data->env, env_var, NULL) == NULL) // fix find_var
		value = ft_strdup("");
	else
		value = ft_strdup(find_var(data->env, env_var, NULL)->content);
	free(env_var);
	return (value);
}

char	*trim_quotes(t_data *data, char *orig_str, int *orig_i)
{
	int		curr_i;
	char	*new_str;
	char	*piece_str;

	curr_i = 0;
	new_str = NULL;
	if (orig_str[curr_i] == '\'')
		return (ft_strtrim(orig_str, "\'"));
	else
		new_str = ft_strtrim(orig_str, "\"");
	while (orig_str && orig_str[curr_i])
	{
		piece_str = get_next_piece(data, orig_str, &curr_i);
		new_str = ft_strjoin(new_str, piece_str);
		if (!new_str || !piece_str)
		{
			free(piece_str);
			clean_all(data, FAILURE, NULL);
		}
		free(piece_str);
	}
	return (new_str);
}

char	*get_next_piece(t_data *data, char *orig_str, int *orig_i)
{
	int		curr_i;
	char	*res;

	curr_i = *orig_i;
	while (orig_str[curr_i] && orig_str[curr_i] != '$'
		&& orig_str[curr_i] != '\'' && orig_str[curr_i] != '\"')
		curr_i++;
	if (curr_i != 0)
		res = ft_substr(orig_str, *orig_i, curr_i);
	else if (orig_str[curr_i] == '$')
	{
		res = expand_var(data, &orig_str[curr_i], orig_i);
		while (orig_str[++curr_i] != orig_str[*orig_i])
			curr_i++;
	}
	else if (orig_str[curr_i] == '\'' || orig_str[curr_i] == '\"')
	{
		res = trim_quotes(data, &orig_str[curr_i], orig_i);
		while (orig_str[curr_i] != orig_str[*orig_i])
			curr_i++;
	}
	*orig_i += curr_i;
	return (res);
}

char	*trim_expand(t_data *data, char *orig_str)
{
	int		curr_i;
	char	*new_str;
	char	*piece_str;

	curr_i = 0;
	new_str = NULL;
	if (*orig_str == '\0')
		return (orig_str);
	while (orig_str && orig_str[curr_i])
	{
		piece_str = get_next_piece(data, orig_str, &curr_i);
		new_str = ft_strjoin(new_str, piece_str);
		if (!new_str || !piece_str)
		{
			free(piece_str);
			clean_all(data, FAILURE, NULL);
		}
		free(piece_str);
	}
	free(orig_str);
	return (new_str);
}

void	check_quote_flg(char *str, int *quote_flg)
{
	if (ft_strchr(str, '\'') != NULL)
		*quote_flg = 1;
	else if (ft_strchr(str, '\"') != NULL)
		*quote_flg = 1;
}

int	check_quots_expand(t_data *data, t_simple_cmd *cmd)
{
	t_args	*args;
	t_redir	*redir;

	check_quote_flg(cmd->command, cmd->inside_quotes);
	cmd->command = trim_expand(data, cmd->command);
	args = cmd->args;
	while (args)
	{
		check_quote_flg(args->value, args->inside_quotes);
		args->value = trim_expand(data, args->value);
		args = args->next;
	}
	redir = cmd->redirections;
	while (redir)
	{
		check_quote_flg(redir->file_name, redir->inside_quotes);
		redir->file_name = trim_expand(data, redir->file_name);
		redir = redir->next;
	}
	// check_move all
	return (SUCCESS);
}

void	exec_pipeline(t_data *data, t_pipe_line *pipeline, int cmd_count)
{
	t_simple_cmd	*curr_cmd;
	int				i;

	if (cmd_count > 1)
		init_pipes(data, cmd_count - 1);
	curr_cmd = pipeline->child;
	i = 0;
	while (i < pipeline->simple_cmd_count)
	{
		curr_cmd->builtin_arr = data->builtin_arr;
		curr_cmd->cmd_count = cmd_count;
		curr_cmd->cmd_i = i;
		check_quots_expand(data, curr_cmd);
		curr_cmd->exit_code = redirect(data, curr_cmd, curr_cmd->redirections); // chang return value
		if (curr_cmd->exit_code == SUCCESS)
			exec_simpl_cmd(data, curr_cmd);
		restart_fd(data, curr_cmd);
		if (curr_cmd->next)
			curr_cmd = curr_cmd->next;
		i++;
	}
	wait_childs(data, curr_cmd);
	close_pipes(data, cmd_count - 1);
}

int	executor(t_data *data, t_cmd_list *cmd_list)
{
	t_pipe_line	*pipeline;

	pipeline = cmd_list->childs;
	map_heredoc(data, check_create_heredoc);
	while (data->exit_var != TERM_SIGINT && pipeline)
	{
		exec_pipeline(data, pipeline, pipeline->simple_cmd_count);
		pipeline = pipeline->next;
	}
	map_heredoc(data, unlink_heredoc);
	return (SUCCESS);
}
