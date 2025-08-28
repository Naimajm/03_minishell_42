/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_execution_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:44:56 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/27 11:13:52 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//static void	extract_command_arguments(t_cmd *command);
static void	fill_command_arguments(t_cmd *command);
static void	process_word_list(t_cmd *command, t_word *current_word, int index);
static void	process_redirections(t_cmd *command);
// static int	count_word_arguments(t_word *words_list);
static bool	is_builtin_command(char *command);

void	build_execution_structure(t_cmd *commands_list, t_shell *shell)
{
	t_cmd	*current_command;

	/*if (!commands_list) // arregla el espacio para asimilarlo a bash. este if hacia que el comando espacio no se comportara como en bash
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));*/
	current_command = (t_cmd *) commands_list;
	while (current_command)
	{
		fill_command_arguments(current_command); // AÑADIR WORD A LISTA DE ARGUMENTOS
		process_redirections(current_command); // PROCESAR REDIRECCIONAMIENTOS SOLO UNA VEZ
		if (current_command->exit_status == ERROR) // gestion EXIT STATUS
			shell->exit_status = ERROR;
		else
			shell->exit_status = SUCCESS;
		current_command = current_command->next;
	}
}

// test 60/75
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
	while (current_word) // args_count = count_word_arguments(current_word); // CREACION LISTA ARGS // calculo total argumentos -> WORD (no operadores)
	{
		if (current_word->word_type == WORD)
			args_count++;
		current_word = current_word->next;
	}
	if (!command->args)
		command->args = (char **) ft_calloc(sizeof(char *), (args_count + 2)); // reserva memoria total argumentos
	else
		while (command->args[index])
			index++;
	current_word = (t_word *) command->words_list; // inicializamos current word ya que la ciclamos antes para contar los argumentos y se queda apuntando a null al final, por lo que la siguiente funcion no entraria
	process_word_list(command, current_word, index); // CICLO PROCESAMIENTO LISTA WORDS
	if (command->args[0]) // verificar si es comando builtin
		command->is_btn = is_builtin_command(command->args[0]);
}

static void	process_word_list(t_cmd *command, t_word *curr_word, int index) // CICLO PROCESAMIENTO LISTA WORDS
{
	while (curr_word)
	{
		if (curr_word->word_type == WORD)
		{
			if (curr_word->processed_word && ft_strcmp(curr_word->processed_word, "") == 0 // TODO: añadida validacion curr_word->processed_word && para que no diera segfault en un caso
				&& curr_word->next)
			{
				curr_word = curr_word->next; // saltar palabra vacia
				continue ;
			}
			command->args[index] = ft_strdup(curr_word->processed_word);
			if (!command->args[index])
				return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO));
			index++;
		}
		else if (curr_word->word_type != WORD && curr_word->word_type != PIPE)
		{
			curr_word = curr_word->next; // SALTAR OPERADOR DE REDIRECCIÓN Y SU ARCHIVO // Saltar operador (< > >> <<)
			if (curr_word && curr_word->word_type == WORD)
				curr_word = curr_word->next; // Saltar nombre de archivo
			continue ;
		}
		curr_word = curr_word->next;
	}
	command->args[index] = NULL; // terminador nulo char **
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

// calculo total argumentos -> WORD (no operadores)
/*static int	count_word_arguments(t_word *words_list)
{
	t_word	*current_word;
	int		args_count;

	if (!words_list)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), ERROR);
	current_word = (t_word *) words_list;
	args_count = 0;
	while (current_word)
	{
		if (current_word->word_type == WORD)
			args_count++;
		current_word = current_word->next;
	}
	return (args_count);
}*/

// Verificar si commando es builtin
static bool	is_builtin_command(char *command)
{
	int			index;
	bool		is_builtin;
	const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset",
		"env", "exit", NULL}; // la ponemos constante para que pase la norminette y deje asignar a la vez que declaramos
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

// TODO: PREGUNTAR A JUAN ESTO PORQUE
/* static bool is_builtin_command(char *command)
{
	bool	is_builtin;

	if (!command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), false);
	is_builtin = false;
	if (!ft_strncmp(command, "echo", ft_strlen("echo") + 1)) // + 1 para incluir el carácter nulo '\0' al final de la cadena yno tener errores del tipo echoo.  por seguridad para asegurarse de que la comparación incluya el carácter nulo terminador (\0) al final de la cadena. es un patrón común en C para asegurarse de que estamos comparando comandos exactos y no prefijos de otros comandos más largos.
		is_builtin = true;
	else if (!ft_strncmp(command, "cd", ft_strlen("cd") + 1))
		is_builtin = true;
	else if (!ft_strncmp(command, "pwd", ft_strlen("pwd")))
		is_builtin = true;
	else if (!ft_strncmp(command, "export", ft_strlen("export") + 1))
		is_builtin = true;
	else if (!ft_strncmp(command, "unset", ft_strlen("unset") + 1))
		is_builtin = true;
	else if (!ft_strncmp(command, "env", ft_strlen("env") + 1))
		is_builtin = true;
	else if (!ft_strncmp(command, "exit", ft_strlen("exit") + 1))
		is_builtin = true;
	return (is_builtin);
} */
