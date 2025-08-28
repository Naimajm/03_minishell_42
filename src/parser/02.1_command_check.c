/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.1_command_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:42:58 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 20:11:36 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	validate_pipe_structure(t_cmd *command);
static int	validate_redirections_structure(t_cmd *command);
static int	count_commands_nodes(t_cmd *command_list);

int	validate_command_structure(t_shell *shell)
{
	t_cmd	*current_command;

	if (!shell)
		return (SYNTAX_ERROR);
	current_command = (t_cmd *) shell->commands_list;
	if (count_commands_nodes(current_command) > MAX_PID)
		return (SYNTAX_ERROR);
	while (current_command)
	{
		if (validate_pipe_structure(current_command) == SYNTAX_ERROR)
		{
			shell->exit_status = SYNTAX_ERROR;
			return (ft_putendl_fd(ERROR_PIPE_SYNTAX, STDERR_FILENO),
				SYNTAX_ERROR);
		}
		if (validate_redirections_structure(current_command) == SYNTAX_ERROR)
		{
			shell->exit_status = SYNTAX_ERROR;
			return (ft_putendl_fd(ERROR_REDIR_SYNTAX, STDERR_FILENO),
				SYNTAX_ERROR);
		}
		current_command = current_command->next;
	}
	return (SUCCESS);
}

static int	validate_pipe_structure(t_cmd *command)
{
	t_word	*first_word;
	t_word	*last_word;

	if (!command->command || ft_strlen(command->command) == 0)
		return (SYNTAX_ERROR);
	first_word = (t_word *) command->words_list;
	if (first_word && first_word->word_type == PIPE)
		return (SYNTAX_ERROR);
	last_word = find_word_last_node(command->words_list);
	if (last_word && is_pipe(last_word->word_type))
		return (SYNTAX_ERROR);
	return (SUCCESS);
}

static int	validate_redirections_structure(t_cmd *command)
{
	t_word	*current_word;

	current_word = (t_word *) command->words_list;
	while (current_word)
	{
		if (current_word->word_type != WORD || current_word->word_type != PIPE)
		{
			if (!current_word->next || current_word->next->word_type != WORD)
				return (SYNTAX_ERROR);
			if (current_word->next && (current_word->next->word_type != WORD
					|| current_word->next->word_type != PIPE))
				return (SYNTAX_ERROR);
		}
		current_word = current_word->next;
	}
	return (SUCCESS);
}

static int	count_commands_nodes(t_cmd *command_list)
{
	t_cmd	*current_command;
	int		count;

	current_command = (t_cmd *) command_list;
	count = 0;
	while (current_command)
	{
		count++;
		current_command = current_command->next;
	}
	return (count);
}
