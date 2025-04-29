NAME	= minishell

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g -I include

LIBFT	= ./libft/libft.a

# Source Files
SRC		= \
	./src/main.c \
	./src/environment/environment_utils.c \
	./src/environment/variable_expansion.c \
	./src/input/input_handling.c \
	./src/input/syntax_validation.c \
	./src/input/syntax_validation_utils.c \
	./src/input/delimiter_utils.c \
	./src/input/quotes_handler.c \
	./src/input/parenthesis_validation.c \
	./src/lexer/lexer_main.c \
	./src/lexer/lexer_process.c \
	./src/lexer/lexer_token.c \
	./src/lexer/lexer_assign_types.c \
	./src/lexer/lexer_state_handler.c \
	./src/lexer/lexer_state_utils.c \
	./src/lexer/lexer_utils.c \
	./src/parser/parser.c \
	./src/parser/parser_utils.c \
	./src/parser/heredoc_init.c \
	./src/parser/heredoc_loop.c \
	./src/parser/heredoc_cleanup.c \
	./src/parser/heredoc_handler.c \
	./src/parser/heredoc_creation.c \
	./src/interpreter/interpreter.c \
	./src/pipe/pipe.c \
	./src/builtin/builtin_handler.c \
	./src/builtin/builtin_pwd_env.c \
	./src/builtin/builtin_echo.c \
	./src/builtin/builtin_exit.c \
	./src/builtin/builtin_cd.c \
	./src/builtin/builtin_export.c \
	./src/builtin/builtin_unset.c \
	./src/execute/command_init.c \
	./src/execute/command_heredoc.c \
	./src/execute/process_command_tokens.c \
	./src/execute/command_files.c \
	./src/execute/execute_external.c \
	./src/execute/process_execution.c \
	./src/execute/process_redirections.c \
	./src/execute/path_builder.c \
	./src/signals/prompt_signals.c \
	./src/signals/pipe_signals.c \
	./src/signals/heredoc_signals.c \
	./src/signals/child_signals.c


OBJ		= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	echo "$(NAME) generated"

$(LIBFT):
	make -C ./libft --no-print-directory
	make bonus -C ./libft --no-print-directory
	echo "libft compiled"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean -C ./libft --no-print-directory
	rm -f $(OBJ)
	echo "object files deleted"

fclean: clean
	make fclean -C ./libft --no-print-directory
	rm -f $(NAME)
	echo "$(NAME) deleted"

re: fclean all

.PHONY: all clean fclean re

.SILENT:
