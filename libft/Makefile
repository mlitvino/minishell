# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/31 17:29:24 by mlitvino          #+#    #+#              #
#    Updated: 2025/03/27 22:25:44 by mlitvino         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCLD_DIR)
AR = ar

NAME = libft.a

SRC_DIR = ./sources
OBJ_DIR = ./objects
INCLD_DIR = ./includes

SRC = \
	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memcpy.c ft_memmove.c \
	ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
	ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c \
	ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
	ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
	ft_putendl_fd.c ft_putnbr_fd.c \
	ft_isspace.c \
	ft_printf.c ft_putchar.c ft_puthex.c ft_putint.c ft_putptr.c ft_putu.c \
	ft_putstr.c \
	get_next_line.c get_next_line_utils.c \
	ft_strcmp.c
BSRC = \
	ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
	ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
	ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c
INCLD = $(INCLD_DIR)/libft.h

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)
BOBJ = $(BSRC:%.c=$(OBJ_DIR)/%.o)

.SECONDARY: $(OBJ) $(BOBJ)

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLD) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLD) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(AR) -rcs $@ $^

bonus: $(NAME) .BONUS_DEPEND

.BONUS_DEPEND : $(NAME) $(BOBJ) $(OBJ)
	$(AR) -rcs $(NAME) $(BOBJ) $(OBJ) $?
	rm -f .BONUS_DEPEND
	touch .BONUS_DEPEND

clean:
	rm -f $(OBJ) $(BOBJ)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f .BONUS_DEPEND
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
