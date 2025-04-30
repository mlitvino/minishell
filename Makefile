# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 15:22:29 by mlitvino          #+#    #+#              #
#    Updated: 2025/04/30 17:46:44 by mlitvino         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC 			= cc -ggdb
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

INCLD		= $(INCLD_DIR)/minishell.h

SRC =	$(addprefix $(SRC_DIR)/, \
		main.c utils.c signals.c expand.c quotes.c heredoc.c heredoc_utils.c utils_clean.c inits.c \
		executor_search.c executor.c executor_redirect.c executor_utils.c \
		$(PARSER_SRC) $(BUILT_SRC) \
		test.c)
PARSER_SRC = $(addprefix $(PARSER_DIR)/, \
		cont_token_checker.c parser.c create_asteriks.c create_nodes.c \
		destroy_nodes.c syntax_checker.c token_checker.c word_checker.c \
		lexer.c lexer_get_tokens.c lexer_get_tokens_op.c \
		lexer_get_tokens_word.c)
BUILT_SRC = $(addprefix $(BUILT_DIR)/, \
		cmd_cd.c cmd_echo.c cmd_env.c cmd_exit.c cmd_export.c cmd_pwd.c \
		cmd_unset.c)

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
