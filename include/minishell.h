/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:08:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/18 19:19:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* libraries */
# include "../libft/libft.h"
/* printf, perror */
# include <stdio.h>
/* malloc, free, exit, getenv */
# include <stdlib.h>
/* readline, rl_clear_history, rl_on_new_line */
# include <readline/readline.h>
/* add_history */
# include <readline/history.h>
/*	read, write, access, close, unlink,
	fork, execve, getpid, getppid,
	isatty, ttyname, ttyslot
	getcwd, chdir
	dup, dup2, pipe	*/
# include <unistd.h>
/* open */
# include <fcntl.h>
/* opendir, readdir, closedir */
# include <dirent.h>
/* fork, execve */
# include <sys/types.h>
/* wait, waitpid, wait3, wait4 */
# include <sys/wait.h>
/* stat, lstat, fstat */
# include <sys/stat.h>
/* ioctl */
# include <sys/ioctl.h>
/* signal, sigaction, sigemptyset, sigaddset, kill */
# include <signal.h>
/* tcsetattr, tcgetattr */
# include <termios.h>
/* tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs */
# include <curses.h>
/* perror */
# include <errno.h>
/* PATH_MAX */
# include <limits.h>

/* macros*/
# define TOKEN_COMPLETE 1
# define TOKEN_CONTINUE 0
# define TOKEN_ERROR -1
# define MIN_PRECEDENCE_LVL 0
# define READ_END 0
# define WRITE_END 1

/* structures */
typedef struct s_token		t_token;
typedef struct s_ast_node	t_ast_node;
typedef struct s_command	t_command;
typedef struct s_files		t_files;

/* context structure to group data together and improve code organization */
typedef struct s_minishell
{
	char					**env;
	int						last_exit_status;
	int						in_pipe;
	t_list					*gc_head;
	t_token					*tokens;
	t_ast_node				*ast_root;
	t_command				*cmd;
	t_list					*heredocs;
	int						sigint_heredocs;
}							t_minishell;

/* global variable to handle signals */
extern int					g_received_signal;

/* enumerate all possible token types in minishell */
typedef enum e_token_type
{
	TOKEN_INVALID,
	TOKEN_BUILTIN_CMD,
	TOKEN_EXTERN_CMD,
	TOKEN_ARGUMENT,
	TOKEN_ENV_VARIABLE,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_HEREDOC_DELIMITER,
	TOKEN_FILENAME,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_OP_PARENTHESIS,
	TOKEN_CL_PARENTHESIS
}							t_token_type;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
}							t_node_type;

typedef struct s_token
{
	char					*value;
	t_token_type			type;
	struct s_token			*next;
}							t_token;

/* enumerate 3 possible states for minishell */
typedef enum e_lexer_state
{
	DEFAULT_STATE,
	SINGLE_QUOTE_STATE,
	DOUBLE_QUOTE_STATE,
}							t_lexer_state;

typedef struct s_lexer
{
	const char				*str;
	size_t					pos;
	char					current_char;
	t_lexer_state			state;
	int						command_expected;
}							t_lexer;

typedef struct s_files
{
	int						fd;
	char					*delim;
	int						heredoc_quote;
	struct s_files			*next;
}							t_files;

typedef struct s_command
{
	t_files					*infile;
	t_files					*outfile;
	char					*cmd_name;
	char					*path;
	char					**args;
	int						has_heredoc;
	int						redirect_error_flag;
}							t_command;

typedef struct s_ast_node
{
	t_token					*tokens;
	t_node_type				type;
	struct s_ast_node		*left;
	struct s_ast_node		*right;
}							t_ast_node;

/* function prototypes */
/* input_handling.c */
void						main_input_loop(int ac, char **av,
								t_minishell *shell);
void						handle_multiple_args(int ac, char **av,
								t_minishell *shell);
char						*read_user_input(void);
int							handle_empty_input(char *input);
void						process_valid_input(char *input,
								t_minishell *shell);

/* syntax_validation.c */
int							handle_syntax_error(char *input,
								t_minishell *shell);
int							validate_heredoc_delimiter(char *str);
int							validate_delimiter_left(const char *str);
int							validate_delimiter_right(const char *str);
int							validate_delimiter_middle(const char *str);
int							validate_balanced_quotes(const char *str);

/* syntax_validation_utils.c */
char						*skip_whitespace_input(char *str);
const char					*skip_operator_right(const char *str,
								const char **last_valid, int *offset);
const char					*handle_pipe_middle(const char *str);
const char					*handle_and_middle(const char *str);
int							check_heredoc_char(char **str);

/* delimiter_utils.c */
int							check_left_char(const char *str);
int							check_right_end(const char *last_valid, int offset);
const char					*process_delimiter_char(const char *str,
								int *in_quotes, char *quote_char);
const char					*process_opening_char(const char *str,
								int *in_quotes, char *quote_char,
								char *prev_char);

/* quotes_handler.c */
int							handle_quotes(char *str, int *in_quotes,
								char *quote_char);
int							valid_open_prevchar(char prev_char);
int							check_closing(const char **str);
int							check_empty_parenthesis(const char **str);
int							validate_no_empty_parenthesis(const char *str);

/* parenthesis_validation.c */
int							validate_balanced_quotes(const char *str);
int							validate_balanced_parenthesis(const char *str);
int							validate_opening_parenthesis(const char *str);
int							validate_closing_parenthesis(const char *str);
void						update_quote_states(char c, int *in_single_quote,
								int *in_double_quote);

/* lexer_main.c */
void						run_lexer(char *str, t_minishell *shell);
void						initialize_lexer_and_tokens(char *str,
								t_lexer *lexer, t_minishell *shell);
t_lexer						init_lexer(const char *arg);

/* lexer_process.c */
int							process_lexer_tokens(t_lexer *lexer,
								t_minishell *shell);
void						skip_whitespace(t_lexer *lexer);
void						add_closing_parentheses(int count,
								t_minishell *shell);
int							count_remove_trailing_parenth(char *value);
int							handle_unclosed_quotes(t_lexer *lexer,
								t_minishell *shell);

/* lexer_token.c */
t_token						*get_next_token(t_lexer *lexer, t_minishell *shell);
t_token						*create_token(t_token_type type, char *value,
								t_minishell *shell);

/* lexer_assign_types.c */
int							is_special_character(t_lexer *lexer);
t_token_type				handle_special_char_token(t_lexer *lexer,
								char **value, t_minishell *shell);
char						*collect_special_character(t_lexer *lexer,
								t_minishell *shell);
t_token_type				get_special_character_token_type(char *value);
t_token_type				handle_regular_token(t_lexer *lexer, char **value,
								t_minishell *shell);

/* lexer_state_handler.c */
char						*collect_token(t_lexer *lexer, t_minishell *shell);
int							handle_default_state(t_lexer *lexer, char **buffer,
								t_minishell *shell);
int							handle_single_quote_state(t_lexer *lexer,
								char **buffer, t_minishell *shell);
int							handle_double_quote_state(t_lexer *lexer,
								char **buffer, t_minishell *shell);

/* lexer_state_utils.c */
int							handle_dollar_sign(t_lexer *lexer, char **buffer,
								t_minishell *shell);
int							advance_and_append(t_lexer *lexer, char **buffer,
								t_minishell *shell);

/* lexer_utils.c */
int							count_tokens(t_token *tokens);
void						token_to_list(t_token **tokens, t_token *new);
void						update_redirect_tokens(t_token *tokens,
								t_minishell *shell);
void						advance_lexer_char(t_lexer *lexer);
t_token						*get_last_token(t_token *lst);

/* variable_expansion.c */
int							handle_variable_expansion(t_lexer *lexer,
								char **buffer, t_minishell *shell);
char						*collect_variable_name(t_lexer *lexer,
								t_minishell *shell);
int							get_variable_name_length(const char *str);
char						*get_variable_value(const char *var_name,
								t_minishell *shell);
int							append_to_buffer(char **buffer, const char *str,
								t_minishell *shell);

/* parser.c */
t_ast_node					*parse_expression(t_minishell *shell,
								int precedence_threshold);
t_ast_node					*parse_condition(t_minishell *shell);
t_ast_node					*parse_command(t_minishell *shell);
t_ast_node					*create_ast_node(t_node_type type, t_ast_node *left,
								t_ast_node *right, t_minishell *shell);

/* parser_utils.c */
int							is_statement_delimiter(int type);
int							get_precedence_lvl(int type);
void						ft_tkadd_back(t_token **tokens, t_token *new);

/* heredoc_init.c */
void						init_heredoc(t_minishell *shell, t_token *token);
int							create_heredoc_pipe(int fd[2]);
void						fork_heredoc_child(t_minishell *shell,
								t_token *token, int fd[2], t_files *heredoc);
void						handle_heredoc_parent(pid_t pid, t_minishell *shell,
								int fd[2]);
t_files						*save_heredoc_fd(t_list *heredoc_list, int new_fd);

/* heredoc_loop.c */
void						heredoc_loop(t_minishell *shell, t_token *token,
								int *pipe, t_files *heredoc);
int							is_heredoc_delimiter(const char *input,
								const char *delimiter);
void						catch_heredoc_input(t_minishell *shell, char *str,
								int fd, t_files *heredoc);
int							expend_in_heredoc(t_minishell *shell, char *str,
								int fd);
char						*get_env_variable(const char *var_name, char **env);

/* heredoc_cleanup.c */
void						close_heredoc_list(t_minishell *shell);
void						close_all_heredocs(t_minishell *shell);
void						heredoc_warning_message(void);

/* heredoc_handler.c */
void						handle_heredoc_token_in_input(char **input,
								t_minishell *shell, t_list **outer_list,
								t_list **current_list);
void						handle_pipe_token(char **input,
								t_list **current_list);
void						heredoc_scan(char *input, t_minishell *shell);
char						*get_delimiter(char *input, t_minishell *shell);
int							is_quoted_delimiter(char *delimiter);

/* heredoc_creation.c */
t_files						*create_heredoc_file(char *delimiter, int is_quoted,
								t_minishell *shell);
t_list						*create_heredoc_list(t_minishell *shell);
void						add_heredoc_to_list(t_files **head,
								t_files *new_file);
void						add_list_to_outer_list(t_list **head,
								t_list *new_list);

/* interpreter.c */
void						read_tree(t_ast_node *root, t_minishell *shell);
void						evaluate_and_execute(t_ast_node *node,
								t_minishell *shell);
void						execute_command_node(t_ast_node *node,
								t_minishell *shell);
void						execute_logical_operator_node(t_ast_node *node,
								t_minishell *shell);

/* prompt_signals.c */
void						setup_prompt_signals(t_minishell *shell);
void						setup_sigint_handler(t_minishell *shell);
void						handle_sigint_at_prompt(int sig);
void						setup_sigquit_handler(t_minishell *shell);
void						setup_sigpipe_handler(t_minishell *shell);

/* pipe_signals.c */
void						setup_pipe_signals(t_minishell *shell);
void						setup_sigint_handler_at_pipe(t_minishell *shell);
void						handle_sigint_at_pipe(int sig);

/* child_signals.c */
void						reset_signal_handlers(t_minishell *shell);
void						ignore_signal_handlers(t_minishell *shell);

/* heredoc_signals.c */
void						setup_heredoc_signals(t_minishell *shell);
void						setup_sigint_handler_at_heredoc(t_minishell *shell);
void						handle_sigint_at_heredoc(int sig);
void						ignore_sigquit_at_heredoc(t_minishell *shell);
void						reset_sigint_at_heredoc(t_minishell *shell);

/* command_init.c */
void						init_command(t_command *cmd, t_token *node_tokens,
								t_minishell *shell);
void						initialize_command_struct(t_command *cmd);
int							count_arg_tokens(t_token *tokens);
t_token						*process_token(t_command *cmd, t_token *token,
								t_minishell *shell, int *i);
int							process_argument(t_command *cmd, t_token *token,
								t_minishell *shell, int i);

/* command_heredoc.c */
t_token						*handle_heredoc_token(t_command *cmd,
								t_token *token, t_minishell *shell);
void						add_heredoc_to_cmd(t_command *cmd, char *delimiter,
								t_minishell *shell);
int							get_last_heredoc_fd(t_list *heredocs);

/* process_command_tokens.c */
int							process_builtin_cmd(t_command *cmd, t_token *token,
								t_minishell *shell, int i);
int							process_extern_cmd(t_command *cmd, t_token *token,
								t_minishell *shell, int i);
t_token						*process_redirect_in(t_command *cmd, t_token *token,
								t_minishell *shell);
t_token						*process_redirect_out(t_command *cmd,
								t_token *token, t_minishell *shell);
t_token						*process_redirect_append(t_command *cmd,
								t_token *token, t_minishell *shell);

/* command_files.c */
t_files						*create_new_file_node(t_minishell *shell);
void						append_file_node(t_files **file_list,
								t_files *new_file);
void						add_infile_to_cmd(t_command *cmd, char *filename,
								t_minishell *shell);
void						add_outfile_to_cmd(t_command *cmd, char *filename,
								t_minishell *shell, int append_flag);

/* path_builder.c */
char						*build_command_path(char *cmd, t_minishell *shell);
char						**create_directories(t_minishell *shell);
char						*find_executable_path(char *str, char **directories,
								t_minishell *shell);

/* execute_external.c */
void						execute_external(t_command *cmd, char **env,
								t_minishell *shell);
void						handle_command_not_found(char *cmd_name,
								t_minishell *shell);
void						fork_and_execute(t_command *cmd, char **env,
								t_minishell *shell);

/* process_execution.c */
void						execute_command_child(t_command *cmd, char **env,
								t_minishell *shell);
void						validate_command(t_command *cmd,
								t_minishell *shell);
void						print_error_and_exit(char *cmd_name, char *message,
								int exit_code, t_minishell *shell);
void						handle_parent_process(pid_t pid,
								t_minishell *shell);

/* process_redirections.c */
t_files						*get_last_file(t_files *files);
void						setup_redirections(t_files *infile,
								t_files *outfile, t_minishell *shell);

/* builtin_handler.c */
int							is_builtin_command(const char *word);
int							execute_builtin(t_command *cmd, t_minishell *shell);
int							execute_builtin_echo(t_command *cmd);
void						print_builtin_error(char *command, char *message);
void						print_numeric_argument_required(char *arg);

/* builtin_pwd_env.c */
int							builtin_pwd(t_command *cmd);
int							builtin_env(t_command *cmd, t_minishell *shell);
int							too_many_arguments(char **args);
int							cd_too_many_arguments(char **args);

/* builtin_pwd_env.c */
int							builtin_echo(t_command *cmd);
void						parse_echo_flags(char **args, int *i, int *newline);
int							is_all_n_flag(char *arg);
void						print_arguments(char **args, int i, int newline,
								int output_fd);
int							print_export_without_args(t_minishell *shell);

/* builtin_exit.c */
int							handle_exit_command(char **args,
								t_minishell *shell);
int							handle_exit_with_arguments(char **args,
								t_minishell *shell);
int							handle_numeric_argument(char *arg,
								t_minishell *shell);
char						*remove_surrounding_quotes(char *arg,
								t_minishell *shell);
int							is_numeric_argument(const char *arg);

/* builtin_cd.c */
int							builtin_cd(char **args, t_minishell *shell);
int							get_current_directory(char *cwd);
int							get_target_path(char **args, t_minishell *shell,
								char **path);
int							change_directory(char *path);
int							update_environment(char *oldpwd,
								t_minishell *shell);

/* builtin_export.c */
int							builtin_export(char **args, t_minishell *shell);
int							process_export_argument(const char *arg,
								t_minishell *shell);
int							is_valid_identifier(const char *arg);
int							print_export_error(const char *arg);

/* builtin_unset.c */
int							builtin_unset(char **args, t_minishell *shell);
int							process_unset_argument(const char *arg,
								t_minishell *shell);
int							is_valid_env_name(const char *name);
int							unset_env_variable(const char *name,
								t_minishell *shell);
int							find_env_index(const char *name,
								t_minishell *shell);

/* environment_utils.c */
char						**duplicate_env(char **envp, t_minishell *shell);
int							set_env_variable(const char *name,
								const char *value, t_minishell *shell);
char						*create_env_string(const char *name,
								const char *value, t_minishell *shell);
int							add_env_variable(char *new_var, t_minishell *shell);

/* pipe.c */
void						init_pipe(t_ast_node *node, t_minishell *shell);
int							create_pipe_fds(int fds[2]);
pid_t						fork_left_child(int fds[], t_ast_node *node,
								t_minishell *shell);
pid_t						fork_right_child(int fds[], t_ast_node *node,
								t_minishell *shell);

#endif
