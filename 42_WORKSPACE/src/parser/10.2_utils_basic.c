/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.2_utils_basic.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:46:07 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/17 12:03:07 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	get_operator_length(char *input, int index);
int	is_word_delimiter(char character);
int	is_redirection(char character);
int	is_pipe(char character);
int	is_quote(char character);

int	get_operator_length(char *input, int index)
{
	int	len;

	if (!input || !input[index])
		return (0);
	len = 0;
	if (input[index] == '>' && input[index + 1] == '>')
		return (len = 2);
	if (input[index] == '<' && input[index + 1] == '<')
		return (len = 2);
	if (input[index] == '>' || input[index] == '<')
		return (len = 1);
	if (input[index] == '|')
		return (len = 1);
	return (len);
}

int	is_word_delimiter(char character)
{
	int	delimiter;

	delimiter = 0;
	if (is_space(character)
		|| is_redirection(character)
		|| is_pipe(character))
		delimiter = 1;
	return (delimiter);
}

int	is_redirection(char character)
{
	int	operator;

	operator = 0;
	if (character == '>' || character == '<')
		operator = 1;
	return (operator);
}

int	is_pipe(char character)
{
	int	pipe;

	if (!character)
		return (0);
	pipe = 0;
	if (character == '|')
		pipe = 1;
	return (pipe);
}

int	is_quote(char character)
{
	int	quote;

	quote = 0;
	if (character == '\'' || character == '\"')
		quote = 1;
	return (quote);
}
