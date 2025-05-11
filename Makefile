# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 15:22:29 by mlitvino          #+#    #+#              #
#    Updated: 2025/05/11 17:06:05 by mlitvino         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC 			= cc -Wall -Wextra -Werror
CFLAGS 		= -I$(LIBFT_DIR)/$(INCLD_DIR) -I$(INCLD_DIR)
# -Wall -Wextra -Werror

NAME 		= minishell
LIBFT 		= $(LIBFT_DIR)/libft.a

SRC_DIR		= sources
OBJ_DIR		= objects
LIBFT_DIR	= libft
INCLD_DIR	= includes

PARSER_DIR	= parser
BUILT_DIR	= builtins
EXEC_DIR	= executor

INCLD		= $(INCLD_DIR)/minishell.h

SRC =	$(addprefix $(SRC_DIR)/, \
		main.c utils.c signals.c check_cmd.c expand.c quotes.c \
		heredoc.c heredoc_utils.c heredoc_trim.c utils_clean.c inits.c \
		$(PARSER_SRC) $(BUILT_SRC) $(EXEC_SRC) \
		test.c)

PARSER_SRC = $(addprefix $(PARSER_DIR)/, \
		cont_token_checker.c parser.c parser_split_tok.c parser_split_tok_utils.c \
		create_asteriks.c create_asteriks_utils.c \
		create_nodes.c create_nodes_utils.c \
		destroy_nodes.c syntax_checker.c token_checker.c word_checker.c \
		lexer.c lexer_get_tokens.c lexer_get_tokens_op.c lexer_utils.c\
		lexer_get_tokens_word.c)
BUILT_SRC = $(addprefix $(BUILT_DIR)/, \
		cmd_cd_utils.c cmd_cd.c cmd_echo.c cmd_env.c cmd_exit.c cmd_export.c cmd_pwd.c \
		cmd_unset.c)
EXEC_SRC = $(addprefix $(EXEC_DIR)/, \
		executor.c executor_utils.c search_exec.c redirect.c \
		check_cmd.c search_utils.c)

OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.c=.o)))

.SECONDARY: $(OBJ)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LIBFT) -lreadline -lncurses -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLD)
	@[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/*/%.c $(INCLD)
	@[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR) all
	make -C $(LIBFT_DIR) bonus

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
