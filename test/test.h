/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:56:02 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:40:32 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <CUnit/Basic.h>
# include <CUnit/CUnit.h>
# include <locale.h>
# include "minishell.h"


void        init_shell_mock(t_minishell *shell);
t_minishell create_mock_shell(int last_status, int in_pipe);
int		    add_builtin_cd_tests(void);
int		    add_builtin_echo_tests(void);
int		    add_builtin_exit_tests(void);
int		    add_builtin_export_tests(void);
int		    add_builtin_pwd_env_tests(void);
int         add_builtin_unset_tests(void);
int         add_variable_expansion_tests(void);
int         add_env_utils_tests(void);
int         add_command_files_tests(void);
int         add_command_heredoc_tests(void);
int         add_command_init_tests(void);
int         add_path_builder_tests(void);
int         add_process_redirections_tests(void);
int         add_lexer_assign_type_tests(void);
int         add_lexer_main_tests(void);
int         add_lexer_process_tests(void);
int         add_lexer_state_handler_tests(void);
int         add_lexer_state_utils_tests(void);
int         add_lexer_token_tests(void);
int         add_heredoc_cleanup_tests(void);
int         add_heredoc_creation_tests(void);
int         add_heredoc_handler_tests(void);
int         add_heredoc_loop_tests(void);

#endif
