/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 01:51:02 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/15 16:46:41 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_the_beginning_of_word(int c)
{
	if (c == '\\')
		return (1);
	else if (c == '\'')
		return (2);
	else if (c == '\"')
		return (3);
	else
		return (0);
}

static void	add_token_and_increament_index(t_token *tokens_list, char *word,
			int *table, int j)
{
	add_token(tokens_list, WORD, word, table[3]);
	table[1] = j;
	table[3]++;
}

void	ft_get_word(t_token *tokens_list, char *line, int *table)
{
	int		quoting;
	char	*word;
	int		j;

	quoting = -1;
	word = NULL;
	j = table[1];
	while (line[j])
	{
		quoting = check_the_beginning_of_word(line[j]);
		if (quoting == 0)
		{
			word = ft_get_words(line, &j, word, &quoting);
			if (ft_strchr("\t ><|;", line[j]))
				break ;
		}
		else if (quoting > 0)
		{
			word = ft_get_words(line, &j, word, &quoting);
			if (line[j] == ' ' || line[j] == '\t')
				break ;
		}
	}
	add_token_and_increament_index(tokens_list, word, table, j);
}
