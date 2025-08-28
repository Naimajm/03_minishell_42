/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_execution_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:44:56 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/27 11:14:56 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	fill_command_arguments(t_cmd *command);
static void	process_word_list(t_cmd *command, t_word *current_word, int index);
static void	process_redirections(t_cmd *command);
static bool	is_builtin_command(char *command);

void	build_execution_structure(t_cmd *commands_list, t_shell *shell)
{
	t_cmd	*current_command;

	current_command = (t_cmd *) commands_list;
	while (current_command)
	{
		fill_command_arguments(current_command);
		process_redirections(current_command);
		if (current_command->exit_status == ERROR)
			shell->exit_status = ERROR;
		else
			shell->exit_status = SUCCESS;
		current_command = current_command->next;
	}
}

static void	fill_command_arguments(t_cmd *command)
{
	int		index;
	int		args_count;
	t_word	*current_word;

	if (!command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	index = 0;
	current_word = (t_word *) command->words_list;
	args_count = 0;
	while (current_word)
	{
		if (current_word->word_type == WORD)
			args_count++;
		current_word = current_word->next;
	}
	if (!command->args)
		command->args = (char **) ft_calloc(sizeof(char *), (args_count + 2));
	else
		while (command->args[index])
			index++;
	current_word = (t_word *) command->words_list;
	process_word_list(command, current_word, index);
	if (command->args[0])
		command->is_btn = is_builtin_command(command->args[0]);
}

static void	process_word_list(t_cmd *command, t_word *curr_w, int index)
{
	while (curr_w)
	{
		if (curr_w->word_type == WORD)
		{
			if (curr_w->processed_word
				&& ft_strcmp(curr_w->processed_word, "") == 0 && curr_w->next)
			{
				curr_w = curr_w->next;
				continue ;
			}
			command->args[index] = ft_strdup(curr_w->processed_word);
			if (!command->args[index])
				return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO));
			index++;
		}
		else if (curr_w->word_type != WORD && curr_w->word_type != PIPE)
		{
			curr_w = curr_w->next;
			if (curr_w && curr_w->word_type == WORD)
				curr_w = curr_w->next;
			continue ;
		}
		curr_w = curr_w->next;
	}
	command->args[index] = NULL;
}

static void	process_redirections(t_cmd *command)
{
	t_word	*current_word;

	if (!command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_word = command->words_list;
	while (current_word)
	{
		if (current_word->word_type == OUTFILE
			&& current_word->next && current_word->next->word_type == WORD
			&& process_outfile(command, current_word) == FAILURE)
			break ;
		else if (current_word->word_type == APPEND
			&& current_word->next && current_word->next->word_type == WORD
			&& process_append(command, current_word) == FAILURE)
			break ;
		else if (current_word->word_type == INFILE
			&& current_word->next && current_word->next->word_type == WORD
			&& process_infile(command, current_word) == FAILURE)
			break ;
		else if (current_word->word_type == HERE_DOC
			&& current_word->next && current_word->next->word_type == WORD
			&& process_heredoc(command, current_word) == FAILURE)
			break ;
		current_word = current_word->next;
	}
}

static bool	is_builtin_command(char *command)
{
	int			index;
	bool		is_builtin;
	const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset",
		"env", "exit", NULL};

	if (!command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), false);
	index = 0;
	is_builtin = false;
	while (builtins[index])
	{
		if (ft_strncmp(command, builtins[index], ft_strlen(command)) == 0)
			is_builtin = true;
		index++;
	}
	return (is_builtin);
}
