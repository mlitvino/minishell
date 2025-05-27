/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 01:51:02 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/27 21:54:13 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_the_beginning_of_word(int c)
{
	if (c == '\'')
		return (2);
	else if (c == '\"')
		return (3);
	else
		return (0);
}

static void	*add_token_and_increament_index(t_token *tokens_list, char *word,
		int *table, int j)
{
	if (add_token(tokens_list, WORD, word, table[3]) == NULL)
		return (NULL);
	table[1] = j;
	table[3]++;
	return (tokens_list);
}

static int	substr_word(char *line, char **word, int *j)
{
	int	quoting;

	quoting = check_the_beginning_of_word(line[*j]);
	if (quoting == 0)
	{
		*word = ft_get_words(line, j, *word, &quoting);
		if (!*word)
			return (FAILURE);
		if (ft_strchr("\t ><|;", line[*j]))
			return (2);
	}
	else if (quoting > 0)
	{
		*word = ft_get_words(line, j, *word, &quoting);
		if (!*word)
			return (FAILURE);
		if ((line[*j] == ' ' || line[*j] == '\t'))
			return (2);
	}
	return (SUCCESS);
}

void	*ft_get_word(t_token *tokens_list, char *line, int *table)
{
	char	*word;
	int		j;
	int		exit_code;

	word = NULL;
	j = table[1];
	while (line[j])
	{
		exit_code = substr_word(line, &word, &j);
		if (exit_code == FAILURE)
			return (NULL);
		if (exit_code == 2)
			break ;
	}
	if (add_token_and_increament_index(tokens_list, word, table, j) == NULL)
		return (NULL);
	return (tokens_list);
}
