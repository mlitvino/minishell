/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:40:39 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/02 00:33:27 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_bachslashes(char *word, int *index, int back_slash)
{
	while (word[*index] == 92)
	{
		back_slash++;
		(*index)++;
	}
	return (back_slash);
}

void	ft_check_quotes(int *in_double, int *in_single,
			char *word, int *i)
{
	if (word[*i] == '\\' && !(*in_single) && word[(*i) + 1] != '\0')
		(*i)++;
	else if (word[(*i)] == '\"' && !(*in_single))
	{
		if (!(*in_double))
			(*in_double) = 1;
		else
			(*in_double) = 0;
	}
	else if (word[(*i)] == '\'' && !(*in_double))
	{
		if (!(*in_single))
			(*in_single) = 1;
		else
			(*in_single) = 0;
	}
}
