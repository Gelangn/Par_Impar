# Colours
DEFAULT 	= \033[0m
GREEN 		= \033[38;5;46m
WHITE 		= \033[38;5;15m
GREY 		= \033[38;5;8m
ORANGE 		= \033[38;5;202m
RED 		= \033[38;5;160m

# Project name (executable)
NAME = ParImpar

# Compiler and flags
CC = gcc

CFLAGS = -Wall -Wextra -Werror -g -pthread
DEBUG = -g3 -pthread
SANITIZE = -fsanitize=address


S = src/

# Source list
SRC = $(wildcard $(S)*.c)
OBJ = $(SRC:.c=.o)

# *** Rules ***

# Default rules
all: $(NAME)
	@echo "$(GREEN)*** Compilation completed OK ***$(DEFAULT)"

# Compilation rule
%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $<

# Regla para construir el archivo estático
$(NAME): $(OBJ)
	@echo "$(ORANGE)*** Creating executable ***$(DEFAULT)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Regla para limpiar archivos objeto
clean:
	@echo "$(RED)[INFO]$(GREY) Cleaning Objects!!!$(DEFAULT)"
	@rm -f $(OBJ)

# Regla para construir el archivo estático con debug
debug: $(OBJ)
	@echo "$(ORANGE)*** Creating executable DEBUG ***$(DEFAULT)"
	@$(CC) $(CFLAGS) $(DEBUG) -o $(NAME)  $(OBJ)
	@echo "$(GREEN)*** Compilation completed OK ***$(DEFAULT)"

sanitize: $(OBJ)
	@echo "$(ORANGE)*** Creating executable SANITIZE ***$(DEFAULT)"
	@$(CC) $(CFLAGS) $(DEBUG) $(SANITIZE) -o $(NAME) $(OBJ)
	@echo "$(GREEN)*** Compilation completed OK ***$(DEFAULT)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)[INFO]$(GREY) Objects removed!!!$(DEFAULT)"

# Regla para recompilar
re: fclean all

.PHONY: all clean fclean re debug sanitize