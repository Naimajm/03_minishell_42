/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03.1_word_builder.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:44:40 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 19:55:20 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void		add_word_node(t_word **word_list, char *input, char word_type);
static t_word	*init_word_node(char *input, char word_type);
t_word			*find_word_last_node(t_word *word_list);

int	create_word(t_word **word_list, t_data_word *d_word)
{
	char	*w_input;

	if (!word_list || d_word->start_index < 0 || d_word->w_len <= 0)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);
	w_input = ft_substr(d_word->cmd_input, d_word->start_index, d_word->w_len);
	if (!w_input)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);
	add_word_node(word_list, w_input, d_word->w_type);
	free(w_input);
	return (d_word->w_len);
}

static void	add_word_node(t_word **word_list, char *input, char word_type)
{
	t_word	*new_node;
	t_word	*last_node;

	if (!word_list || !input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	new_node = init_word_node(input, word_type);
	if (!new_node)
		return (ft_putendl_fd(ERROR_WORD_INIT, STDERR_FILENO));
	last_node = find_word_last_node(*word_list);
	if (!last_node)
		*word_list = new_node;
	else
		last_node->next = new_node;
}

static t_word	*init_word_node(char *input, char word_type)
{
	t_word	*new_node;

	if (!input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	new_node = (t_word *) malloc(sizeof(t_word));
	if (!new_node)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), NULL);
	new_node->raw_w = ft_strdup(input);
	if (!new_node->raw_w)
		return (ft_putendl_fd(ERROR_WORD_INIT, STDERR_FILENO), NULL);
	new_node->word_type = word_type;
	new_node->processed_word = NULL;
	new_node->tokens_list = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_word	*find_word_last_node(t_word *word_list)
{
	if (!word_list)
		return (NULL);
	while (word_list->next)
		word_list = word_list-> next;
	return (word_list);
}
