/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:03 by codespace         #+#    #+#             */
/*   Updated: 2025/05/11 14:06:05 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* List of delimetres:
		' - single quote,
			should be handled if one {not interpret unclosed quote}
		" - double quote,
			should be habdled if one {not interpret unclosed quote}
		/ - idnk
		- - attribute specificator for commands
		| - fot PIPES
		$ - for envitonmental variables
		&& and || - for priorities
		* - wildcard
*/

t_redir	*ft_create_redir(t_token **tokens, int index)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->index = index;
	redir->next = NULL;
	if ((*tokens)->type == GREAT)
		redir->type = RE_GREAT;
	else if ((*tokens)->type == DOUBLE_GREAT)
		redir->type = RE_DOUBLE_GREAT;
	else if ((*tokens)->type == LESS)
		redir->type = RE_LESS;
	else if ((*tokens)->type == DOUBLE_LESS)
		redir->type = RE_DOUBLE_LESS;
	(*tokens) = (*tokens)->next;
	redir->file_name = ft_strdup((*tokens)->value);
	if (!redir->file_name)
		return (free(redir), NULL);
	(*tokens) = (*tokens)->next;
	redir->fd = -1;
	redir->existing = 0;
	redir->inside_quotes = 0;
	redir->delim = NULL;
	return (redir);
}

t_redir	*ft_insert_redir(t_redir *redir, t_token **tokens, int index)
{
	t_redir	*tmp;

	tmp = NULL;
	if (redir == NULL)
	{
		redir = ft_create_redir(tokens, index);
		if (!redir)
			return (NULL);
	}
	else
	{
		tmp = redir;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = ft_create_redir(tokens, index);
		if (!tmp->next)
		{
			free_redir(redir);
			return (NULL);
		}
	}
	return (redir);
}

void	ft_insert_arg(t_args *head, t_args *current_args)
{
	t_args	*tmp;

	tmp = head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = current_args;
}

t_args	*ft_create_arg(char *value)
{
	t_args	*arg;

	arg = (t_args *)malloc(sizeof(t_args));
	if (!arg)
		return (NULL);
	arg->value = ft_strdup(value);
	if (!arg->value)
		return (free(arg), NULL);
	arg->next = NULL;
	arg->inside_quotes = 0;
	return (arg);
}

void	*add_subtoken(t_token **orig_list, char *content, int type)
{
	t_token	*token_list;

	token_list = *orig_list;
	if (!content)
		return (NULL);
	if (!token_list)
	{
		token_list = (t_token *)malloc(sizeof(t_token));
		*orig_list = token_list;
	}
	else
	{
		while (token_list->next != NULL)
			token_list = token_list->next;
		token_list->next = (t_token *)malloc(sizeof(t_token));
		token_list = token_list->next;
	}
	if (!token_list)
		return (free(content), NULL);
	token_list->index = 0;
	token_list->type = type;
	token_list->value = content;
	token_list->next = NULL;
	return (token_list);
}

char	*subvalue(char *str, int *curr_i)
{
	int		start;
	char	*result;
	char	*part;
	char	*tmp;

	result = ft_strdup("");
	while (result && str[*curr_i] && str[*curr_i] != ' ')
	{
		part = NULL;
		if (str[*curr_i] == '\'' || str[*curr_i] == '\"')
		{
			start = *curr_i;
			*curr_i += get_i_end_token(&str[*curr_i], str[*curr_i]);
			part = ft_substr(str, start, *curr_i - start);
		}
		else
		{
			start = *curr_i;
			while (str[*curr_i] && str[*curr_i] != ' ' &&
				str[*curr_i] != '\'' && str[*curr_i] != '\"')
				*curr_i += 1;
			part = ft_substr(str, start, *curr_i - start);

		}
		if (!part)
		{
			free(result);
			return (NULL);
		}
		tmp = result;
		result = ft_strjoin(tmp, part);
		free(tmp);
		free(part);
	}
	return (result);
}

t_token	*split_token(t_data *data, t_token *token, int *empty_flag)
{
	int		curr_i;
	t_token	*new_token_list;
	char	*new_value;
	char	*expnd_str;

	curr_i = 0;
	new_token_list = NULL;
	expnd_str = expand_str(data, token->value, ft_strdup(""), 1);
	if (!expnd_str)
		return (NULL);
	if (!*expnd_str)
		*empty_flag = 1;
	while (expnd_str[curr_i] && expnd_str[curr_i] == ' ')
		curr_i++;
	while (expnd_str[curr_i])
	{
		new_value = subvalue(expnd_str, &curr_i);
		while (expnd_str[curr_i] && expnd_str[curr_i] == ' ')
			curr_i++;
		if (!*new_value)
		{
			free(new_value);
			continue ;
		}
		if (add_subtoken(&new_token_list, new_value, token->type) == NULL)
		{
			ft_destoy_token_list(new_token_list);
			return (NULL);
		}
	}
	return (new_token_list);
}

void	*expand_tokens_list(t_data *data, t_token **tokens)
{
	t_token	*current;
	t_token *temp_prev;
	t_token	*new_tokens;
	int		empty_flag;

	current = (*tokens)->next;
	temp_prev = *tokens;
	while (current->type != NEWLINE)
	{
		empty_flag = 0;
		if (current->type == DOUBLE_LESS)
		{
			temp_prev = current->next;
			current = current->next->next;
			continue ;
		}
		new_tokens = split_token(data, current, &empty_flag);
		if (!new_tokens && empty_flag == 0)
			return (NULL);
		if (empty_flag == 1)
		{
			new_tokens = current->next;
			free(current->value);
			free(current);
			temp_prev->next = new_tokens;
			current = new_tokens;
		}
		else
		{
			temp_prev->next = new_tokens;
			while (new_tokens->next)
				new_tokens = new_tokens->next;
			temp_prev = new_tokens;
			new_tokens->next = current->next;
			free(current->value);
			free(current);
			current = new_tokens->next;
		}
	}
	return (tokens);
}

t_cmd_list	*ft_parser(t_data *data, t_token *tokens_list, int *status)
{
	t_cmd_list	*command_list;

	command_list = NULL;
	if (!ft_syntax_check(tokens_list, status))
	{
		//show_token(tokens_list); // print
		if (expand_tokens_list(data, &tokens_list) != NULL)
			command_list = ft_create_asteriks(tokens_list);
		//show_token(tokens_list); // print
		ft_destoy_token_list(tokens_list);
	}
	return (command_list);
}
