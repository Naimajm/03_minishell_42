/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.1_expand_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:20:15 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 20:12:44 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	extract_single_variable(char *raw_token, int start_index);
int			last_exit_status_expander(t_token *token, int first_index);
int			curly_braces_expander(t_token *token, int first_i, int final_i,
				bool success);
int			literal_expander(t_token *token, int first_index, bool success);

int	basic_expander(t_token *token, int index)
{
	t_expand		*expand_node;
	t_data_token	data_token;
	bool			success;
	char			*key;

	success = true;
	data_token.token_len = extract_single_variable(token->raw_tkn, index);
	data_token.raw_w = ft_substr(token->raw_tkn,
			index, data_token.token_len);
	expand_node = add_expand_node(&token->exp_list,
			data_token.raw_w, index, BASIC_EXPANSION);
	key = extract_key(data_token.raw_w, 1);
	expand_node->key = ft_strdup(key);
	expand_node->last_index = index + data_token.token_len - 1;
	if (!token || data_token.token_len <= 0 || !data_token.raw_w
		|| !expand_node || !key || !expand_node->key)
		success = false;
	free(data_token.raw_w);
	free(key);
	if (success)
		return (data_token.token_len);
	return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
}

static int	extract_single_variable(char *raw_token, int start_index)
{
	int	index;

	if (!raw_token || !is_expansion_char(raw_token[start_index]))
		return (0);
	index = start_index + 1;
	if (is_space(raw_token[index]) || !raw_token[index])
		return (1);
	if (raw_token[index] == '?')
		return (2);
	if (!ft_isalpha(raw_token[index]) && raw_token[index] != '_')
		return (1);
	while ((ft_isalnum(raw_token[index]) || raw_token[index] == '_')
		&& raw_token[index])
		index++;
	return (index - start_index);
}

int	last_exit_status_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char		*substitution_str;
	int			len_input;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	substitution_str = ft_strdup("$?");
	if (!substitution_str)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), FAILURE);
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->exp_list, substitution_str,
			first_index, LAST_EXIT_STATUS);
	expand_node->key = ft_strdup("$?");
	if (!expand_node->key)
	{
		free(substitution_str);
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), FAILURE);
	}
	free(substitution_str);
	return (len_input);
}

int	curly_braces_expander(t_token *token, int first_i, int final_i,
		bool success)
{
	t_expand	*exp_node;
	char		*substi_str;
	int			len_input;
	char		*key;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	final_i = first_i + 2;
	while (token->raw_tkn[final_i] && token->raw_tkn[final_i] != '}')
		final_i++;
	if (token->raw_tkn[final_i] != '}')
		return (FAILURE);
	substi_str = ft_substr(token->raw_tkn, first_i, (final_i - first_i + 1));
	len_input = ft_strlen(substi_str);
	exp_node = add_expand_node(&token->exp_list, substi_str, first_i, CURLY_B);
	key = ft_strtrim(extract_key(substi_str, 1), "{}");
	exp_node->key = ft_strdup(key);
	if (!substi_str || !exp_node || !key || !exp_node->key)
		success = false;
	exp_node->last_index = first_i + len_input - 1;
	free(key);
	free(substi_str);
	if (!success)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), FAILURE);
	return (len_input);
}

int	literal_expander(t_token *token, int index, bool success)
{
	t_expand	*expand_node;
	char		*substitution_str;
	int			len_input;
	char		*key;
	int			first_index;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	first_index = index - 1;
	len_input = extract_single_variable(token->raw_tkn, first_index + 1);
	substitution_str = ft_substr(token->raw_tkn, first_index, len_input + 1);
	expand_node = add_expand_node(&token->exp_list, substitution_str,
			first_index, LITERAL);
	key = ft_substr(substitution_str, 1, ft_strlen(substitution_str) - 1);
	expand_node->key = ft_strdup(key);
	if (!substitution_str || !key || !expand_node || !expand_node->key)
		success = false;
	expand_node->last_index = index + len_input - 1;
	free(key);
	free(substitution_str);
	if (!success)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), FAILURE);
	return (len_input);
}
