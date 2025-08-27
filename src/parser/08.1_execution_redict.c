/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08.1_execution_redict.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:16:26 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/17 12:55:49 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int			process_infile(t_cmd *command, t_word *word); // 08_executor_builder.c
int			process_outfile(t_cmd *command, t_word *word);
int			process_append(t_cmd *command, t_word *word);
int			process_heredoc(t_cmd *command, t_word *word);
static void	print_redir_err(t_cmd *command, char *file, char *mesasage);

int	process_infile(t_cmd *command, t_word *word)
{
	//t_cmd	*current_command;
	//current_command = (t_cmd *) command;
	if (command->infile)
		free(command->infile); ///  liberar existente si redict consedcutivos
	command->infile = ft_strdup(word->next->processed_word);
	if (!command->infile)
		return (FAILURE);
	if (access(command->infile, R_OK) == FAILURE) // verificacion permisos archivos
	{
		print_redir_err(command, command->infile, ERROR_FILE_NOT_FOUND);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	process_outfile(t_cmd *command, t_word *word)
{
	int	fd;

	if (command->outfile)
		free(command->outfile);
	command->outfile = ft_strdup(word->next->processed_word);
	if (!command->outfile)
		return (FAILURE);
	command->append = false;
	fd = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == FAILURE)
	{
		print_redir_err(command, command->outfile, ERROR_PERMISSION_DENIED);
		return (FAILURE);
	}
	else
	{
		if (close(fd) == FAILURE)
		{
			command->exit_status = ERROR;
			ft_putendl_fd(ERROR_CLOSING_FILE, STDERR_FILENO);
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

int	process_append(t_cmd *command, t_word *word)
{
	int	fd;

	if (command->outfile)
		free(command->outfile);
	command->outfile = ft_strdup(word->next->processed_word);
	if (!command->outfile)
		return (FAILURE);
	command->append = true;
	fd = open(command->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == FAILURE)
	{
		print_redir_err(command, command->outfile, ERROR_PERMISSION_DENIED);
		return (FAILURE);
	}
	else
	{
		if (close(fd) == FAILURE)
		{
			command->exit_status = ERROR;
			ft_putendl_fd(ERROR_CLOSING_FILE, STDERR_FILENO);
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

int	process_heredoc(t_cmd *command, t_word *word)
{
	if (command->delimiter)
		free(command->delimiter);
	command->delimiter = ft_strdup(word->next->processed_word);
	if (!command->delimiter)
		return (FAILURE);
	command->hd = true;
	return (SUCCESS);
}

static void	print_redir_err(t_cmd *command, char *file, char *mesasage)
{
	ft_putstr_fd("minishell: `", STDERR_FILENO); // sin salto de linea
	ft_putstr_fd(file, STDERR_FILENO); // sin salto de linea
	ft_putendl_fd(mesasage, STDERR_FILENO);
	command->exit_status = ERROR;
}
