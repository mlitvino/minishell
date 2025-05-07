/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_asteriks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:45:15 by alfokin           #+#    #+#             */
/*   Updated: 2025/05/07 16:52:44 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*loop_create_pipeline(t_token **tokens, t_simple_cmd **head,
		t_pipe_line **pipe_line)
{
	t_simple_cmd	*current_cmd;

	current_cmd = NULL;
	while ((*tokens)->type != NEWLINE)
	{
		if (*head == NULL)
		{
			*head = ft_create_simple_cmd(tokens);
			if (!*head)
				return (free_cmds(*head), NULL);
			(*pipe_line)->simple_cmd_count += 1;
		}
		else
		{
			if ((*tokens)->type == SEMI || (*tokens)->type == NEWLINE)
				break ;
			(*tokens) = (*tokens)->next;
			current_cmd = ft_create_simple_cmd(tokens);
			ft_insert_simple_cmd(*head, current_cmd);
			if (!current_cmd)
				return (free_cmds(*head), NULL);
			(*pipe_line)->simple_cmd_count += 1;
		}
	}
	return (tokens);
}

t_pipe_line	*ft_create_pieline(t_token **tokens)
{
	t_pipe_line		*pipe_line;
	t_simple_cmd	*head;

	pipe_line = ft_init_pipe_line();
	if (!pipe_line)
		return (NULL);
	head = NULL;
	if (loop_create_pipeline(tokens, &head, &pipe_line) == NULL)
		return (free(pipe_line), NULL);
	return (reset_pipe_line_head(&pipe_line, head));
}

void	*ft_create_next_pipeline(t_token *tokens_list, t_cmd_list **head)
{
	t_pipe_line	*current_pipeline;
	void		*prev_ptr;

	current_pipeline = (*head)->childs;
	while (current_pipeline->next != NULL)
		current_pipeline = current_pipeline->next;
	if (tokens_list->type != NEWLINE && tokens_list->type != SEMI)
	{
		current_pipeline->next = ft_create_pieline(&tokens_list);
		if (!current_pipeline->next)
		{
			current_pipeline = (*head)->childs;
			while (current_pipeline)
			{
				free_cmds(current_pipeline->child);
				prev_ptr = current_pipeline->next;
				free(current_pipeline);
				current_pipeline = prev_ptr;
			}
			return (NULL);
		}
		(*head)->pipe_line_count += 1;
	}
	return (tokens_list);
}

t_cmd_list	*ft_create_asteriks(t_token *tokens_list)
{
	t_cmd_list	*head;

	head = init_cmd_list();
	if (!head)
		return (NULL);
	while (tokens_list->type != NEWLINE)
	{
		if (head->childs == NULL)
		{
			head->childs = ft_create_pieline(&tokens_list);
			if (!head->childs)
				return (free(head), NULL);
			head->pipe_line_count += 1;
		}
		else if (tokens_list->type == SEMI)
		{
			tokens_list = tokens_list->next;
			if (ft_create_next_pipeline(tokens_list, &head) == NULL)
				return (free(head), NULL);
		}
		else
			tokens_list = tokens_list->next;
	}
	return (head);
}
