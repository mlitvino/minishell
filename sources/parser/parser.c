/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:03 by codespace         #+#    #+#             */
/*   Updated: 2025/05/08 17:46:23 by mlitvino         ###   ########.fr       */
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

// void	*get_subtoken()
// {

// }

// void	*expand_tokens_list(t_data *data, t_token **tokens)
// {
// 	char	*expnd_str;
// 	t_token	*temp_next;
// 	t_token	*temp;
// 	t_token	*new_token;
// 	int		i;

// 	while (*tokens)
// 	{
// 		temp_next = (*tokens)->next;
// 		expnd_str = expand_str(data, (*tokens)->value, ft_strdup(""), 1);
// 		if (!expnd_str)
// 			return (NULL);
// 		i = 0;
// 		while(expnd_str[i])
// 		{
// 			new_token = get_subtoken();
// 			if (*tokens == temp)
// 			{
// 				*tokens = temp
// 			}
// 		}
// 		tokens = temp_next;
// 	}
// 	return (tokens);
// }

static char	*strjoin_and_free(char *s1, char *s2)
{
	char *joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

static int	get_quote_block_len(const char *str)
{
	int		i = 1;
	char	quote = str[0];

	while (str[i] && str[i] != quote)
		i++;
	if (str[i] == quote)
		i++;
	return (i);
}

static int	get_word_len(const char *str)
{
	int i = 0;

	while (str[i] && !isspace((unsigned char)str[i])
		&& str[i] != '\'' && str[i] != '\"')
		i++;
	return (i);
}

static char	*collect_token(const char **str)
{
	char	*value = ft_strdup("");
	char	*part;
	int		len;

	while (**str && !isspace((unsigned char)**str))
	{
		if (**str == '\'' || **str == '\"')
			len = get_quote_block_len(*str);
		else
			len = get_word_len(*str);
		part = ft_substr(*str, 0, len);
		value = strjoin_and_free(value, part);
		*str += len;
	}
	part = ft_strtrim(value, " \t\n");
	free(value);
	return (part);
}

t_token	*get_subtoken_list(const char *str)
{
	t_token	*head = NULL;
	t_token	*last = NULL;
	t_token	*new_tok;
	char	*value;

	while (*str)
	{
		while (*str && isspace((unsigned char)*str))
			str++;
		if (!*str)
			break;
		value = collect_token(&str);
		new_tok = malloc(sizeof(t_token));
		if (!new_tok)
			return (free(value), NULL);
		new_tok->value = value;
		new_tok->next = NULL;
		if (!head)
			head = new_tok;
		else
			last->next = new_tok;
		last = new_tok;
	}
	return (head);
}
void	replace_token_with_subtokens(t_token **current, char *expanded_str)
{
	t_token	*new_list = get_subtoken_list(expanded_str);
	t_token	*last;

	if (!new_list)
		return ;

	last = new_list;
	while (last->next)
		last = last->next;

	t_token *old = *current;
	last->next = old->next;
	*current = new_list;
	free(old->value);
	free(old);
}

void	*expand_tokens_list(t_data *data, t_token **tokens)
{
	t_token	**current = tokens;
	char	*expanded;

	while (*current)
	{
		printf("BEFORE (%s)\n", (*current)->value); //dell
		expanded = expand_str(data, (*current)->value, ft_strdup(""), 1);
		printf("AFTER (%s)\n", expanded); //dell
		if (!expanded)
			return (NULL);
		replace_token_with_subtokens(current, expanded);
		free(expanded);
		while (*current && (*current)->next)
			current = &(*current)->next;
		current = &(*current)->next;
	}
	return (tokens);
}

t_cmd_list	*ft_parser(t_data *data, t_token *tokens_list, int *status)
{
	t_cmd_list	*command_list;

	command_list = NULL;
	if (!ft_syntax_check(tokens_list, status))
	{
		expand_tokens_list(data, &tokens_list);
		printf("NEEEEEEEEEEEEEEEEEEEEEW\n"); // dell
		show_token(tokens_list); // print

		command_list = ft_create_asteriks(tokens_list);
		ft_destoy_token_list(tokens_list);
	}
	return (command_list);
}
