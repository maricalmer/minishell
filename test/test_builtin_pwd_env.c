/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_pwd_env.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 19:43:42 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	test_builtin_pwd(void)
{
	t_command	cmd;

	char *pwd_args[] = {"pwd", NULL};
	cmd.args = pwd_args;
	cmd.outfile = NULL;
	int status = builtin_pwd(&cmd);
	CU_ASSERT_EQUAL(status, 0);
}

void	test_builtin_env(void)
{
	t_command	cmd;
	t_minishell	shell = create_mock_shell(42, 0);
	char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
	shell.env = env_vars;

	char *env_args[] = {"env", NULL};
	cmd.args = env_args;
	cmd.outfile = NULL;
	int status = builtin_env(&cmd, &shell);
	CU_ASSERT_EQUAL(status, 0);

	char *env_args_too_many[] = {"env", "extra", NULL};
	cmd.args = env_args_too_many;
	status = builtin_env(&cmd, &shell);
	CU_ASSERT_EQUAL(status, 127);
}

void	test_too_many_arguments(void)
{
	char *args_valid[] = {"env", NULL};
	char *args_invalid[] = {"env", "extra", NULL};

	int status = too_many_arguments(args_valid);
	CU_ASSERT_EQUAL(status, 0);

	status = too_many_arguments(args_invalid);
	CU_ASSERT_EQUAL(status, 1);
}

void	test_cd_too_many_arguments(void)
{
	char *args_valid[] = {"cd", "/home", NULL};
	char *args_invalid[] = {"cd", "/home", "extra", NULL};

	int status = cd_too_many_arguments(args_valid);
	CU_ASSERT_EQUAL(status, 0);

	status = cd_too_many_arguments(args_invalid);
	CU_ASSERT_EQUAL(status, 1);
}


int	add_builtin_pwd_env_tests(void)
{
	CU_pSuite suite = CU_add_suite("builtin_pwd_env", 0, 0);
	if (!suite)
		return (1);	
	if (!CU_add_test(suite, "test_builtin_pwd", test_builtin_pwd) ||
		!CU_add_test(suite, "test_builtin_env", test_builtin_env) ||
		!CU_add_test(suite, "test_too_many_arguments", test_too_many_arguments) ||
		!CU_add_test(suite, "test_cd_too_many_arguments", test_cd_too_many_arguments))
	{
		return (1);
	}
	return (0);
}
