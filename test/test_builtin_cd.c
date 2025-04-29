/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_cd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

char	*mock_get_variable_value(const char *name)
{
	if (strcmp(name, "HOME") == 0)
	{
		return ("/home/user");
	}
	return (NULL);
}


int	mock_set_env_variable(const char *name, const char *value,
	t_minishell *shell)
{
	return (0);
}

void	test_get_current_directory(void)
{
	char	cwd[PATH_MAX];
	int		result;

	result = get_current_directory(cwd);
	CU_ASSERT_EQUAL(result, 0);
}

void	test_get_target_path(void)
{
	t_minishell	shell;
	char		*path;
	char		*args_no_arg[] = { "cd", NULL };
	int			result_no_arg;
	int			result_tilde;
	int			result_path;

	path = NULL;
	result_no_arg = get_target_path(args_no_arg, &shell, &path);
	CU_ASSERT_EQUAL(result_no_arg, 0);
	CU_ASSERT_STRING_EQUAL(path, "/home/user");

	char		*args_tilde[] = { "cd", "~", NULL };
	result_tilde = get_target_path(args_tilde, &shell, &path);
	CU_ASSERT_EQUAL(result_tilde, 0);
	CU_ASSERT_STRING_EQUAL(path, "/home/user");
	char		*args_path[] = { "cd", "/usr/local", NULL };
	result_path = get_target_path(args_path, &shell, &path);
	CU_ASSERT_EQUAL(result_path, 0);
	CU_ASSERT_STRING_EQUAL(path, "/usr/local");
	char		*args_empty_home[] = { "cd", "~", NULL };
	char		*args_empty_home_shell[] = { "cd", "~", NULL };
	shell.get_variable_value = mock_get_variable_value;
	result_no_home = get_target_path(args_empty_home, &shell, &path);
	CU_ASSERT_EQUAL(result_no_home, 1);
}

void	test_change_directory(void)
{
	int	result_valid = change_directory("/usr");
	int	result_invalid = change_directory("/invalid/path");

	CU_ASSERT_EQUAL(result_valid, 0);
	CU_ASSERT_EQUAL(result_invalid, 1);
}

void	test_update_environment(void)
{
	t_minishell shell;
	char		oldpwd[PATH_MAX] = "/home/user";
	int 		result = update_environment(oldpwd, &shell);

	shell.set_env_variable = mock_set_env_variable;
	CU_ASSERT_EQUAL(result, 0);
}

void	test_builtin_cd(void)
{
	t_minishell	shell;
	char		*args_no_arg[] = { "cd", NULL };
	int			result_no_arg = builtin_cd(args_no_arg, &shell);
	char		*args_tilde[] = { "cd", "~", NULL };
	int			result_tilde = builtin_cd(args_tilde, &shell);
	char		*args_path[] = { "cd", "/usr/local", NULL };
	int			result_path = builtin_cd(args_path, &shell);
	char		*args_too_many[] = { "cd", "/usr", "/home", NULL };
	int			result_too_many = builtin_cd(args_too_many, &shell);

	shell.get_variable_value = mock_get_variable_value;
	shell.set_env_variable = mock_set_env_variable;
	CU_ASSERT_EQUAL(result_no_arg, 0);
	CU_ASSERT_EQUAL(result_tilde, 0);
	CU_ASSERT_EQUAL(result_path, 0);
	CU_ASSERT_EQUAL(result_too_many, 1);
}

int	add_builtin_cd_tests(void)
{
	CU_pSuite suite = CU_add_suite("builtin_cd", 0, 0);

	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_get_current_directory", test_get_current_directory) ||
		!CU_add_test(suite, "test_get_target_path", test_get_target_path) ||
		!CU_add_test(suite, "test_change_directory", test_change_directory) ||
		!CU_add_test(suite, "test_update_environment", test_update_environment) ||
		!CU_add_test(suite, "test_builtin_cd", test_builtin_cd))
	{
		return (1);
	}
	return (0);
}
