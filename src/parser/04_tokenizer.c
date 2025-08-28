/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 19:58:37 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	word_tokenizer(t_word *word);
static int	noquotes_tokenizer(t_word *word, int start_index);
static int	quotes_tokenizer(t_word *word, int start_index);
static int	operator_tokenizer(t_word *word, int start_index);

void	tokenizer(t_word *words_list, t_shell *shell)
{
	t_word	*current_word;

	if (!words_list || !shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_word = (t_word *) words_list;
	while (current_word)
	{
		if (word_tokenizer(current_word) == FAILURE)
			shell->exit_status = ERROR;
		current_word = current_word->next;
	}
}

static int	word_tokenizer(t_word *word)
{
	char	*raw_word;
	int		index;
	int		token_len;

	if (!word)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	raw_word = word->raw_w;
	index = 0;
	while (raw_word[index])
	{
		while (is_space(raw_word[index]))
			index++;
		if (!raw_word[index])
			break ;
		if (is_quote(raw_word[index]))
			token_len = quotes_tokenizer(word, index);
		else if (is_redirection(raw_word[index]) || is_pipe(raw_word[index]))
			token_len = operator_tokenizer(word, index);
		else
			token_len = noquotes_tokenizer(word, index);
		if (token_len == FAILURE)
			return (FAILURE);
		index = advance_index_by_length(index, token_len);
	}
	return (SUCCESS);
}

static int	noquotes_tokenizer(t_word *word, int start_index)
{
	int				i;
	t_data_token	d_tkn;

	if (!word || !word->raw_w || start_index < 0)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	d_tkn.raw_w = (char *) word->raw_w;
	i = start_index;
	d_tkn.start_index = start_index;
	if (is_expansion_char(d_tkn.raw_w[i]))
	{
		i++;
		if (d_tkn.raw_w[i] == '?')
			i++;
		else if (ft_isalpha(d_tkn.raw_w[i]) || d_tkn.raw_w[i] == '_')
			while ((ft_isalnum(d_tkn.raw_w[i]) || d_tkn.raw_w[i] == '_')
				&& d_tkn.raw_w[i])
				i++;
	}
	else
		i = not_is_special_tkn(&d_tkn, i);
	d_tkn.token_len = i - start_index;
	d_tkn.token_type = NO_QUOTES;
	create_token(&word->tokens_list, &d_tkn);
	return (d_tkn.token_len);
}

static int	quotes_tokenizer(t_word *word, int start_index)
{
	t_data_token	d_token;
	char			delimiter;
	int				end_position;

	d_token.start_index = start_index;
	if (!word || !word->raw_w || start_index < 0)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	d_token.raw_w = (char *) word->raw_w;
	delimiter = d_token.raw_w[start_index];
	if (delimiter == '\"')
		d_token.token_type = DOUBLE_QUOTES;
	else if (delimiter == '\'')
		d_token.token_type = SINGLE_QUOTES;
	else
		d_token.token_type = NO_QUOTES;
	end_position = find_matching_quote_position(d_token.raw_w, start_index);
	if (end_position == FAILURE)
		return (ft_putendl_fd(ERROR_QUOTE_SYNTAX, STDERR_FILENO), FAILURE);
	d_token.token_len = end_position - start_index;
	create_token(&word->tokens_list, &d_token);
	return (d_token.token_len);
}

static int	operator_tokenizer(t_word *word, int start_index)
{
	t_data_token	d_token;

	if (!word || start_index < 0)
		return (FAILURE);
	d_token.raw_w = (char *) word->raw_w;
	d_token.start_index = start_index;
	d_token.token_type = OPERATOR;
	d_token.token_len = get_operator_length(d_token.raw_w, start_index);
	if (d_token.token_len == 0)
		return (FAILURE);
	if (d_token.token_len == 2 && d_token.raw_w[start_index] == '>')
		d_token.raw_w = ">>";
	else if (d_token.token_len == 2 && d_token.raw_w[start_index] == '<')
		d_token.raw_w = "<<";
	else if (d_token.token_len == 1 && d_token.raw_w[start_index] == '>')
		d_token.raw_w = ">";
	else if (d_token.token_len == 1 && d_token.raw_w[start_index] == '<')
		d_token.raw_w = "<";
	else if (d_token.token_len == 1 && is_pipe(d_token.raw_w[start_index]))
		d_token.raw_w = "|";
	else
		return (FAILURE);
	create_token(&word->tokens_list, &d_token);
	return (d_token.token_len);
}
