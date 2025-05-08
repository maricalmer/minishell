/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heredoc_loop.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 20:02:46 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	test_is_heredoc_delimiter(void)
{
	CU_ASSERT_TRUE(is_heredoc_delimiter("END", "END"));
	CU_ASSERT_TRUE(is_heredoc_delimiter("END\n", "END"));
	CU_ASSERT_FALSE(is_heredoc_delimiter("EN", "END"));
	CU_ASSERT_FALSE(is_heredoc_delimiter("ENDsomething", "END"));
}

void	test_get_env_variable_found(void)
{
	char *env[] = {"USER=root", "PATH=/bin", NULL};
	CU_ASSERT_STRING_EQUAL(get_env_variable("PATH", env), "/bin");
}

void	test_get_env_variable_not_found(void)
{
	char *env[] = {"USER=root", "PATH=/bin", NULL};
	CU_ASSERT_PTR_NULL(get_env_variable("FOO", env));
}

void	test_expend_in_heredoc(void)
{
	char *env[] = {"FOO=bar", NULL};
	char str[64] = "$FOO rest";
	int fds[2];
	pipe(fds);

	int len = expend_in_heredoc(&(t_minishell){.env = env}, str, fds[1]);
	close(fds[1]);
	char buf[10] = {0};
	read(fds[0], buf, 9);
	close(fds[0]);

	CU_ASSERT_STRING_EQUAL(buf, "bar");
	CU_ASSERT_EQUAL(len, 4);
}

void	test_catch_heredoc_input_quoted_no_expansion(void)
{
	t_files heredoc = {.heredoc_quote = 1};
	int fds[2];
	pipe(fds);
	catch_heredoc_input(&(t_minishell){0}, "$NOT_EXPANDED", fds[1], &heredoc);
	close(fds[1]);

	char buf[128] = {0};
	read(fds[0], buf, 127);
	close(fds[0]);

	CU_ASSERT_STRING_EQUAL(buf, "$NOT_EXPANDED\n");
}

int	add_heredoc_loop_tests(void)
{
    CU_pSuite suite = CU_add_suite("heredoc_loop", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_is_heredoc_delimiter", test_is_heredoc_delimiter) ||
        !CU_add_test(suite, "test_get_env_variable_found", test_get_env_variable_found) ||
        !CU_add_test(suite, "test_get_env_variable_not_found", test_get_env_variable_not_found) ||
        !CU_add_test(suite, "test_expend_in_heredoc", test_expend_in_heredoc) ||
        !CU_add_test(suite, "test_catch_heredoc_input_quoted_no_expansion", test_catch_heredoc_input_quoted_no_expansion))
    {
        return (1);
    }
    return (0);
}