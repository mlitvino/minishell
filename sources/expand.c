/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:14:10 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/08 12:47:05 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_var_value(t_data *data, char *env_var)
{
	t_list	*temp;

	temp = find_var(data->env, env_var, NULL);
	if (!temp || ft_strchr(temp->content, '=') == NULL)
		return (NULL);
	else
		return (ft_strchr(temp->content, '=') + 1);
}

int	get_i_end_token(char *str, char token)
{
	int	i;

	i = 0;
	if (token == '\'' || token == '\"')
		i = ft_strchr(str + 1, token) - str + 1;
	else if (token == '$')
	{
		if (str[i] == '$')
			i++;
		if (ft_isdigit(str[i]))
			return (++i);
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
			i++;
		if (str[i] == '?')
			i++;
	}
	return (i);
}

char	*trim_spaces(char *value)
{
	char	*new_val;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (value[i++])
		if (value[i - 1] != ' ' || (value[i - 1] == ' ' && value[i] != ' '))
			len++;
	new_val = malloc(sizeof(char) * (len + 1));
	if (!new_val)
		return (NULL);
	new_val[len] = '\0';
	i = 0;
	len = 0;
	while (value[i])
	{
		if (value[i] != ' ' || (value[i] == ' ' && value[i + 1] != ' '))
			new_val[len++] = value[i];
		i++;
	}
	return (new_val);
}

char	*substr_value(t_data *data, char *var, int trim_flg, int i)
{
	char	*env_var;
	char	*value;

	(void)trim_flg;
	env_var = ft_substr(var, 1, i - 1);
	if (!env_var)
		return (NULL);
	value = find_var_value(data, env_var);
	if (!value)
		value = ft_strdup("");
	else
		value = ft_strdup(value);
	free(env_var);
	value = ft_strtrim(value, " ");
	if (!value)
		return (NULL);
	value = trim_spaces(value);
	return (value);
}

char	*expand_var(t_data *data, char *var, int trim_flg)
{
	int		i;
	char	*value;

	i = 1;
	if (ft_strncmp(var, "$?", 2) == 0)
		return (ft_itoa(data->exit_var));
	if (ft_isdigit(var[i]) == 1 || var[i] == '\'' || var[i] == '\"')
		return (ft_strdup(""));
	while (var[i] && (ft_isalnum(var[i]) == 1 || var[i] == '_'))
		i++;
	if (i == 1)
		return (ft_strdup("$"));
	value = substr_value(data, var, trim_flg, i);
	return (value);
}

char	*expand_str(t_data *data, char *orig_str, char *new_str, int trim_flg)
{
	int		i;
	char	*piece_str;
	char	*temp;

	i = 0;
	if (!orig_str || !new_str)
		return (free(new_str), NULL);
	while (orig_str[i] && orig_str[i] != '$')
		i++;
	if (!orig_str[i] && i == 0)
		return (new_str);
	if (i != 0)
		piece_str = ft_substr(orig_str, 0, i);
	else if (orig_str[i] == '$')
	{
		piece_str = expand_var(data, &orig_str[i], trim_flg);
		i += get_i_end_token(&orig_str[i], '$');
	}
	temp = ft_strjoin(new_str, piece_str); // change logic
	free(new_str);
	if (!temp || !piece_str)
		return (free(temp), free(piece_str), NULL);
	free(piece_str);
	return (expand_str(data, &orig_str[i], temp, trim_flg));
}
