/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   07_semantic_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:15:07 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 20:07:14 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	has_executable_word(t_cmd *command);
static int	validate_multiple_redirections(t_cmd *command);

int	validate_command_semantics(t_shell *shell)
{
	t_cmd	*current_command;

	if (!shell)
		return (SYNTAX_ERROR);
	current_command = (t_cmd *) shell->commands_list;
	while (current_command)
	{
		if (!has_executable_word(current_command))
		{
			ft_putendl_fd(ERROR_COMMAND_EMPTY, STDERR_FILENO);
			shell->exit_status = SYNTAX_ERROR;
			return (SYNTAX_ERROR);
		}
		if (validate_multiple_redirections(current_command) == SYNTAX_ERROR)
		{
			ft_putendl_fd(ERROR_REDIR_SYNTAX, STDERR_FILENO);
			shell->exit_status = SYNTAX_ERROR;
			return (SYNTAX_ERROR);
		}
		current_command = current_command->next;
	}
	return (SUCCESS);
}

static bool	has_executable_word(t_cmd *command)
{
	t_word	*current_word;

	current_word = command->words_list;
	while (current_word)
	{
		if (current_word->word_type == WORD)
			return (true);
		current_word = current_word->next;
	}
	return (false);
}

static int	validate_multiple_redirections(t_cmd *command)
{
	int		outfile_count;
	int		infile_count;
	t_word	*current_word;

	outfile_count = 0;
	infile_count = 0;
	current_word = (t_word *) command->words_list;
	while (current_word)
	{
		if (current_word->word_type == OUTFILE
			|| current_word->word_type == APPEND)
			outfile_count++;
		else if (current_word->word_type == INFILE
			|| current_word->word_type == HERE_DOC)
			infile_count++;
		current_word = current_word->next;
	}
	return (SUCCESS);
}
