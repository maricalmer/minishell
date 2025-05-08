/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_echo.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 19:37:57 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_command	*create_mock_command(char **args, int fd)
{
	t_command *cmd = malloc(sizeof(t_command));

	cmd->args = args;

	if (fd != STDOUT_FILENO) {
		t_files *file = malloc(sizeof(t_files));
		file->fd = fd;
		file->next = NULL;
		cmd->outfile = file;
	}
	else
	{
		cmd->outfile = NULL;
	}
	return (cmd);
}

int	capture_output(char **args, char *buffer, size_t buf_size)
{
	int	pipe_fds[2];

	pipe(pipe_fds);
	t_command *cmd = create_mock_command(args, pipe_fds[1]);
	builtin_echo(cmd);
	close(pipe_fds[1]);

	ssize_t n = read(pipe_fds[0], buffer, buf_size - 1);
	buffer[n] = '\0';

	close(pipe_fds[0]);
	free(cmd->outfile);
	free(cmd);

	return ((int)n);
}

void	test_echo_simple_text(void)
{
	char *args[] = { "echo", "Hello", "World", NULL };
	char output[100];

	int bytes = capture_output(args, output, sizeof(output));

	CU_ASSERT_STRING_EQUAL(output, "Hello World\n");
	CU_ASSERT_TRUE(bytes > 0);
}

void	test_echo_no_newline_flag(void)
{
	char *args[] = { "echo", "-n", "No", "newline", NULL };
	char output[100];

	int bytes = capture_output(args, output, sizeof(output));

	CU_ASSERT_STRING_EQUAL(output, "No newline");
	CU_ASSERT_TRUE(bytes > 0);
}

void	test_echo_multiple_n_flags(void)
{
	char *args[] = { "echo", "-n", "-n", "-n", "Still", "no", "newline", NULL };
	char output[100];

	int bytes = capture_output(args, output, sizeof(output));

	CU_ASSERT_STRING_EQUAL(output, "Still no newline");
	CU_ASSERT_TRUE(bytes > 0);
}

void	test_echo_no_arguments(void)
{
	char *args[] = { "echo", NULL };
	char output[100];

	int bytes = capture_output(args, output, sizeof(output));

	CU_ASSERT_STRING_EQUAL(output, "\n");
	CU_ASSERT_TRUE(bytes > 0);
}

int	add_builtin_echo_tests(void)
{
	CU_pSuite suite = CU_add_suite("builtin_cd", 0, 0);

	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_echo_simple_text", test_echo_simple_text) ||
		!CU_add_test(suite, "test_echo_no_newline_flag", test_echo_no_newline_flag) ||
		!CU_add_test(suite, "test_echo_multiple_n_flags", test_echo_multiple_n_flags) ||
		!CU_add_test(suite, "test_echo_no_arguments", test_echo_no_arguments))
		return (1);
	return (0);
}

