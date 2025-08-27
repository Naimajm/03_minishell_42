/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_variable_expander.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 11:59:59 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	extract_expansion_nodes(t_token *token, int index);
static void	resolve_expansion_values(t_token *token, t_shell *shell);
static void	insert_expansion_values(t_token *token);
static int	subs_len_asign(t_token *token, int index);

void	variable_expander(t_word *words_list, t_shell *shell)
{
	t_word	*current_word;
	t_token	*current_token;

	if (!words_list || !shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_word = (t_word *) words_list;
	while (current_word)
	{
		current_token = (t_token *) current_word->tokens_list;
		while (current_token)
		{
			if (!current_token)
			{
				shell->exit_status = ERROR;
				ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO);
				return ;
			}
			extract_expansion_nodes(current_token, 0);
			resolve_expansion_values(current_token, shell);
			insert_expansion_values(current_token);
			current_token = current_token->next;
		}
		current_word = current_word->next;
	}
}

// GENERAR LISTAS NODOS EXPAND, KEY, VALUE
static void	extract_expansion_nodes(t_token *token, int index)
{
	int	subs_len;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	if (token->type == SINGLE_QUOTES) // Para tokens literales, no hay expansiones
		return ;
	while (token->raw_tkn[index]) // CATEGORIZACION EXPAND NODOS
	{
		if (is_expansion_char(token->raw_tkn[index]))
		{
			if (!token->raw_tkn[index + 1]
				|| is_space(token->raw_tkn[index + 1])
				|| token->raw_tkn[index + 1] == '"') // Caso $ sin nada después O $ seguido de espacio -> NO ES EXPANSIÓN, saltar
				index++; // tratar $ como texto normal
			else if (token->raw_tkn[index + 1]) // Caso $ seguido de carácter válido para expansión
			{
				subs_len = subs_len_asign(token, index);
				if (subs_len == FAILURE) // error
					return ;
				index += subs_len;
			}
		}
		else
			index++; // resto caracteres
	}
}

// resolver valores	
static void	resolve_expansion_values(t_token *token, t_shell *shell)
{
	t_expand	*expand_node;
	char		*value;

	if (!token || !shell->environment)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	expand_node = (t_expand *) token->exp_list;
	while (expand_node)
	{
		if (expand_node->type == LAST_EXIT_STATUS)
			value = ft_itoa(shell->exit_status);
		else if (expand_node->type == LITERAL)
			value = ft_strdup(expand_node->key);
		else
			value = get_environment_var(shell->environment, expand_node->key);
		if (!value && (expand_node->type == LAST_EXIT_STATUS
				|| expand_node->type == LITERAL))
			return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO));
		expand_node->value = value;
		expand_node = expand_node->next;
	}
}

// INSERTAR VALOR EN TOKEN -> FINAL TOKEN
static void	insert_expansion_values(t_token *token)
{
	int	last_position;

	if (!token || !token->exp_list || token->type == SINGLE_QUOTES) // tokens literales o NO HAY EXPANSION VARIABLE
	{
		token->exp_token = ft_strdup(token->raw_tkn);
		if (!token->exp_token)
			return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO));
		return ;
	}
	last_position = insert_expand_node_value(token, 0); // insertar valores expandidos de cada nodo
	// Añadir el resto del token después de la última expansión
	if (token->raw_tkn[last_position])
		token->exp_token = ft_strjoin_free(token->exp_token,
				&token->raw_tkn[last_position]);
}

static int	subs_len_asign(t_token *token, int index)
{
	int	subs_len;

	if (token->raw_tkn[index + 1] == '?') // GESTION CASO ESPECIAL "$?
		subs_len = last_exit_status_expander(token, index);
	else if (index > 0 && token->raw_tkn[index - 1] == '\\') // caso \$VAR -> literal , no inicia expancion
		subs_len = literal_expander(token, index, true);
	else if (token->raw_tkn[index + 1] == '{') // caso ${VAR}xx
		subs_len = curly_braces_expander(token, index, 0, true);
	else // caso normal -> EXPANSION BASICA 
		subs_len = basic_expander(token, index);
	return (subs_len);
}
