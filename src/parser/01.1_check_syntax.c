/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01.1_check_syntax.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 23:10:07 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/27 11:17:49 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_pipe_syntax(char *input, int index, bool in_single_quotes,
				bool in_double_quotes);
static int	check_quotes_balanced(const char *input);
static int	check_redirection_syntax(char *input);
static int	validate_redirection_at_position(char *input, int pos);

int	validate_syntax(t_shell *shell)
{
	if (!shell->input)
		return (SYNTAX_ERROR);
	if (check_pipe_syntax(shell->input, 0, false, false) == SYNTAX_ERROR)
	{
		ft_putendl_fd(ERROR_PIPE_SYNTAX, STDERR_FILENO);
		shell->exit_status = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	if (check_quotes_balanced(shell->input) == SYNTAX_ERROR)
	{
		ft_putendl_fd(ERROR_QUOTE_SYNTAX, STDERR_FILENO);
		shell->exit_status = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	if (check_redirection_syntax(shell->input) == SYNTAX_ERROR)
	{
		ft_putendl_fd(ERROR_REDIR_SYNTAX, STDERR_FILENO);
		shell->exit_status = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

static int	check_pipe_syntax(char *input, int index, bool in_single_quotes,
			bool in_double_quotes)
{
	int		position;

	if (!input)
		return (SYNTAX_ERROR);
	while (input[index] && is_space(input[index]))
		index++;
	if (is_pipe(input[index]))
		return (SYNTAX_ERROR);
	while (input[index])
	{
		update_quote_state(input[index], &in_single_quotes, &in_double_quotes);
		if (!in_single_quotes && !in_double_quotes && is_pipe(input[index]))
		{
			if (is_pipe(input[index + 1]))
				return (SYNTAX_ERROR);
			position = index + 1;
			while (input[position] && is_space(input[position]))
				position++;
			if (!input[position] || is_pipe(input[position]))
				return (SYNTAX_ERROR);
		}
		index++;
	}
	return (SUCCESS);
}

static int	check_quotes_balanced(const char *input)
{
	int		index;
	bool	in_single_quotes;
	bool	in_double_quotes;

	if (!input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO),
			SYNTAX_ERROR);
	index = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (input[index])
	{
		update_quote_state(input[index], &in_single_quotes, &in_double_quotes);
		index++;
	}
	if (in_single_quotes || in_double_quotes)
		return (SYNTAX_ERROR);
	else
		return (SUCCESS);
}

static int	check_redirection_syntax(char *input)
{
	int	index;
	int	redirect_position;

	index = 0;
	if (!input)
		return (FAILURE);
	while (input[index])
	{
		redirect_position = find_redirection_outside_quotes(input, index);
		if (redirect_position != (int)ft_strlen(input))
		{
			if (validate_redirection_at_position(input,
					redirect_position) == SYNTAX_ERROR)
				return (SYNTAX_ERROR);
			index = redirect_position + 1;
		}
		else
			break ;
	}
	return (SUCCESS);
}

static int	validate_redirection_at_position(char *input, int position)
{
	int	next_position;
	int	operator_len;

	operator_len = get_operator_length(input, position);
	if (operator_len == 0)
		return (SYNTAX_ERROR);
	next_position = position + operator_len;
	while (input[next_position] && is_space(input[next_position]))
		next_position++;
	if (!input[next_position])
		return (SYNTAX_ERROR);
	if (is_redirection(input[next_position]) || is_pipe(input[next_position]))
		return (SYNTAX_ERROR);
	return (SUCCESS);
}
