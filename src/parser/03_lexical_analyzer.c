/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_lexical_analyzer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:37:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 19:53:20 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	command_extractor(t_cmd *command);
static int	word_extractor(t_cmd *process_list, int start_index);
static int	operator_extractor(t_cmd *process_list, int start_index);

void	lexical_analyzer(t_cmd *current_command, t_shell *shell)
{
	if (!current_command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	if (command_extractor(current_command) == FAILURE)
	{
		ft_putendl_fd(ERROR_COMMAND_INIT, STDERR_FILENO);
		shell->exit_status = ERROR;
		return ;
	}
}

static int	command_extractor(t_cmd *command)
{
	char	*command_input;
	int		index;
	int		word_len;

	if (!command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	command_input = command->command;
	index = 0;
	while (command_input[index])
	{
		while (is_space(command_input[index]))
			index++;
		if (!command_input[index])
			break ;
		if (is_redirection(command_input[index])
			|| is_pipe(command_input[index]))
			word_len = operator_extractor(command, index);
		else
			word_len = word_extractor(command, index);
		if (word_len == FAILURE)
			return (FAILURE);
		index = advance_index_by_length(index, word_len);
	}
	return (SUCCESS);
}

static int	word_extractor(t_cmd *command, int start_index)
{
	int			word_end_position;
	t_data_word	data_word;

	data_word.start_index = start_index;
	data_word.cmd_input = command->command;
	if (!command->command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	word_end_position = find_word_end_outside_quotes(command->command,
			start_index);
	if (word_end_position == FAILURE)
		return (ft_putendl_fd(ERROR_COMMAND_INIT, STDERR_FILENO), FAILURE);
	data_word.w_len = word_end_position - start_index;
	data_word.w_type = WORD;
	if (data_word.w_len > 0)
		create_word(&command->words_list, &data_word);
	return (data_word.w_len);
}

static int	operator_extractor(t_cmd *command, int start_index)
{
	t_data_word	data_word;

	if (!command || start_index < 0)
		return (FAILURE);
	data_word.cmd_input = command->command;
	data_word.start_index = start_index;
	data_word.w_len = get_operator_length(data_word.cmd_input, start_index);
	if (data_word.w_len == 2 && data_word.cmd_input[start_index] == '>')
		data_word.w_type = APPEND;
	else if (data_word.w_len == 2 && data_word.cmd_input[start_index] == '<')
		data_word.w_type = HERE_DOC;
	else if (data_word.w_len == 1 && data_word.cmd_input[start_index] == '>')
		data_word.w_type = OUTFILE;
	else if (data_word.w_len == 1 && data_word.cmd_input[start_index] == '<')
		data_word.w_type = INFILE;
	else if (data_word.w_len == 1 && is_pipe(data_word.cmd_input[start_index]))
		data_word.w_type = PIPE;
	else
		return (FAILURE);
	create_word(&command->words_list, &data_word);
	return (data_word.w_len);
}
