/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:30:49 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/18 14:02:26 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_all(t_data *data)
{
	int				exit_status;
	t_pipe_line		*pipeline;
	t_simple_cmd	*cmd;
	t_redir			*redir;
	t_args			*args;

	exit_status = 0; // change

	free(data->read_line);
	data->read_line = NULL;

	ft_lstclear(&data->local_vars, free);
	data->local_vars = NULL;

	ft_lstclear(&data->env, free);
	data->env = NULL;

	free(data->builtin_arr);
	data->builtin_arr = NULL;

	pipeline = data->cmd_list->childs;
	while (pipeline)
	{
		cmd = pipeline->child;
		while (cmd)
		{
			redir = cmd->redirections;
			while (redir)
			{
				free(redir->file_name);
				redir->file_name = NULL;

				free(redir->delim);
				redir->delim = NULL;

				redir = redir->next;
			}

			args = cmd->args;
			while (args)
			{
				free(args->value);
				args->value = NULL;
				args = args->next;
			}

			free(cmd->command);
			cmd->command = NULL;

			// close_pipes(cmd->pipes, cmd->cmd_count - 1);
			cmd = cmd->next;
		}
		pipeline = pipeline->next;
	}

	//exit(exit_status);
}

char	*expand_var(t_data *data, char *var)
{
	char	*key_var;
	char	*value_var;
	t_list	*temp;

	key_var = ft_strjoin(var, "=");
	// NULL check
	temp = find_var(&data->local_vars, key_var, NULL);
	if (!temp)
		temp = find_var(&data->env, key_var, NULL);
	if (temp)
	{
		value_var = strchr(temp->content, '=') + 1;
		return (value_var);
	}
	return (NULL);
}

t_redir	*find_redir(t_pipe_line *pipeline, t_redir_type find_type)
{
	static t_pipe_line	*current_pipeline = NULL;
	static t_simple_cmd	*current_cmd = NULL;
	static t_redir		*current_redir = NULL;

	if (current_pipeline == NULL)
	{

	}
	current_pipeline = NULL;
	current_cmd = NULL;
	current_redir = NULL;
	return (NULL);
}
