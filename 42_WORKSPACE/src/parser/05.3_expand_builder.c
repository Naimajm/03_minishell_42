/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.3_expand_builder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:02:16 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 12:02:00 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void			print_expand_nodes_list(t_expand *expand_list);
int				insert_expand_node_value(t_token *token, int last_pos);
t_expand		*add_expand_node(t_expand **expand_list,
					char *substitution_variable, int first_index,
					int expand_type);
static t_expand	*init_expand_node(char *variable, int first_index,
					int expand_type);
static t_expand	*find_expand_last_node(t_expand *token_list);

void	print_expand_nodes_list(t_expand *expand_list)
{
	t_expand	*expand_node;
	int			node_index;

	if (!expand_list)
		return ;
	expand_node = (t_expand *)(expand_list);
	node_index = 1;
	while (expand_node)
	{
		printf("\t\t\t └────┐\n");
		printf("\t\t\t ┌───────────────────┐\n");
		printf("\t\t\t | expand_node: [%i]  |\n", node_index);
		printf("\t\t\t └───────────────────┘\n");
		printf("\t\t\t\t current node -> %p // next -> %p\n",
			expand_node, expand_node->next);
		printf("\t\t\t\t first_index \t-> %d\n", expand_node->first_i);
		printf("\t\t\t\t last_index \t-> %d\n", expand_node->last_index);
		printf("\t\t\t\t subs_variable \t-> %s\n",
			expand_node->substitution_str);
		printf("\t\t\t\t key \t\t-> %s\n", expand_node->key);
		printf("\t\t\t\t └──> value \t-> %s\n", expand_node->value);
		node_index++;
		expand_node = expand_node->next;
	}
}

int	insert_expand_node_value(t_token *tkn, int last_pos)
{
	t_expand	*curr_nod;
	char		*result;
	char		*prfx;

	if (!tkn)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	curr_nod = (t_expand *) tkn->exp_list;
	result = ft_strdup("");
	if (!result)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), FAILURE);
	while (curr_nod)
	{
		if (curr_nod->first_i > last_pos) // Añadir texto antes de la variable
		{
			prfx = ft_substr(tkn->raw_tkn, last_pos,
					curr_nod->first_i - last_pos); //printf("  prefix: '%s'\n", prefix);
			result = ft_strjoin_free(result, prfx);
			free(prfx);
		}
		result = ft_strjoin_free(result, curr_nod->value); // Añadir el valor expandido
		last_pos = curr_nod->last_index + 1;
		curr_nod = curr_nod->next;
	}
	tkn->exp_token = result;
	return (last_pos);
}

t_expand	*add_expand_node(t_expand **expand_list, char *substit_variable,
				int first_index, int expand_type)
{
	t_expand	*new_node;
	t_expand	*last_node;

	if (!expand_list || !substit_variable)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	new_node = init_expand_node(substit_variable, first_index, expand_type);
	if (!new_node)
		return (ft_putendl_fd(ERROR_EXPAND_INIT, STDERR_FILENO), NULL);
	last_node = find_expand_last_node(*expand_list);
	if (!last_node)
		*expand_list = new_node;
	else
		last_node->next = new_node;
	return (new_node);
}

static t_expand	*init_expand_node(char *substitution_variable,
	int first_index, int expand_type)
{
	t_expand	*new_node;

	if (!substitution_variable)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	new_node = (t_expand *) malloc(sizeof(t_expand));
	if (!new_node)
		return (ft_putendl_fd(ERROR_EXPAND_INIT, STDERR_FILENO), NULL);
	new_node->type = expand_type;
	new_node->first_i = first_index;
	new_node->substitution_str = ft_strdup(substitution_variable);
	if (!new_node->substitution_str)
		return (ft_putendl_fd(ERROR_EXPAND_INIT, STDERR_FILENO), NULL);
	new_node->last_index = first_index + ft_strlen(substitution_variable) - 1;
	new_node->key = NULL;
	new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

static t_expand	*find_expand_last_node(t_expand *token_list)
{
	if (!token_list)
		return (NULL);
	while (token_list->next)
		token_list = token_list-> next;
	return (token_list);
}
