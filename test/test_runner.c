/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_runner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 20:58:45 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:25:21 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main(void)
{
	int	failed;

	setlocale(LC_NUMERIC, "C");
	if (CU_initialize_registry() != CUE_SUCCESS)
		return (CU_get_error());
	if (add_builtin_cd_tests() != CUE_SUCCESS
			|| add_builtin_echo_tests() != CUE_SUCCESS
			|| add_builtin_exit_tests() != CUE_SUCCESS
			|| add_builtin_export_tests() != CUE_SUCCESS
			|| add_builtin_pwd_env_tests() != CUE_SUCCESS
			|| add_variable_expansion_tests() != CUE_SUCCESS
			|| add_env_utils_tests() != CUE_SUCCESS
			|| add_command_files_tests() != CUE_SUCCESS
			|| add_command_heredoc_tests() != CUE_SUCCESS
			|| add_command_init_tests() != CUE_SUCCESS
			|| add_path_builder_tests() != CUE_SUCCESS
			|| add_process_redirections_tests() != CUE_SUCCESS
			|| add_lexer_assign_type_tests() != CUE_SUCCESS
			|| add_lexer_main_tests() != CUE_SUCCESS
			|| add_lexer_process_tests() != CUE_SUCCESS
			|| add_lexer_state_handler_tests() != CUE_SUCCESS
			|| add_lexer_state_utils_tests() != CUE_SUCCESS
			|| add_lexer_token_tests() != CUE_SUCCESS)
	{
		CU_cleanup_registry();
		return (CU_get_error());
	}
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	failed = CU_get_number_of_failures();
	CU_cleanup_registry();
	if (failed > 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
