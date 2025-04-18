#include "minishell.h"

void	show_token(t_token *token)
{
	printf("------------TOKENS-----------------\n");
	for(int i = 0; token; i++)
	{
		printf("i: %d\n", i);
		printf("index: %d\n", token->index);
		printf("type: %d\n", token->type);
		printf("value: (%s)\n", token->value);
		if (token->next)
			printf("------------NEXT_TOKEN--------------\n");
		token = token->next;
	}
	printf("------------END_TOKENS-----------------\n\n\n");
}

void	show_arg(t_args *args)
{
	printf("	------------ARGS-----------------\n");
	for (int i = 0; args; i++)
	{
		printf("	i: %d\n", i);
		printf("	inside_quotes: %d\n", args->inside_quotes);
		printf("	value: (%s)\n", args->value);
		if (args->next)
			printf("	----------NEXT_ARG------------\n");
		args = args->next;
	}
	printf("	-----------END_ARGS------------\n");
}

void	show_redir(t_redir *redir)
{
	printf("	------------REDIRS-----------------\n");
	for (int i = 0; redir; i++)
	{
		printf("	i: %d\n", i);
		printf("	index: %d\n", redir->index);
		printf("	type: %d\n", redir->type);
		printf("	inside_quotes: %d\n", redir->inside_quotes);
		printf("	file_name: (%s)\n", redir->file_name);
		if (redir->next)
			printf("	----------NEXT_REDIR------------\n");
		redir = redir->next;
	}
	printf("	-----------END_REDIRS------------\n");
}

void	show_cmd(t_simple_cmd *cmd)
{
	printf("------------CMDS-----------------\n");
	for (int i = 0; cmd; i++)
	{
		printf("i: %d\n", i);
		printf("inside_quotes: %d\n", cmd->inside_quotes);
		printf("command: (%s)\n", cmd->command);
		show_arg(cmd->args);
		show_redir(cmd->redirections);
		if (cmd->next)
			printf("----------NEXT_CMD------------\n");
		cmd = cmd->next;
	}
	printf("-----------END_CMDS------------\n");
}

void	show_cmd_list(t_cmd_list *list)
{
	printf("pipe_line_count: %d\n", list->pipe_line_count);
	printf("-------------PIPE_LINES-------------\n");

	t_pipe_line *childs = list->childs;
	for(int i = 0; childs; i++)
	{
		printf("i: %d\n", i);
		printf("simple_cmd_count: %d\n", childs->simple_cmd_count);

		show_cmd(childs->child);

		if (childs->next)
			printf("-----------NEXT_PIPE_LINES-------------\n");
		childs = childs->next;
	}
	printf("-------------END_PIPE_LINES-------------\n");
}
