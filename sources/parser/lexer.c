/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 01:50:24 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/15 15:58:37 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_destoy_token_list(t_token *tokens_list)
{
	t_token	*tmp;

	tmp = NULL;
	while (tokens_list->type != NEWLINE)
	{
		tmp = tokens_list;
		tokens_list = tokens_list->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
	if (tokens_list->type == NEWLINE)
	{
		free(tokens_list->value);
		tokens_list->value = NULL;
		free(tokens_list);
		tokens_list = NULL;
		//write(1, CYAN, ft_strlen(CYAN));
	}
}

t_token	*first_token(void)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	new_token->value = ft_strdup("NONE");
	new_token->next = NULL;
	new_token->type = NONE;
	new_token->index = 0;
	return (new_token);
}

void	add_token(t_token *token_list, t_token_type type,
			   char *content, int index)
{
	t_token	*tmp;

	tmp = token_list;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = (t_token *)malloc(sizeof(t_token));
	tmp->next->index = index;
	tmp->next->type = type;
	tmp->next->value = content;
	tmp->next->next = NULL;
}

/*
 * table[5];
 * 0 = i  ; 1 = j ; k = 2; index = 3 ; quote = 4;
 * */
void	create_tokens_list(t_token *tokens_list, char *line)
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
		if (ft_strrchr("|;><", line[table[1]]) != NULL)
			get_space_pipe_semi_redir(tokens_list, line, &table[1], &table[3]);
		if (ft_strrchr("\t <>;|", line[table[1]]) == NULL
			|| line[table[1]] == '\\')
		{
			table[2] = table[1];
			ft_get_word(tokens_list, line, table);
		}
		table[0] = table[1];
	}
	add_token(tokens_list, NEWLINE, ft_strdup("newline"), table[3]);
}

t_token	*ft_lexer(char *line)
{
	t_token	*tokens_list;

	tokens_list = NULL;
	tokens_list = first_token();
	create_tokens_list(tokens_list, line);
	return (tokens_list);
}
