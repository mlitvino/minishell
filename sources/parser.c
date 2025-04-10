/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:03 by codespace         #+#    #+#             */
/*   Updated: 2025/04/10 17:47:23 by alfokin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* List of delimetres:
        ' - single quote, should be handled if one
        " - double quote
        /
        -
        &
        |
*/

char    *ft_strtok(char *str)
{
    
}

void	parser(char *input, t_cmd_list *cmd_list)
{
    char	**tokens;
    int		i;

    tokens = ft_split(input, ' ');
    if (!tokens)
        return ;
    i = 0;
    while (tokens[i])
    {
        if (ft_strncmp(tokens[i], "cd", 2) == 0)
            cmd_list->key = CD;
        else if (ft_strncmp(tokens[i], "pwd", 3) == 0)
            cmd_list->key = PWD;
        else if (ft_strncmp(tokens[i], "ls", 2) == 0)
            cmd_list->key = LS;
        else
            cmd_list->key = OTHER;
        cmd_list->value = tokens[i];
        i++;
    }
    free(tokens);
}