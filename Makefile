# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alfokin <alfokin@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 15:22:29 by mlitvino          #+#    #+#              #
#    Updated: 2025/04/14 00:20:23 by alfokin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc -ggdb
CFLAGS = -I$(LIBFT_DIR)/$(INCLD_DIR) -I$(INCLD_DIR)
# -Wall -Wextra -Werror

NAME = minishell
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR		= ./sources
PARS_DIR 	= parser
OBJ_DIR		= ./objects
LIBFT_DIR	= ./libft
INCLD_DIR	= ./includes

INCLD = $(INCLD_DIR)/minishell.h
PARS_FILES	=	parser.c cont_token_checker.c create_asteriks.c \
				create_nodes.c destroy_nodes.c syntax_checker.c \
				token_checker.c word_checker.c
SRC =	main.c readline.c executable_handler.c \
		cmd_cd.c cmd_echo.c cmd_env.c cmd_exit.c \
		cmd_export.c cmd_pwd.c cmd_unset.c \
		$(PARS_DIR)/$(PARS_FILES)

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

.SECONDARY: $(OBJ)

all: $(LIBFT) $(NAME) delete_obj_dir

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLD) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT) | $(OBJ_DIR)
	$(CC) $(OBJ) $(LIBFT) -lreadline -o $@

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
