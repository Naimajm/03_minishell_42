/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.2_command_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:11:43 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/17 13:47:43 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_cmd	*init_command_node(char *input);
static t_cmd	*find_command_last_node(t_cmd *commands_list);

void	add_command_node(t_cmd **commands_list, char *input)
{
	t_cmd	*new_node;
	t_cmd	*last_node;

	if (!commands_list || !input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	new_node = init_command_node(input);
	if (!new_node)
		return (ft_putendl_fd(ERROR_COMMAND_INIT, STDERR_FILENO));
	last_node = find_command_last_node(*commands_list);
	if (!last_node)
		*commands_list = new_node;
	else
		last_node->next = new_node;
}

static t_cmd	*init_command_node(char *input)
{
	t_cmd	*new_node;

	if (!input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	new_node = (t_cmd *) malloc(sizeof(t_cmd));
	if (!new_node)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), NULL);
	new_node->command = ft_strdup(input);
	if (!new_node->command)
		return (ft_putendl_fd(ERROR_COMMAND_INIT, STDERR_FILENO), NULL);
	new_node->args = NULL;
	new_node->infile = NULL;
	new_node->outfile = NULL;
	new_node->delimiter = NULL;
	new_node->append = false;
	new_node->hd = false;
	new_node->is_btn = false;
	new_node->exit_status = SUCCESS;
	new_node->words_list = NULL;
	new_node->next = NULL;
	return (new_node);
}

static t_cmd	*find_command_last_node(t_cmd *commands_list)
{
	if (!commands_list)
		return (NULL);
	while (commands_list->next)
		commands_list = commands_list-> next; // retorna puntero a ultimo nodo   
	return (commands_list);
}
