/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_tokens_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 01:50:58 by alfokin           #+#    #+#             */
/*   Updated: 2025/04/27 23:48:18 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_no_quoting_word(char *line, int *i)
{
	char	*word;
	int		j;

	j = *i;
	while (line[j] && !ft_strrchr("\t '\"\\<>;|", line[j]))
		j++;
	word = ft_substr(line, *i, j - *i);
	*i = j;
	return (word);
}

static int	ft_count_backslashes(char *line, int j)
{
	int		k;
	int		back_slash_count;

	k = j - 1;
	back_slash_count = 0;
	while (line[k] == '\\')
	{
		back_slash_count++;
		k--;
	}
	return (back_slash_count);
}

static char	*get_double_quotes_word(char *line, int *i)
{
	int start;
	int j;
	char *raw;
	char *processed;
	int k;
	int p;

	start = *i;
	j = *i + 1;
	while (line[j])
	{
		if (line[j] == '\"' && line[j - 1] != '\\')
			break ;
		else if (line[j] == '\"' && line[j - 1] == '\\')
		{
			if (ft_count_backslashes(line, j) % 2 == 0)
				break ;
		}
		j++;
	}
	if (line[j] == '\0')
	{
		raw = ft_substr(line, start, j - start);
		*i = j;
		return (raw);
	}
	raw = ft_substr(line, start + 1, j - start - 1);
	*i = j + 1;
	processed = (char *)malloc(ft_strlen(raw) + 1);
	if (!processed)
	{
		free(raw);
		return (NULL);
	}
	k = 0;
	p = 0;
	while (raw[k])
	{
		if (raw[k] == '\\' && raw[k + 1] != '\0')
		{
			if (raw[k + 1] == '\"' || raw[k + 1] == '\\' || raw[k + 1] == '$')
			{
				processed[p++] = raw[k + 1];
				k += 2;
				continue ;
			}
			processed[p++] = raw[k++];
			continue ;
		}
		processed[p++] = raw[k++];
	}
	processed[p] = '\0';
	free(raw);
	return (processed);
}

static char	*get_quoting_word(char *line, int *i, int quoting)
{
	int		j;
	char	*word;

	j = *i;
	word = NULL;
	if (quoting == 1)
	{
		if (line[j + 1] == '\0')
		{
			word = ft_substr(line, j, 1);
			*i += 1;
		}
		else
		{
			word = ft_substr(line, j + 1, 1);
			*i += 2;
		}
		return (word);
	}
	else if (quoting == 2)
	{
		j++;
		while (line[j] && line[j] != '\'')
			j++;
		if (line[j] == '\0')
		{
			word = ft_substr(line, *i, j - *i);
			*i = j;
		}
		else
		{
			word = ft_substr(line, *i + 1, j - *i - 1);
			*i = j + 1;
		}
		return (word);
	}
	else if (quoting == 3)
		return (get_double_quotes_word(line, i));
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
		word = ft_strjoin(word, tmp1);
		free(tmp);
		free(tmp1);
	}
	else if (*quoting > 0)
	{
		tmp = word;
		tmp1 = get_quoting_word(line, j, *quoting);
		word = ft_strjoin(word, tmp1);
		free(tmp1);
		free(tmp);
		*quoting = -1;
	}
	return (word);
}
