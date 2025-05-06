/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:10:48 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/06 10:19:10 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Builds the abstract syntax tree (AST) from tokenized input.                */
/*  - Supports operator precedence for pipes, AND, OR, and parentheses.       */
/*  - Parses logical groupings (expressions), subshells (via parentheses),    */
/*    and command nodes.                                                      */
/*  - Creates AST nodes with appropriate type and subtree structure.          */
/*  - Handles heredoc initialization during command parsing.                  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*parse_expression(t_minishell *shell, int precedence_threshold)
{
	t_ast_node	*left;
	t_ast_node	*right;
	int			precedence_lvl;
	int			delimiter;

	left = parse_condition(shell);
	if (!left)
		return (NULL);
	while (shell->tokens && is_statement_delimiter(shell->tokens->type)
		&& get_precedence_lvl(shell->tokens->type) >= precedence_threshold)
	{
		delimiter = shell->tokens->type;
		precedence_lvl = get_precedence_lvl(delimiter) + 1;
		shell->tokens = shell->tokens->next;
		right = parse_expression(shell, precedence_lvl);
		if (!right)
			break ;
		if (delimiter == TOKEN_PIPE)
			left = create_ast_node(NODE_PIPE, left, right, shell);
		else if (delimiter == TOKEN_AND)
			left = create_ast_node(NODE_AND, left, right, shell);
		else if (delimiter == TOKEN_OR)
			left = create_ast_node(NODE_OR, left, right, shell);
	}
	return (left);
}

t_ast_node	*parse_condition(t_minishell *shell)
{
	t_ast_node	*node;

	if (is_statement_delimiter(shell->tokens->type)
		|| shell->tokens->type == TOKEN_CL_PARENTHESIS)
		perror(shell->tokens->value);
	if (shell->tokens->type == TOKEN_OP_PARENTHESIS)
	{
		shell->tokens = shell->tokens->next;
		if (!shell->tokens)
			return (NULL);
		node = parse_expression(shell, 0);
		if (shell->tokens && shell->tokens->type == TOKEN_CL_PARENTHESIS)
			shell->tokens = shell->tokens->next;
		else
			return (NULL);
	}
	else
		node = parse_command(shell);
	return (node);
}

t_ast_node	*parse_command(t_minishell *shell)
{
	t_ast_node	*node;
	t_token		*tmp;

	shell->sigint_heredocs = 0;
	node = create_ast_node(NODE_COMMAND, NULL, NULL, shell);
	while (shell->tokens && !is_statement_delimiter(shell->tokens->type)
		&& shell->tokens->type != TOKEN_OP_PARENTHESIS
		&& shell->tokens->type != TOKEN_CL_PARENTHESIS)
	{
		if (shell->tokens->type == TOKEN_HEREDOC && !shell->sigint_heredocs)
			init_heredoc(shell, shell->tokens);
		tmp = shell->tokens;
		shell->tokens = shell->tokens->next;
		tmp->next = NULL;
		ft_tkadd_back(&node->tokens, tmp);
	}
	return (node);
}

t_ast_node	*create_ast_node(t_node_type type, t_ast_node *left,
		t_ast_node *right, t_minishell *shell)
{
	t_ast_node	*new;

	new = gc_calloc(&shell->gc_head, 1, sizeof(t_ast_node));
	if (!new)
	{
		perror("minishell: create_ast_node");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	new->tokens = NULL;
	new->type = type;
	new->left = left;
	new->right = right;
	return (new);
}
