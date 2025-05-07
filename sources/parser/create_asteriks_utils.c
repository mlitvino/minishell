/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_asteriks_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:31:16 by mlitvino          #+#    #+#             */
/*   Updated: 2025/05/07 16:31:57 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe_line	*ft_init_pipe_line(void)
{
	t_pipe_line	*pipe_line;

	pipe_line = (t_pipe_line *)malloc(sizeof(t_pipe_line));
	if (!pipe_line)
		return (NULL);
	pipe_line->next = NULL;
	pipe_line->child = NULL;
	pipe_line->simple_cmd_count = 0;
	pipe_line->exit_status = SUCCESS;
	pipe_line->pid_last_cmd = 0;
	return (pipe_line);
}

t_pipe_line	*reset_pipe_line_head(t_pipe_line **pipe_line, t_simple_cmd *head)
{
	(*pipe_line)->child = head;
	return (*pipe_line);
}
