/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:15:56 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/29 17:51:23 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_quote_flg(char *str, int *quote_flg)
{
	if (ft_strchr(str, '\'') != NULL)
		*quote_flg = 1;
	else if (ft_strchr(str, '\"') != NULL)
		*quote_flg = 1;
}

char	*trim_quotes(t_data *data, char *str)
{
	int		curr_i;
	char	*new_str;
	char	*temp;
	int		end_i;

	curr_i = 0;
	end_i = ft_strchr(&str[curr_i + 1], str[curr_i]) - str - 1;
	temp = ft_substr(&str[curr_i + 1], 0, end_i);
	if (*str == '\"')
		new_str = expand_str(data, temp, ft_strdup(""));
	else
		new_str = ft_strdup(temp);
	free(temp);
	return (new_str);
}

char	*get_next_piece(t_data *data, char *orig_str, int *orig_i)
{
	int		curr_i;
	char	*res;

	curr_i = 0;
	while (orig_str[curr_i] && orig_str[curr_i] != '$'
		&& orig_str[curr_i] != '\'' && orig_str[curr_i] != '\"')
		curr_i++;
	if (curr_i != 0)
		res = ft_substr(orig_str, 0, curr_i);
	else if (orig_str[curr_i] == '$')
	{
		res = expand_var(data, &orig_str[curr_i]);
		curr_i += get_i_end_token(&orig_str[curr_i], '$');
	}
	else if (orig_str[curr_i] == '\'' || orig_str[curr_i] == '\"')
	{
		res = trim_quotes(data, &orig_str[curr_i]);
		curr_i += get_i_end_token(&orig_str[curr_i], orig_str[curr_i]);
	}
	*orig_i += curr_i;
	return (res);
}

char	*trim_expand(t_data *data, char *orig_str)
{
	int		orig_i;
	char	*new_str;
	char	*piece_str;

	orig_i = 0;
	new_str = NULL;
	if (*orig_str == '\0')
		return (orig_str);
	while (orig_str && orig_str[orig_i])
	{
		piece_str = get_next_piece(data, &orig_str[orig_i], &orig_i);
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

int	check_quots_expand(t_data *data, t_simple_cmd *cmd)
{
	t_args	*args;
	t_redir	*redir;

	check_quote_flg(cmd->command, &cmd->inside_quotes);
	cmd->command = trim_expand(data, cmd->command);
	args = cmd->args;
	while (args)
	{
		check_quote_flg(args->value, &args->inside_quotes);
		args->value = trim_expand(data, args->value);
		args = args->next;
	}
	redir = cmd->redirections;
	while (redir)
	{
		check_quote_flg(redir->file_name, &redir->inside_quotes);
		redir->file_name = trim_expand(data, redir->file_name);
		redir = redir->next;
	}
	return (SUCCESS);
}
