/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:03 by codespace         #+#    #+#             */
/*   Updated: 2025/04/24 18:08:31 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* List of delimetres:
		' - single quote, should be handled if one {not interpret unclosed quote}
		" - double quote, should be habdled if one {not interpret unclosed quote}
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
	(*tokens) = (*tokens)->next;
	redir->file_name = ft_strdup((*tokens)->value);
	(*tokens) = (*tokens)->next;

	// added by mlitvino
	redir->fd = -1;
	redir->delim = NULL;
	redir->inside_quotes = 0;
	return (redir);
}

t_redir	*ft_insert_redir(t_redir *redir, t_token **tokens, int index)
{
	t_redir	*tmp;

	tmp = NULL;
	if (redir == NULL)
		redir = ft_create_redir(tokens, index);
	else
	{
		tmp = redir;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = ft_create_redir(tokens, index);
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
	arg->value = ft_strdup(value);
	arg->next = NULL;
	arg->inside_quotes = 0;
	return (arg);
}

t_cmd_list	*ft_parser(t_token *tokens_list, int *status)
{
	t_cmd_list	*command_list;

	command_list = NULL;
	//write(1, RED, ft_strlen(RED));
	if (!ft_syntax_check(tokens_list, status))
	{
		command_list = ft_create_asteriks(tokens_list);
		ft_destoy_token_list(tokens_list);
	}
	return (command_list);
}
