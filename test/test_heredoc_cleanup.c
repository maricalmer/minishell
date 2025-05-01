/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heredoc_cleanup.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_files *create_heredoc_files(int count) {
	t_files *head = NULL;
	t_files *prev = NULL;

	for (int i = 0; i < count; ++i) {
		t_files *node = malloc(sizeof(t_files));
		node->fd = open("/dev/null", O_RDONLY);  // dummy fd
		node->next = NULL;

		if (prev)
			prev->next = node;
		else
			head = node;
		prev = node;
	}
	return head;
}

// Wrap a t_files in a t_list node
t_list *wrap_heredoc_in_list(t_files *files) {
	t_list *lst = malloc(sizeof(t_list));
	lst->content = files;
	lst->next = NULL;
	return lst;
}

// === TESTS ===

void test_close_all_heredocs(void) {
	t_minishell shell;
	t_files *files = create_heredoc_files(3);
	t_list *heredoc_list = wrap_heredoc_in_list(files);

	shell.heredocs = heredoc_list;

	// Confirm fds are valid before
	t_files *node = files;
	while (node) {
		CU_ASSERT_NOT_EQUAL(node->fd, -1);
		node = node->next;
	}

	close_all_heredocs(&shell);

	node = files;
	while (node) {
		CU_ASSERT_EQUAL(node->fd, -1);  // Should all be closed now
		node = node->next;
	}

	// Cleanup
	node = files;
	while (node) {
		t_files *tmp = node;
		node = node->next;
		free(tmp);
	}
	free(heredoc_list);
}

void test_heredoc_warning_message(void) {
	// Redirect stdout to a buffer
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	FILE *original_stdout = stdout;
	FILE *temp = fmemopen(buffer, sizeof(buffer), "w");
	stdout = temp;

	heredoc_warning_message();
	fflush(temp);

	stdout = original_stdout;
	fclose(temp);

    CU_ASSERT_PTR_NOT_NULL(strstr(buffer, "minishell: warning: here-document"));
    CU_ASSERT_PTR_NOT_NULL(strstr(buffer, "delimited by end-of-file"));
}

int add_heredoc_cleanup_tests(void)
{
    CU_pSuite suite = CU_add_suite("heredoc_cleanup", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_close_all_heredocs", test_close_all_heredocs) ||
        !CU_add_test(suite, "test_heredoc_warning_message", test_heredoc_warning_message))
    {
        return (1);
    }
    return (0);
}