/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_command_generate.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:01:32 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 19:52:14 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	generate_command(t_shell *shell, int start_index);
static int	create_clean_command(t_cmd **commands_list, char *raw_input,
				int start_index, int command_len);

void	create_commands_structure(t_shell *shell)
{
	int	index;
	int	input_len;

	if (!shell || !shell->input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	index = 0;
	while (shell->input[index])
	{
		while (is_space(shell->input[index]))
			index++;
		if (!shell->input[index])
			break ;
		input_len = generate_command(shell, index);
		if (input_len <= 0)
		{
			index++;
			continue ;
		}
		index = advance_index_by_length(index, input_len);
		while (is_space(shell->input[index]))
			index++;
		if (is_pipe(shell->input[index]))
			index++;
	}
}

static int	generate_command(t_shell *shell, int start_index)
{
	int	pipe_position;
	int	command_len;

	if (!shell || start_index < 0)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	pipe_position = find_pipe_outside_quotes(shell->input, start_index);
	command_len = pipe_position - start_index;
	if (command_len > 0)
		create_clean_command(&shell->commands_list, shell->input,
			start_index, command_len);
	return (command_len);
}

static int	create_clean_command(t_cmd **commands_list, char *raw_input,
			int start_index, int command_len)
{
	char	*command_input;
	char	*clean_command;

	if (!commands_list || !raw_input || start_index < 0 || command_len <= 0)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);
	command_input = ft_substr(raw_input, start_index, command_len);
	if (!command_input)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);
	clean_command = ft_strtrim(command_input, " \t\n\r");
	if (!clean_command)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);
	if (ft_strlen(clean_command) > 0)
		add_command_node(commands_list, clean_command);
	else
	{
		free(clean_command);
		return (ft_putendl_fd(ERROR_COMMAND_EMPTY, STDERR_FILENO), FAILURE);
	}
	free(command_input);
	free(clean_command);
	return (command_len);
}
