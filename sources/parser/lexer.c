/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 01:50:24 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/07 13:17:15 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*first_token(void)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup("NONE");
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->next = NULL;
	new_token->type = NONE;
	new_token->index = 0;
	return (new_token);
}

void	*add_token(t_token *token_list, t_token_type type, char *content,
		int index)
{
	t_token	*tmp;

	if (!content)
		return (NULL);
	tmp = token_list;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = (t_token *)malloc(sizeof(t_token));
	if (!tmp->next)
	{
		free(content);
		return (NULL);
	}
	tmp->next->index = index;
	tmp->next->type = type;
	tmp->next->value = content;
	tmp->next->next = NULL;
	return (token_list);
}

static void	*get_token(t_token *tokens_list, int *table, char *line)
{
	if (ft_strrchr("|;><", line[table[1]]) != NULL)
	{
		if (get_space_pipe_semi_redir(tokens_list, line, &table[1],
				&table[3]) == NULL)
			return (NULL);
	}
	if (ft_strrchr("\t <>;|", line[table[1]]) == NULL || line[table[1]] == '\\')
	{
		table[2] = table[1];
		if (ft_get_word(tokens_list, line, table) == NULL)
			return (NULL);
	}
	return (tokens_list);
}

/*
 * table[5];
 * 0 = i  ; 1 = j ; k = 2; index = 3 ; quote = 4;
 * */
void	*create_tokens_list(t_token *tokens_list, char *line)
{
	int	table[5];

	table[3] = 1;
	table[4] = 0;
	table[0] = 0;
	while (line[table[0]])
	{
		table[1] = table[0];
		while (line[table[1]] == ' ' || line[table[1]] == '\t')
			table[1]++;
		if (get_token(tokens_list, table, line) == NULL)
			return (NULL);
		table[0] = table[1];
	}
	if (add_token(tokens_list, NEWLINE, ft_strdup("newline"), table[3]) == NULL)
		return (NULL);
	return (tokens_list);
}

t_token	*ft_lexer(char *line)
{
	t_token	*tokens_list;

	tokens_list = first_token();
	if (!tokens_list)
		return (NULL);
	if (create_tokens_list(tokens_list, line) == NULL)
	{
		ft_destoy_token_list(tokens_list);
		return (NULL);
	}
	return (tokens_list);
}
