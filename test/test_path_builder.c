/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_path_builder.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 20:11:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_minishell shell;
char *env_with_path[] = {
	"USER=test",
	"PATH=/bin:/usr/bin:/tmp",
	NULL
};

static void	setup_shell_env(void)
{
	shell.gc_head = NULL;
	shell.env = env_with_path;
}

void	test_build_command_path_absolute_valid(void)
{
	setup_shell_env();

	const char *cmd = "/tmp/test_cmd.sh";
	int fd = open(cmd, O_CREAT | O_WRONLY, 0755);
	write(fd, "#!/bin/sh\necho Hello\n", 21);
	close(fd);

	char *result = build_command_path("/tmp/test_cmd.sh", &shell);
	CU_ASSERT_PTR_NOT_NULL(result);
	CU_ASSERT_STRING_EQUAL(result, "/tmp/test_cmd.sh");

	unlink(cmd);
}

void	test_build_command_path_absolute_invalid(void)
{
	setup_shell_env();
	char *result = build_command_path("/nonexistent/cmd", &shell);
	CU_ASSERT_PTR_NULL(result);
}

void	test_build_command_path_in_path(void)
{
	setup_shell_env();

	char *result = build_command_path("ls", &shell);
	CU_ASSERT_PTR_NOT_NULL(result);
	CU_ASSERT_STRING_EQUAL(result, "/bin/ls");

	free(result);
}

void	test_create_directories(void)
{
	setup_shell_env();
	char **dirs = create_directories(&shell);
	CU_ASSERT_PTR_NOT_NULL(dirs);
	CU_ASSERT_STRING_EQUAL(dirs[0], "/bin");
	CU_ASSERT_STRING_EQUAL(dirs[1], "/usr/bin");
	CU_ASSERT_STRING_EQUAL(dirs[2], "/tmp");
}

void	test_find_executable_path_found(void)
{
	setup_shell_env();

	char *dirs[] = { "/bin", "/usr/bin", NULL };
	char *result = find_executable_path("ls", dirs, &shell);
	CU_ASSERT_PTR_NOT_NULL(result);
	CU_ASSERT_STRING_EQUAL(result, "/bin/ls");
}

void	test_find_executable_path_not_found(void)
{
	setup_shell_env();

	char *dirs[] = { "/nonexistent", "/nowhere", NULL };
	char *result = find_executable_path("ghostcmd", dirs, &shell);
	CU_ASSERT_PTR_NULL(result);
}

int	add_path_builder_tests(void)
{
	CU_pSuite suite = CU_add_suite("builtin_pwd_env", 0, 0);

	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_build_command_path_absolute_valid", test_build_command_path_absolute_valid) ||
		!CU_add_test(suite, "test_build_command_path_absolute_invalid", test_build_command_path_absolute_invalid) ||
		!CU_add_test(suite, "test_build_command_path_in_path", test_build_command_path_in_path) ||
		!CU_add_test(suite, "test_create_directories", test_create_directories) ||
		!CU_add_test(suite, "test_find_executable_path_found", test_find_executable_path_found) ||
		!CU_add_test(suite, "test_find_executable_path_not_found", test_find_executable_path_not_found))
	{
		return (1);
	}
	return (0);
}
