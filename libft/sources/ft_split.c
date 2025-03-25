/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:56:12 by mlitvino          #+#    #+#             */
/*   Updated: 2024/11/08 17:16:45 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_w(char const *s, char c)
{
	int	count;
	int	flag;

	count = 0;
	flag = 1;
	while (*s && *s == c)
		s++;
	while (*s)
	{
		if (*s != c && flag == 1)
		{
			count++;
			flag = 0;
		}
		if (*s == c && flag == 0)
			flag = 1;
		s++;
	}
	return (count);
}

static int	ft_check(char **tab, int j)
{
	int	i;

	if (tab[j] != NULL)
		return (0);
	i = 0;
	while (i < j)
	{
		free(tab[i++]);
	}
	free(tab);
	return (1);
}

static int	ft_skip_len(const char *s, char c, int *i)
{
	int	word_len;

	word_len = 0;
	while (s[*i] == c)
		(*i)++;
	while (s[*i + word_len] && s[*i + word_len] != c)
		word_len++;
	return (word_len);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		i;
	int		j;
	int		word_len;
	int		word_count;

	i = 0;
	j = 0;
	word_count = ft_count_w(s, c);
	tab = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (tab == NULL)
		return (NULL);
	while (s[i] && j < word_count)
	{
		word_len = ft_skip_len(s, c, &i);
		if (word_len > 0)
		{
			tab[j] = ft_substr(s, i, word_len);
			if (ft_check(tab, j++))
				return (NULL);
		}
		i += word_len;
	}
	tab[j] = NULL;
	return (tab);
}
