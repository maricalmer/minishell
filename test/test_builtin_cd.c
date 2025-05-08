/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_cd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 23:05:59 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	init_shell_mock(t_minishell *shell)
{
	shell->env = malloc(sizeof(char *) * 3);
	shell->env[0] = strdup("PWD=/mock/pwd");
	shell->env[1] = strdup("OLDPWD=/mock/oldpwd");
	shell->env[2] = strdup("HOME=/tmp");
	shell->env[3] = NULL;

	shell->last_exit_status = SHELL_STATUS_SUCCESS;
	shell->in_pipe = 0;
	shell->gc_head = NULL;
	shell->tokens = NULL;
	shell->ast_root = NULL;
	shell->cmd = NULL;
	shell->heredocs = NULL;
	shell->sigint_heredocs = 0;
}

int	get_target_path_mock(char **args, t_minishell *shell, char **path)
{
	(void)shell;
	if (args[1] == NULL || strcmp(args[1], "~") == 0)
	{
		*path = "/mock/home";
		if (*path == NULL || strlen(*path) == 0)
			return (1);
	}
	else
		*path = args[1];
	return (0);
}

int	update_environment_mock(char *oldpwd, t_minishell *shell)
{
	(void)oldpwd;
	(void)shell;
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

	init_shell_mock(&shell);
	path = NULL;

	char *args_no_arg[] = { "cd", NULL };
	int result = get_target_path_mock(args_no_arg, &shell, &path);
	CU_ASSERT_EQUAL(result, 0);
	CU_ASSERT_STRING_EQUAL(path, "/mock/home");

	char *args_path[] = { "cd", "/usr/local", NULL };
	result = get_target_path_mock(args_path, &shell, &path);
	CU_ASSERT_EQUAL(result, 0);
	CU_ASSERT_STRING_EQUAL(path, "/usr/local");
}

void	test_change_directory(void)
{
	int result_valid = change_directory("/tmp");
	int result_invalid = change_directory("/this/path/should/not/exist");

	CU_ASSERT_EQUAL(result_valid, 0);
	CU_ASSERT_EQUAL(result_invalid, 1);
}

void	test_update_environment_mocked(void)
{
	t_minishell	shell;

	char oldpwd[PATH_MAX] = "/mock/oldpwd";
	int result = update_environment_mock(oldpwd, &shell);
	CU_ASSERT_EQUAL(result, 0);
}

void	test_builtin_cd(void)
{
	t_minishell shell = create_mock_shell(42, 0);
	init_shell_mock(&shell);
	char *args_no_arg[] = { "cd", NULL };
	char *args_path[] = { "cd", "/tmp", NULL };
	char *args_too_many[] = { "cd", "/tmp", "/another", NULL };

	int result_no_arg = builtin_cd(args_no_arg, &shell);
	int result_path = builtin_cd(args_path, &shell);
	int result_too_many = builtin_cd(args_too_many, &shell);

	CU_ASSERT_EQUAL(result_no_arg, 0);
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
		!CU_add_test(suite, "update_environment_mocked", test_update_environment_mocked) ||
		!CU_add_test(suite, "test_builtin_cd", test_builtin_cd))
	{
		return (1);
	}
	return (0);
}

