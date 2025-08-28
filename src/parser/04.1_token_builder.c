/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04.1_token_builder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:26:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 19:59:19 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int				not_is_special_tkn(t_data_token	*d_tkn, int i);
static void		add_token_node(t_token **token_list,
					char *word, int token_type);
static t_token	*find_token_last_node(t_token *token_list);
static t_token	*init_token_node(char *input, int type_token);

int	not_is_special_tkn(t_data_token	*d_tkn, int i)
{
	while (!is_space(d_tkn->raw_w[i])
		&& !is_redirection(d_tkn->raw_w[i])
		&& !is_pipe(d_tkn->raw_w[i])
		&& !is_quote(d_tkn->raw_w[i])
		&& !is_expansion_char(d_tkn->raw_w[i])
		&& d_tkn->raw_w[i])
		i++;
	return (i);
}

int	create_token(t_token **token_list, t_data_token *data_token)
{
	char	*token_input;

	if (!token_list || data_token->start_index < 0
		|| data_token->token_len <= 0)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);
	token_input = ft_substr(data_token->raw_w, data_token->start_index,
			data_token->token_len);
	if (!token_input)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);
	add_token_node(token_list, token_input, data_token->token_type);
	free(token_input);
	return (data_token->token_len);
}

static void	add_token_node(t_token **token_list, char *word, int token_type)
{
	t_token	*new_node;
	t_token	*last_node;

	if (!token_list || !word)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	new_node = init_token_node(word, token_type);
	if (!new_node)
		return (ft_putendl_fd(ERROR_TOKEN_INIT, STDERR_FILENO));
	last_node = find_token_last_node(*token_list);
	if (!last_node)
		*token_list = new_node;
	else
		last_node->next = new_node;
}

static t_token	*init_token_node(char *word, int token_type)
{
	t_token	*new_node;

	if (!word)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	new_node = (t_token *) malloc(sizeof(t_token));
	if (!new_node)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), NULL);
	new_node->raw_tkn = ft_strdup(word);
	if (!new_node->raw_tkn)
		return (ft_putendl_fd(ERROR_TOKEN_INIT, STDERR_FILENO), NULL);
	new_node->type = token_type;
	new_node->exp_token = NULL;
	new_node->noquotes_token = NULL;
	new_node->exp_list = NULL;
	new_node->next = NULL;
	return (new_node);
}

static t_token	*find_token_last_node(t_token *token_list)
{
	if (!token_list)
		return (NULL);
	while (token_list->next)
		token_list = token_list-> next;
	return (token_list);
}
