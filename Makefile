NAME	= minishell

ROOT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

CC		= cc

# headers
HEADERS_DIRECTORY := $(ROOT_DIR)/include
HEADERS := $(addprefix $(HEADERS_DIRECTORY)/, minishell.h)

# source files
SOURCES_DIRECTORY := $(ROOT_DIR)/src
OBJECTS_DIRECTORY := $(ROOT_DIR)/build
SOURCES		:= $(addprefix $(SOURCES_DIRECTORY)/,\
			main.c \
			$(addprefix environment/, environment_utils.c variable_expansion.c) \
			$(addprefix input/, input_handling.c input_syntax_checker.c \
				operator_syntax.c delimiter_utils.c grouping_syntax.c \
				grouping_validation.c) \
			$(addprefix lexer/, lexer_main.c lexer_process.c lexer_assign_types.c \
				lexer_state_handler.c lexer_state_utils.c lexer_utils.c lexer_token.c) \
			$(addprefix parser/, parser.c parser_utils.c heredoc_init.c \
				heredoc_loop.c heredoc_cleanup.c heredoc_handler.c heredoc_creation.c) \
			$(addprefix interpreter/, interpreter.c) \
			$(addprefix pipe/, pipe.c) \
			$(addprefix builtin/, builtin_handler.c builtin_pwd_env.c builtin_echo.c \
				builtin_exit.c builtin_cd.c builtin_export.c builtin_unset.c) \
			$(addprefix execute/, command_init.c command_heredoc.c process_command_tokens.c \
				command_files.c execute_external.c process_execution.c process_redirections.c \
				path_builder.c) \
			$(addprefix signals/, prompt_signals.c pipe_signals.c heredoc_signals.c \
				child_signals.c) \
)

OBJECTS := $(patsubst $(SOURCES_DIRECTORY)/%.c,$(OBJECTS_DIRECTORY)/%.o, $(SOURCES))

# test framework
CUNIT_DIRECTORY := $(ROOT_DIR)/lib/cunit
CUNIT := $(CUNIT_DIRECTORY)/lib/libcunit.a
CUNIT_LIB := -L$(CUNIT_DIRECTORY)/lib -lcunit
TEST_SOURCES := $(addprefix test/, \
		test_runner.c  test_builtin_cd.c test_builtin_echo.c test_builtin_exit.c test_builtin_export.c \
		test_builtin_pwd_env.c test_builtin_unset.c test_variable_expansion.c test_env_utils.c \
		test_command_files.c test_command_heredoc.c test_command_init.c test_path_builder.c  \
		test_process_redirections.c test_lexer_assign_type.c test_lexer_main.c test_lexer_process.c \
		test_lexer_state_handler.c test_lexer_state_utils.c test_lexer_token.c test_heredoc_cleanup.c \
		test_heredoc_creation.c test_heredoc_handler.c test_heredoc_loop.c \
)

TEST_HEADER := $(ROOT_DIR)/test/test.h
TEST_OBJECTS := $(patsubst %.c, $(OBJECTS_DIRECTORY)/%.o, $(TEST_SOURCES))
TEST_EXEC := test_runner

# libft
LIBFT_DIRECTORY := $(ROOT_DIR)/lib/libft
LIBFT := $(LIBFT_DIRECTORY)/libft.a

CFLAGS	= -Wall -Wextra -Werror -g -Iinclude -I$(CUNIT_DIRECTORY)/include -I$(LIBFT_DIRECTORY)

all: $(OBJECTS_DIRECTORY) $(LIBFT) $(NAME)

YELLOW=\033[0;33m
GREEN=\033[0;32m
NC=\033[0m

$(CUNIT):
	@if [ ! -d "$(CUNIT_DIRECTORY)" ]; then \
		echo "$(YELLOW)CUnit not found! Downloading and building...$(NC)"; \
		mkdir -p $(ROOT_DIR)/lib && cd $(ROOT_DIR)/lib && \
		wget https://sourceforge.net/projects/cunit/files/CUnit/2.1-3/CUnit-2.1-3.tar.bz2 && \
		tar -xvf CUnit-2.1-3.tar.bz2 && mv CUnit-2.1-3 cunit && rm CUnit-2.1-3.tar.bz2; \
	fi
	@echo "$(YELLOW)Building CUnit...$(NC)"
	@cd $(CUNIT_DIRECTORY) && autoreconf -fi && ./configure --prefix=$(CUNIT_DIRECTORY) && make && make install
	@echo "$(GREEN)CUnit compiled and installed$(NC)"

$(NAME): $(OBJECTS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LIBFT) -lreadline
	@echo "$(GREEN)$(NAME) generated$(NC)"

$(OBJECTS_DIRECTORY)/%.o: $(SOURCES_DIRECTORY)/%.c $(HEADERS) | $(OBJECTS_DIRECTORY)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_DIRECTORY):
	@mkdir -p $(OBJECTS_DIRECTORY)

$(TEST_EXEC): $(filter-out $(OBJECTS_DIRECTORY)/main.o, $(OBJECTS)) $(TEST_OBJECTS) $(LIBFT)
	@$(CC) $(TEST_OBJECTS) $(filter-out $(OBJECTS_DIRECTORY)/main.o, $(OBJECTS)) $(LIBFT) $(CUNIT_LIB) -lreadline -o $(TEST_EXEC) -Wl,--gc-sections

$(OBJECTS_DIRECTORY)/test/%.o: test/%.c $(HEADERS) $(TEST_HEADER) | $(OBJECTS_DIRECTORY)/test
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_DIRECTORY)/test:
	@mkdir -p $(OBJECTS_DIRECTORY)/test

$(LIBFT):
	@make -C $(LIBFT_DIRECTORY) --no-print-directory
	@make bonus -C $(LIBFT_DIRECTORY) --no-print-directory
	@echo "$(GREEN)Libft compiled$(NC)"

clean:
	@rm -rf $(OBJECTS_DIRECTORY)
	@make -s -C $(LIBFT_DIRECTORY) fclean >/dev/null 2>&1
	@rm -rf $(CUNIT_DIRECTORY) clean >/dev/null 2>&1
	@rm -f $(TEST_EXEC)

fclean: clean
	@rm -f $(NAME)

re: fclean all

test: $(CUNIT) $(TEST_EXEC)
	@echo "$(GREEN)Running tests...$(NC)"
	@export LD_LIBRARY_PATH=$(CUNIT_DIRECTORY)/lib:$$LD_LIBRARY_PATH && ./$(TEST_EXEC) || true

.PHONY: all clean fclean re
