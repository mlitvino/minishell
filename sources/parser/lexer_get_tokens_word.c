/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_tokens_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 01:50:58 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/07 13:38:44 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count_backslashes(char *line, int j)
{
	int	k;
	int	back_slash_count;

	k = j - 1;
	back_slash_count = 0;
	while (line[k] == '\\')
	{
		back_slash_count++;
		k--;
	}
	return (back_slash_count);
}

static char	*get_double_quotes_word(char *line, int *i, int j)
{
	char	*word;
	int		back_slash_count;

	back_slash_count = 0;
	word = NULL;
	while (line[j])
	{
		if (line[j] == '"' && line[j - 1] != '\\')
			break ;
		else if (line[j] == '"' && line[j - 1] == '\\')
		{
			back_slash_count = ft_count_backslashes(line, j);
			if (back_slash_count % 2 == 0)
			{
				back_slash_count = 0;
				break ;
			}
			else
				back_slash_count = 0;
		}
		j++;
	}
	word = ft_substr(line, *i, j - *i + 1);
	*i = j + 1;
	return (word);
}

static char	*get_quoting_word(char *line, int *i, int quoting)
{
	int		j;
	char	*word;

	j = *i;
	word = NULL;
	if (quoting == 1)
	{
		word = ft_substr(line, *i, 2);
		*i += 2;
		return (word);
	}
	else if (quoting == 2)
	{
		j++;
		while (line[j] && line[j] != '\'')
			j++;
		word = ft_substr(line, *i, j - *i + 1);
		*i = j + 1;
		return (word);
	}
	else if (quoting == 3)
		return (get_double_quotes_word(line, i, ++j));
	return (word);
}

static void	*check_strjoin(char *tmp1, char **word, char *tmp)
{
	if (!tmp1)
		return (NULL);
	*word = ft_strjoin(*word, tmp1);
	free(tmp1);
	free(tmp);
	if (!*word)
		return (NULL);
	return (word);
}

char	*ft_get_words(char *line, int *j, char *word, int *quoting)
{
	char	*tmp;
	char	*tmp1;

	if (*quoting == 0)
	{
		tmp = word;
		tmp1 = get_no_quoting_word(line, j);
		if (check_strjoin(tmp1, &word, tmp) == NULL)
			return (NULL);
	}
	else if (*quoting > 0)
	{
		tmp = word;
		tmp1 = get_quoting_word(line, j, *quoting);
		if (check_strjoin(tmp1, &word, tmp) == NULL)
			return (NULL);
		*quoting = -1;
	}
	return (word);
}
