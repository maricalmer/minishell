/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heredoc_creation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void test_create_heredoc_file(void) {
	t_minishell shell = {0};
	t_files *file = create_heredoc_file("EOF", 1, &shell);

	CU_ASSERT_PTR_NOT_NULL(file);
	CU_ASSERT_STRING_EQUAL(file->delim, "EOF");
	CU_ASSERT_EQUAL(file->fd, -1);
	CU_ASSERT_EQUAL(file->heredoc_quote, 1);
	CU_ASSERT_PTR_NULL(file->next);
}

void test_create_heredoc_list(void) {
	t_minishell shell = {0};
	t_list *list = create_heredoc_list(&shell);

	CU_ASSERT_PTR_NOT_NULL(list);
	CU_ASSERT_PTR_NULL(list->content);
	CU_ASSERT_PTR_NULL(list->next);
}

void test_add_heredoc_to_list(void) {
	t_files *head = NULL;
	t_files *file1 = malloc(sizeof(t_files));
	t_files *file2 = malloc(sizeof(t_files));
	file1->next = NULL;
	file2->next = NULL;

	add_heredoc_to_list(&head, file1);
	CU_ASSERT_PTR_EQUAL(head, file1);
	CU_ASSERT_PTR_NULL(head->next);

	add_heredoc_to_list(&head, file2);
	CU_ASSERT_PTR_EQUAL(head->next, file2);
	CU_ASSERT_PTR_NULL(head->next->next);

	free(file1);
	free(file2);
}

void test_add_list_to_outer_list(void) {
	t_list *head = NULL;
	t_list *l1 = malloc(sizeof(t_list));
	t_list *l2 = malloc(sizeof(t_list));
	l1->next = NULL;
	l2->next = NULL;

	add_list_to_outer_list(&head, l1);
	CU_ASSERT_PTR_EQUAL(head, l1);
	CU_ASSERT_PTR_NULL(head->next);

	add_list_to_outer_list(&head, l2);
	CU_ASSERT_PTR_EQUAL(head->next, l2);
	CU_ASSERT_PTR_NULL(head->next->next);

	free(l1);
	free(l2);
}

int add_heredoc_creation_tests(void)
{
    CU_pSuite suite = CU_add_suite("heredoc_creation", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_create_heredoc_file", test_create_heredoc_file) ||
        !CU_add_test(suite, "test_create_heredoc_list", test_create_heredoc_list) ||
        !CU_add_test(suite, "test_add_heredoc_to_list", test_add_heredoc_to_list) ||
        !CU_add_test(suite, "test_add_list_to_outer_list", test_add_list_to_outer_list))
    {
        return (1);
    }
    return (0);
}