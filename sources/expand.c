/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:14:10 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/29 18:08:36 by mlitvino         ###   ########.fr       */
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

char	*expand_var(t_data *data, char *var)
{
	char	*env_var;
	int		i;
	char	*value;

	i = 1;
	if (ft_strncmp(var, "$?", 2) == 0)
		return (ft_itoa(data->exit_var));
	if (ft_isdigit(var[i]) == 1)
		return (ft_strdup(""));
	while (var[i] && (ft_isalnum(var[i]) == 1 || var[i] == '_'))
		i++;
	if (i == 1)
		return (ft_strdup("$"));
	env_var = ft_substr(var, 1, i - 1);
	if (!env_var)
		return (NULL);

	char *temp = ft_strjoin(env_var, "="); // change logic
	free(env_var);
	env_var = temp;

	value = find_var_value(data, env_var);
	if (!value)
		value = ft_strdup("");
	else
		value = ft_strdup(value);
	free(env_var);
	return (value);
}

char	*expand_str(t_data *data, char *orig_str, char *new_str)
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
		piece_str = expand_var(data, &orig_str[i]);
		i += get_i_end_token(&orig_str[i], '$');
	}
	temp = ft_strjoin(new_str, piece_str); // change logic
	if (!temp || !piece_str)
		return (free(new_str), free(piece_str), NULL);
	free(new_str);
	free(piece_str);
	return (expand_str(data, &orig_str[i], temp));
}
