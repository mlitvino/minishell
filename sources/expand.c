/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:14:10 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/17 13:02:17 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_i_end_token(char *str, char token)
{
	int		i;
	char	*temp;

	i = 0;
	if (token == '\'' || token == '\"')
	{
		temp = ft_strchr(str + 1, token);
		if (temp)
			i = temp - str + 1;
		else
			i = -1;
	}
	else if (token == '$')
	{
		if (str[i] == '$')
			i++;
		if (str[i] == '?' || str[i] == '$')
			return (++i);
		if (ft_isdigit(str[i]))
			return (++i);
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
			i++;
	}
	return (i);
}

char	*substr_value(t_data *data, char *var, int i)
{
	char	*env_var;
	char	*value;

	env_var = ft_substr(var, 1, i - 1);
	if (!env_var)
		return (NULL);
	value = find_var_value(data, env_var);
	if (!value)
		value = ft_strdup("");
	else
		value = ft_strdup(value);
	free(env_var);
	return (value);
}

char	*expand_var(t_data *data, char *var)
{
	int		i;
	char	*value;

	i = 1;
	value = NULL;
	if (ft_strncmp(var, "$?", 2) == 0)
		return (ft_itoa(data->exit_var));
	if (ft_isdigit(var[i]) == 1 || var[i] == '\'' || var[i] == '\"')
		return (ft_strdup(""));
	while (var[i] && (ft_isalnum(var[i]) == 1 || var[i] == '_'))
		i++;
	if (i == 1 && var[i] != '$')
		return (ft_strdup("$"));
	if (ft_strncmp("$$", var, 2) == 0)
		if (find_var_value(data, "SYSTEMD_EXEC_PID") != NULL)
			return (ft_strdup(find_var_value(data, "SYSTEMD_EXEC_PID")));
	value = substr_value(data, var, i);
	return (value);
}

static char	*get_expnd_piece(t_data *data, char *orig_str, int *i,
		int skip_quot)
{
	char	*piece_str;

	piece_str = NULL;
	if (*i != 0)
		piece_str = ft_substr(orig_str, 0, *i);
	else if (orig_str[*i] == '$')
	{
		piece_str = expand_var(data, &orig_str[*i]);
		*i += get_i_end_token(&orig_str[*i], orig_str[*i]);
	}
	else if (orig_str[*i] == '\'' || orig_str[*i] == '\"')
	{
		if (skip_quot == 1)
		{
			piece_str = ft_substr(&orig_str[*i], 0, ft_strchr(&orig_str[*i + 1],
						orig_str[*i]) - &orig_str[*i] + 1);
			*i += get_i_end_token(&orig_str[*i], orig_str[*i]);
		}
		else
		{
			piece_str = ft_substr(&orig_str[*i], 0, 1);
			*i += 1;
		}
	}
	return (piece_str);
}

char	*expand_str(t_data *data, char *orig_str, char *new_str, int skip_quot)
{
	int		i;
	char	*temp;
	char	*piece_str;

	i = 0;
	if (!orig_str || !new_str)
		return (free(new_str), NULL);
	while (orig_str[i] && (orig_str[i] != '$' && (orig_str[i] != '\''
				&& orig_str[i] != '\"')))
		i++;
	if (!orig_str[i] && i == 0)
		return (new_str);
	piece_str = get_expnd_piece(data, orig_str, &i, skip_quot);
	temp = ft_strjoin(new_str, piece_str);
	free(new_str);
	if (!temp || !piece_str)
		return (free(temp), free(piece_str), NULL);
	free(piece_str);
	return (expand_str(data, &orig_str[i], temp, skip_quot));
}
