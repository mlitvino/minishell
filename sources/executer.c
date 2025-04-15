/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:03:21 by mlitvino          #+#    #+#             */
/*   Updated: 2025/04/15 18:42:30 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	pipex(char *read_line, char *env[])
// {
// 	int		pipefd[2];
// 	pid_t	child_pid;
// 	int		outfile;
// 	int		infile;
// 	char	*line;

// 	child_pid = fork();
// 	if (child_pid == 0)
// 	{
// 		outfile = open("te.txt", O_RDWR | O_TRUNC);
// 		infile = open("test.txt", O_RDWR);
// 		pipe(pipefd);
// 		child_pid = fork();
// 		if (child_pid == 0)
// 		{
// 			close(pipefd[0]);
// 			dup2(infile, 0);
// 			dup2(pipefd[1], 1);


// 		close(outfile);
// 		close(infile);
// 		close(pipefd[0]);
// 		close(pipefd[1]);

// 			char *argv[] = {"cat", NULL};
// 			execve("/bin/cat", argv, env);
// 			perror("execve:");
// 			exit(1);
// 		}
// 		else
// 		{
// 			close(pipefd[1]);


// 			dup2(pipefd[0], 0);
// 			dup2(outfile, 1);

// 		close(outfile);
// 		close(infile);
// 		close(pipefd[0]);
// 		close(pipefd[1]);

// 			char *argv2[] = {"cat", NULL};
// 			execve("/bin/cat", argv2, env);
// 			perror("eeexecve");
// 			exit(1);
// 		}


// 		close(outfile);
// 		close(infile);
// 		close(pipefd[0]);
// 		close(pipefd[1]);

// 		cmd_exit();
// 	}
// 	else
// 	{
// 		waitpid(child_pid, 0, 0);
// 	}
// }

void	executer()
{
	
}
