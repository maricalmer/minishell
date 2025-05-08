/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_process_redirections.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 20:11:49 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void    test_get_last_file(void)
{
	t_files file1 = { .fd = 1, .next = NULL };
	t_files file2 = { .fd = 2, .next = NULL };
	t_files file3 = { .fd = 3, .next = NULL };

	CU_ASSERT_PTR_EQUAL(get_last_file(&file1), &file1);
	file1.next = &file2;
	file2.next = &file3;
	CU_ASSERT_PTR_EQUAL(get_last_file(&file1), &file3);
	CU_ASSERT_PTR_EQUAL(get_last_file(NULL), NULL);
}

int	add_process_redirections_tests(void)
{
	CU_pSuite suite = CU_add_suite("process_redirections", 0, 0);

	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_get_last_file", test_get_last_file))
	{
		return (1);
	}
	return (0);
}
