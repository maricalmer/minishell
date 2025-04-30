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


void    init_shell_mock(t_minishell *shell);
int		add_builtin_cd_tests(void);
int		add_builtin_echo_tests(void);
int		add_builtin_exit_tests(void);
int		add_builtin_export_tests(void);
int		add_builtin_pwd_env_tests(void);
int     add_builtin_unset_tests(void);

#endif
