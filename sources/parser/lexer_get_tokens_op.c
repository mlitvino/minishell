/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_tokens_op.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 01:50:48 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/07 14:00:34 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*add_less_redir(t_token *tokens_list,
		char *line, int *j, int *index)
{
	if (line[*j + 1] == '<')
	{
		if (add_token(tokens_list, DOUBLE_LESS, ft_strdup("<<"),
				*index) == NULL)
			return (NULL);
		*j = *j + 2;
		(*index)++;
	}
	else
	{
		if (add_token(tokens_list, LESS, ft_strdup("<"), *index) == NULL)
			return (NULL);
		(*j)++;
		(*index)++;
	}
	return (tokens_list);
}

static void	*get_redir(t_token *tokens_list, char *line, int *j, int *index)
{
	if (line[*j] == '>')
	{
		if (line[*j + 1] == '>')
		{
			if (add_token(tokens_list, DOUBLE_GREAT, ft_strdup(">>"),
					*index) == NULL)
				return (NULL);
			*j = *j + 2;
			(*index)++;
		}
		else
		{
			if (add_token(tokens_list, GREAT, ft_strdup(">"), *index) == NULL)
				return (NULL);
			(*j)++;
			(*index)++;
		}
	}
	else if (line[*j] == '<')
	{
		if (add_less_redir(tokens_list, line, j, index) == NULL)
			return (NULL);
	}
	return (tokens_list);
}

static void	*get_semi_token(t_token *tokens_list,
		int *index, int *j, char *line)
{
	char	*token;

	if (line[*j + 1] == ';')
		token = ft_strdup(";;");
	else
		token = ft_strdup(";");
	if (!token)
		return (NULL);
	if (add_token(tokens_list, SEMI, token, *index) == NULL)
		return (NULL);
	(*index)++;
	(*j)++;
	return (tokens_list);
}

static void	*get_pipe_token(t_token *tokens_list,
		int *index, int *j, char *line)
{
	char	*token;

	if (line[*j + 1] == '|')
		token = ft_strdup("||");
	else
		token = ft_strdup("|");
	if (!token)
		return (NULL);
	if (add_token(tokens_list, PIPE, token, *index) == NULL)
		return (NULL);
	(*index)++;
	(*j)++;
	return (tokens_list);
}

void	*get_space_pipe_semi_redir(t_token *tokens_list, char *line, int *j,
		int *index)
{
	if (line[*j] == '|')
	{
		if (get_pipe_token(tokens_list, index, j, line) == NULL)
			return (NULL);
	}
	else if (line[*j] == ';')
	{
		if (get_semi_token(tokens_list, index, j, line) == NULL)
			return (NULL);
	}
	else if (get_redir(tokens_list, line, j, index) == NULL)
		return (NULL);
	return (tokens_list);
}
