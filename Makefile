# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 15:22:29 by mlitvino          #+#    #+#              #
#    Updated: 2025/04/27 23:50:03 by alfokin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc -ggdb
CFLAGS = -I$(LIBFT_DIR)/$(INCLD_DIR) -I$(INCLD_DIR)
# -Wall -Wextra -Werror

NAME = minishell
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR		= ./sources
OBJ_DIR		= ./objects
LIBFT_DIR	= ./libft
INCLD_DIR	= ./includes

INCLD = $(INCLD_DIR)/minishell.h
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/readline.c $(SRC_DIR)/executable_handler.c \
      $(SRC_DIR)/cmd_cd.c $(SRC_DIR)/cmd_echo.c $(SRC_DIR)/cmd_env.c $(SRC_DIR)/cmd_exit.c \
      $(SRC_DIR)/cmd_export.c $(SRC_DIR)/cmd_pwd.c $(SRC_DIR)/cmd_unset.c \
      $(SRC_DIR)/parser/cont_token_checker.c \
      $(SRC_DIR)/parser/parser.c $(SRC_DIR)/parser/create_asteriks.c $(SRC_DIR)/parser/create_nodes.c \
      $(SRC_DIR)/parser/destroy_nodes.c $(SRC_DIR)/parser/syntax_checker.c $(SRC_DIR)/parser/token_checker.c \
      $(SRC_DIR)/parser/word_checker.c $(SRC_DIR)/parser/lexer.c $(SRC_DIR)/parser/lexer_get_tokens.c \
      $(SRC_DIR)/parser/lexer_get_tokens_op.c $(SRC_DIR)/parser/lexer_get_tokens_word.c \
	  $(SRC_DIR)/utils.c $(SRC_DIR)/signals.c $(SRC_DIR)/heredoc.c \
	  $(SRC_DIR)/test.c

OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

.SECONDARY: $(OBJ)

all: $(LIBFT) $(NAME) delete_obj_dir

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Automatically generate rules for each source file
define make_obj_rule
$(OBJ_DIR)/$(notdir $(1:.c=.o)): $(1) $(INCLD)
	$(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach file, $(SRC), $(eval $(call make_obj_rule,$(file))))

$(NAME): $(OBJ) $(LIBFT) | $(OBJ_DIR)
	$(CC) $(OBJ) $(LIBFT) -lreadline -lncurses -o $@

$(LIBFT):
	make -C $(LIBFT_DIR) all
	make -C $(LIBFT_DIR) bonus

clean:
	rm -f $(OBJ)
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

delete_obj_dir:
	@if [ -z "$$(ls -A $(OBJ_DIR))" ]; then \
		@rm -rf $(OBJ_DIR); \
	fi

re: fclean all

.PHONY: all clean fclean re delete_obj_dir
